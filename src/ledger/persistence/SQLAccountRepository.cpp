// Standard include(s)
#include <iostream>
#include <map>
#include <memory>
#include <stack>
#include <stdexcept>
#include <vector>

// Qt include(s)
#include <QtCore>
#include <QtSql>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include "SQLAccountRepository.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
QString SQLAccountRepository::table_name_ = "account";

//--------------------------------------------------------------------------------------------------
SQLAccountRepository::SQLAccountRepository(QSqlDatabase & db,
                                           std::shared_ptr<CurrencyRepository> currencies)
    : db_(db), currencies_(currencies) {
    QSqlQuery query(db);
    bool success = query.exec("CREATE TABLE IF NOT EXISTS " + table_name_ + "("
                              "id INTEGER PRIMARY KEY, "
                              "name VARCHAR NOT NULL, "
                              "currency_id INTEGER DEFAULT 1, "
                              "lft INTEGER NOT NULL, "
                              "rgt INTEGER NOT NULL, "
                              "archived BOOLEAN DEFAULT 0, "
                              "FOREIGN KEY(currency_id) REFERENCES currency(id) "
                              "ON DELETE SET DEFAULT);");

    if (not success) {
        last_error_ = query.lastError().text();
        throw std::runtime_error(last_error_.toStdString());
    }

    query.exec("SELECT id FROM " + table_name_ + " WHERE id=1;");
    if (not query.first() and not query.isValid()) {
        query.prepare("INSERT INTO " + table_name_ + "(id, name, lft, rgt) VALUES(1, 'root', 1, 2);");
        if (not query.exec()) {
            last_error_ = query.lastError().text();
            throw std::runtime_error(last_error_.toStdString());
        }
    }

    cache();
}

//--------------------------------------------------------------------------------------------------
void SQLAccountRepository::cache() {
    accounts_.clear();

    QSqlQuery query(db_);
    query.exec("SELECT * FROM " + table_name_ + " ORDER BY lft;");

    QSqlRecord record;

    std::stack<NestedSetAccount> parents;
    while (query.next()) {
        record = query.record();

        NestedSetAccount account(record.value("id").toInt());
        account.setName(record.value("name").toString());
        account.setCurrency(currencies_->getCurrency(record.value("currency_id").toInt()));
        account.lft = record.value("lft").toInt();
        account.rgt = record.value("rgt").toInt();

        // If not the root
        if (account.id() > 1) {
            // If this account is not a child of the current parent, move back up the tree
            while (account.lft > parents.top().rgt) {
                parents.pop();
            }

            account.setParent(parents.top().id());
            auto children = accounts_[account.parent()].children();
            children.push_back(account.id());
            accounts_[account.parent()].setChildren(children);
        }
        accounts_[account.id()] = account;

        // Push this account onto the stack as the next (potential) parent
        parents.push(account);
    }
}

//--------------------------------------------------------------------------------------------------
int SQLAccountRepository::create(const Account & account, const Account & parent) {
    auto iter = accounts_.find(parent.id());
    if (iter == accounts_.end()) {
        last_error_ = QObject::tr("No parent account found with ID %1").arg(parent.id());
        return -1;
    }
    int rgt = iter->second.rgt;

    db_.transaction();
    QSqlQuery query(db_);

    query.prepare("UPDATE " + table_name_ + " SET lft=lft+2 WHERE lft>=:lft;");
    query.bindValue(":lft", rgt);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return -1;
    }

    query.prepare("UPDATE " + table_name_ + " SET rgt=rgt+2 WHERE rgt>=:rgt;");
    query.bindValue(":rgt", rgt);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return -1;
    }

    query.prepare("INSERT INTO " + table_name_ + "(name, currency_id, lft, rgt) "
            "VALUES(:name, :currency, :lft, :rgt);");
    query.bindValue(":name", account.name());
    query.bindValue(":currency", account.currency().id());
    query.bindValue(":lft", rgt);
    query.bindValue(":rgt", rgt + 1);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return -1;
    }

    db_.commit();
    cache();
    return query.lastInsertId().toInt();
}

//--------------------------------------------------------------------------------------------------
Account SQLAccountRepository::getAccount(int id) {
    auto iter = accounts_.find(id);
    if (iter != accounts_.end()) {
        return iter->second;
    } else {
        last_error_ = QObject::tr("No account exists with ID %1").arg(id);
        return Account(-1);
    }
}

//--------------------------------------------------------------------------------------------------
Account SQLAccountRepository::getRoot() {
    return getAccount(1);
}

//--------------------------------------------------------------------------------------------------
QString SQLAccountRepository::lastError() {
    return last_error_;
}

//--------------------------------------------------------------------------------------------------
bool SQLAccountRepository::move(const Account & account, const Account & parent) {
    auto iter = accounts_.find(account.id());
    if (iter == accounts_.end()) {
        last_error_ = QObject::tr("No account found with ID %1").arg(account.id());
        return false;
    }
    auto acct = iter->second;

    iter = accounts_.find(account.parent());
    if (iter == accounts_.end()) {
        last_error_ = QObject::tr("No old parent account found with ID %1").arg(account.parent());
        return false;
    }
    auto oparent = iter->second;

    iter = accounts_.find(parent.id());
    if (iter == accounts_.end()) {
        last_error_ = QObject::tr("No new parent account found with ID %1").arg(parent.id());
        return false;
    }
    auto nparent = iter->second;

    // Using algorithm from https://rogerkeays.com/how-to-move-a-node-in-nested-sets-with-sql
    int newlft = nparent.rgt;
    int width = acct.rgt - acct.lft + 1;
    int distance = newlft - acct.lft;
    int tmppos = acct.lft;

    // Account for new space when moving backwards
    if (distance < 0) {
        distance -= width;
        tmppos += width;
    }

    db_.transaction();
    QSqlQuery query(db_);

    // First make space for the subtree

    query.prepare("UPDATE " + table_name_ + " SET lft=lft+:width WHERE lft>=:lft;");
    query.bindValue(":width", width);
    query.bindValue(":lft", newlft);
    if (not query.exec()) {
        last_error_ = "Unable to update lft to make space: " + query.lastError().text();
        db_.rollback();
        return false;
    }

    query.prepare("UPDATE " + table_name_ + " SET rgt=rgt+:width WHERE rgt>=:rgt;");
    query.bindValue(":width", width);
    query.bindValue(":rgt", newlft);
    if (not query.exec()) {
        last_error_ = "Unable to update rgt to make space: " + query.lastError().text();
        db_.rollback();
        return false;
    }

    // Move subtree into that space

    query.prepare("UPDATE " + table_name_ + " SET lft=lft+:distance, "
            "rgt=rgt+:distance WHERE lft>=:pos AND rgt<:pos+:width");
    query.bindValue(":distance", distance);
    query.bindValue(":width", width);
    query.bindValue(":pos", tmppos);
    if (not query.exec()) {
        last_error_ = "Unable to move subtree: " + query.lastError().text();
        db_.rollback();
        return false;
    }

    // Remove old space

    query.prepare("UPDATE " + table_name_ + " SET lft=lft-:width WHERE lft>:lft;");
    query.bindValue(":width", width);
    query.bindValue(":lft", acct.rgt);
    if (not query.exec()) {
        last_error_ = "Unable to update lft to remove space: " + query.lastError().text();
        db_.rollback();
        return false;
    }

    query.prepare("UPDATE " + table_name_ + " SET rgt=rgt-:width WHERE rgt>:rgt;");
    query.bindValue(":width", width);
    query.bindValue(":rgt", acct.rgt);
    if (not query.exec()) {
        last_error_ = "Unable to update rgt to remove space: " + query.lastError().text();
        db_.rollback();
        return false;
    }

    db_.commit();
    cache();
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLAccountRepository::remove(const Account & account) {
    auto iter = accounts_.find(account.id());
    if (iter == accounts_.end()) {
        last_error_ = QObject::tr("No account found with ID %1").arg(account.id());
        return false;
    }
    int lft = iter->second.lft;
    int rgt = iter->second.rgt;
    int width = rgt - lft + 1;

    db_.transaction();
    QSqlQuery query(db_);

    // Delete the subtree
    query.prepare("DELETE FROM " + table_name_ + " WHERE lft>=:lft AND rgt<=:rgt");
    query.bindValue(":lft", lft);
    query.bindValue(":rgt", rgt);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return false;
    }

    // Remove old space

    query.prepare("UPDATE " + table_name_ + " SET lft=lft-:width WHERE lft>:lft;");
    query.bindValue(":width", width);
    query.bindValue(":lft", rgt);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return false;
    }

    query.prepare("UPDATE " + table_name_ + " SET rgt=rgt-:width WHERE rgt>:rgt;");
    query.bindValue(":width", width);
    query.bindValue(":rgt", rgt);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return false;
    }

    db_.commit();
    cache();
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLAccountRepository::update(const Account & account) {
    QSqlQuery query(db_);
    query.prepare("UPDATE " + table_name_ + " SET name=:name, "
            "currency_id=:currency WHERE id=:id;");
    query.bindValue(":name", account.name());
    query.bindValue(":currency", account.currency().id());
    query.bindValue(":id", account.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    cache();
    return true;
}

} // ledger namespace
} // ub namespace