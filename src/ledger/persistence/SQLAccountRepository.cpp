/*
 * UnderBudget
 * Copyright 2017 Kyle Treubig
 *
 * UnderBudget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UnderBudget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UnderBudget.  If not, see <http://www.gnu.org/licenses/>.
 */

// Standard include(s)
#include <cstdint>
#include <iostream>
#include <map>
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
SQLAccountRepository::SQLAccountRepository(QSqlDatabase & db) : db_(db) {
    if (not db.tables().contains("currency")) {
        last_error_ = QObject::tr("Currency table does not exist");
        throw std::runtime_error(last_error_.toStdString());
    }

    QSqlQuery query(db);
    bool success = query.exec("CREATE TABLE IF NOT EXISTS " + table_name_ +
                              "("
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
        query.prepare("INSERT INTO " + table_name_ +
                      "(id, name, lft, rgt) VALUES(1, 'root', 1, 2);");
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
    query.exec("SELECT account.id,account.name,account.currency_id,currency.code,account.lft,"
               "account.rgt FROM " +
               table_name_ + " JOIN currency on account.currency_id=currency.id ORDER BY lft;");

    QSqlRecord record;

    std::stack<NestedSetAccount> parents;
    while (query.next()) {
        record = query.record();

        NestedSetAccount account(record.value("id").value<int64_t>());
        account.setName(record.value("name").toString());
        account.setCurrency(Currency(record.value("currency_id").value<int64_t>(),
                                     record.value("code").toString()));
        account.lft = record.value("lft").value<int64_t>();
        account.rgt = record.value("rgt").value<int64_t>();

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
int64_t SQLAccountRepository::create(const Account & account, const Account & parent) {
    auto iter = accounts_.find(parent.id());
    if (iter == accounts_.end()) {
        last_error_ = QObject::tr("No parent account found with ID %1").arg(parent.id());
        return -1;
    }
    int64_t rgt = iter->second.rgt;

    db_.transaction();
    QSqlQuery query(db_);

    query.prepare("UPDATE " + table_name_ + " SET lft=lft+2 WHERE lft>=:lft;");
    query.bindValue(":lft", QVariant::fromValue(rgt));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return -1;
    }

    query.prepare("UPDATE " + table_name_ + " SET rgt=rgt+2 WHERE rgt>=:rgt;");
    query.bindValue(":rgt", QVariant::fromValue(rgt));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return -1;
    }

    query.prepare("INSERT INTO " + table_name_ + "(name, currency_id, lft, rgt) "
                                                 "VALUES(:name, :currency, :lft, :rgt);");
    query.bindValue(":name", account.name());
    query.bindValue(":currency", QVariant::fromValue(account.currency().id()));
    query.bindValue(":lft", QVariant::fromValue(rgt));
    query.bindValue(":rgt", QVariant::fromValue(rgt + 1));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return -1;
    }

    db_.commit();
    cache();
    return query.lastInsertId().value<int64_t>();
}

//--------------------------------------------------------------------------------------------------
Account SQLAccountRepository::getAccount(int64_t id) {
    auto iter = accounts_.find(id);
    if (iter != accounts_.end()) {
        return iter->second;
    } else {
        last_error_ = QObject::tr("No account exists with ID %1").arg(id);
        return Account(-1);
    }
}

//--------------------------------------------------------------------------------------------------
std::vector<Account> SQLAccountRepository::getLeafAccounts() {
    std::vector<Account> accounts;

    QSqlQuery query(db_);
    query.prepare("SELECT id FROM " + table_name_ + " WHERE rgt=lft+1 ORDER BY name;");
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    while (query.next()) {
        int64_t id = query.record().value("id").value<int64_t>();
        // Don't ever include the root account
        if (id == 1) {
            continue;
        }
        auto iter = accounts_.find(id);
        if (iter != accounts_.end()) {
            accounts.push_back(iter->second);
        }
    }

    return accounts;
}

//--------------------------------------------------------------------------------------------------
Account SQLAccountRepository::getRoot() {
    return getAccount(1);
}

//--------------------------------------------------------------------------------------------------
QString SQLAccountRepository::lastError() const {
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
    int64_t newlft = nparent.rgt;
    int64_t width = acct.rgt - acct.lft + 1;
    int64_t distance = newlft - acct.lft;
    int64_t tmppos = acct.lft;

    // Account for new space when moving backwards
    if (distance < 0) {
        distance -= width;
        tmppos += width;
    }

    db_.transaction();
    QSqlQuery query(db_);

    // First make space for the subtree

    query.prepare("UPDATE " + table_name_ + " SET lft=lft+:width WHERE lft>=:lft;");
    query.bindValue(":width", QVariant::fromValue(width));
    query.bindValue(":lft", QVariant::fromValue(newlft));
    if (not query.exec()) {
        last_error_ = "Unable to update lft to make space: " + query.lastError().text();
        db_.rollback();
        return false;
    }

    query.prepare("UPDATE " + table_name_ + " SET rgt=rgt+:width WHERE rgt>=:rgt;");
    query.bindValue(":width", QVariant::fromValue(width));
    query.bindValue(":rgt", QVariant::fromValue(newlft));
    if (not query.exec()) {
        last_error_ = "Unable to update rgt to make space: " + query.lastError().text();
        db_.rollback();
        return false;
    }

    // Move subtree into that space

    query.prepare("UPDATE " + table_name_ +
                  " SET lft=lft+:distance, "
                  "rgt=rgt+:distance WHERE lft>=:pos AND rgt<:pos+:width");
    query.bindValue(":distance", QVariant::fromValue(distance));
    query.bindValue(":width", QVariant::fromValue(width));
    query.bindValue(":pos", QVariant::fromValue(tmppos));
    if (not query.exec()) {
        last_error_ = "Unable to move subtree: " + query.lastError().text();
        db_.rollback();
        return false;
    }

    // Remove old space

    query.prepare("UPDATE " + table_name_ + " SET lft=lft-:width WHERE lft>:lft;");
    query.bindValue(":width", QVariant::fromValue(width));
    query.bindValue(":lft", QVariant::fromValue(acct.rgt));
    if (not query.exec()) {
        last_error_ = "Unable to update lft to remove space: " + query.lastError().text();
        db_.rollback();
        return false;
    }

    query.prepare("UPDATE " + table_name_ + " SET rgt=rgt-:width WHERE rgt>:rgt;");
    query.bindValue(":width", QVariant::fromValue(width));
    query.bindValue(":rgt", QVariant::fromValue(acct.rgt));
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
    int64_t lft = iter->second.lft;
    int64_t rgt = iter->second.rgt;
    int64_t width = rgt - lft + 1;

    db_.transaction();
    QSqlQuery query(db_);

    // Delete the subtree
    query.prepare("DELETE FROM " + table_name_ + " WHERE lft>=:lft AND rgt<=:rgt");
    query.bindValue(":lft", QVariant::fromValue(lft));
    query.bindValue(":rgt", QVariant::fromValue(rgt));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return false;
    }

    // Remove old space

    query.prepare("UPDATE " + table_name_ + " SET lft=lft-:width WHERE lft>:lft;");
    query.bindValue(":width", QVariant::fromValue(width));
    query.bindValue(":lft", QVariant::fromValue(rgt));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        return false;
    }

    query.prepare("UPDATE " + table_name_ + " SET rgt=rgt-:width WHERE rgt>:rgt;");
    query.bindValue(":width", QVariant::fromValue(width));
    query.bindValue(":rgt", QVariant::fromValue(rgt));
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
    query.bindValue(":currency", QVariant::fromValue(account.currency().id()));
    query.bindValue(":id", QVariant::fromValue(account.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    cache();
    return true;
}

} // ledger namespace
} // ub namespace
