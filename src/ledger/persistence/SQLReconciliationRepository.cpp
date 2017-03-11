/*
 * Copyright 2017 Kyle Treubig
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Standard include(s)
#include <stdexcept>
#include <vector>

// Qt include(s)
#include <QtCore>
#include <QtSql>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountRepository.hpp>
#include <ledger/model/Reconciliation.hpp>
#include "SQLReconciliationRepository.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
QString SQLReconciliationRepository::table_name_ = "reconciliation";

//--------------------------------------------------------------------------------------------------
SQLReconciliationRepository::SQLReconciliationRepository(
    QSqlDatabase & db, std::shared_ptr<AccountRepository> accounts)
        : db_(db), accounts_(accounts) {
    if (not db_.tables().contains("account")) {
        last_error_ = QObject::tr("Account table does not exist");
        throw std::runtime_error(last_error_.toStdString());
    }

    QSqlQuery query(db_);
    bool success =
        query.exec(QString("CREATE TABLE IF NOT EXISTS %1 ("
                           "id INTEGER PRIMARY KEY, "
                           "account_id INTEGER NOT NULL, "
                           "beginning_balance INTEGER NOT NULL, "
                           "beginning_date DATE NOT NULL, "
                           "ending_balance INTEGER NOT NULL, "
                           "ending_date DATE NOT NULL, "
                           "FOREIGN KEY(account_id) REFERENCES account(id) ON DELETE CASCADE);")
                       .arg(table_name_));
    if (not success) {
        last_error_ = query.lastError().text();
        throw std::runtime_error(last_error_.toStdString());
    }
}

//--------------------------------------------------------------------------------------------------
int SQLReconciliationRepository::create(const Reconciliation & reconciliation) {
    QSqlQuery query(db_);
    query.prepare(
        QString("INSERT INTO %1(account_id, beginning_balance, beginning_date,"
                " ending_balance, ending_date) VALUES(:account, :begin_balance, :begin_date,"
                " :end_balance, :end_date);")
            .arg(table_name_));
    query.bindValue(":account", reconciliation.account().id());
    query.bindValue(":begin_balance", reconciliation.beginningBalance().scaled());
    query.bindValue(":begin_date", reconciliation.beginningDate().toString("yyyy-MM-dd"));
    query.bindValue(":end_balance", reconciliation.endingBalance().scaled());
    query.bindValue(":end_date", reconciliation.endingDate().toString("yyyy-MM-dd"));

    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return -1;
    }

    return query.lastInsertId().toInt();
}

//--------------------------------------------------------------------------------------------------
Reconciliation SQLReconciliationRepository::getReconciliation(int id) {
    QSqlQuery query(db_);
    query.prepare(QString("SELECT %1.*, date(%1.beginning_date, 'unixepoch') AS begin_date, "
                          "date(%1.ending_date, 'unixepoch') AS end_date FROM %1 "
                          "WHERE id=:id;")
                      .arg(table_name_));
    query.bindValue(":id", id);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    } else if (not query.first()) {
        last_error_ = QObject::tr("No reconciliation found for ID %1").arg(id);
    } else {
        return toReconciliation(query.record());
    }
    return Reconciliation();
}

//--------------------------------------------------------------------------------------------------
std::vector<Reconciliation>
SQLReconciliationRepository::getReconciliations(const Account & account) {
    std::vector<Reconciliation> reconciliations;
    QSqlQuery query(db_);
    query.prepare(QString("SELECT %1.*, date(%1.beginning_date, 'unixepoch') AS begin_date, "
                          "date(%1.ending_date, 'unixepoch') AS end_date FROM %1 "
                          "WHERE account_id=:id;")
                      .arg(table_name_));
    query.bindValue(":id", account.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    while (query.next()) {
        reconciliations.push_back(toReconciliation(query.record()));
    }
    return reconciliations;
}

//--------------------------------------------------------------------------------------------------
QString SQLReconciliationRepository::lastError() const {
    return last_error_;
}

//--------------------------------------------------------------------------------------------------
bool SQLReconciliationRepository::remove(const Reconciliation & reconciliation) {
    QSqlQuery query(db_);
    query.prepare(QString("DELETE FROM %1 WHERE id=:id;").arg(table_name_));
    query.bindValue(":id", reconciliation.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLReconciliationRepository::update(const Reconciliation & reconciliation) {
    QSqlQuery query(db_);
    query.prepare(QString("UPDATE %1 SET account_id=:account, beginning_balance=:begin_balance, "
                          " beginning_date=strftime('%s', :begin_date), "
                          " ending_balance=:end_balance, ending_date=strftime('%s', :end_date) "
                          " WHERE id=:id;")
                      .arg(table_name_));
    query.bindValue(":account", reconciliation.account().id());
    query.bindValue(":begin_balance", reconciliation.beginningBalance().scaled());
    query.bindValue(":begin_date", reconciliation.beginningDate().toString("yyyy-MM-dd"));
    query.bindValue(":end_balance", reconciliation.endingBalance().scaled());
    query.bindValue(":end_date", reconciliation.endingDate().toString("yyyy-MM-dd"));
    query.bindValue(":id", reconciliation.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
Reconciliation SQLReconciliationRepository::toReconciliation(const QSqlRecord & record) {
    auto account = accounts_->getAccount(record.value("account_id").toInt());

    Reconciliation reconciliation(record.value("id").toInt());
    reconciliation.setAccount(account);
    reconciliation.setBeginningBalance(
        Money(record.value("beginning_balance").toInt(), account.currency()));
    reconciliation.setBeginningDate(
        QDate::fromString(record.value("beginning_date").toString(), "yyyy-MM-dd"));
    reconciliation.setEndingBalance(
        Money(record.value("ending_balance").toInt(), account.currency()));
    reconciliation.setEndingDate(
        QDate::fromString(record.value("ending_date").toString(), "yyyy-MM-dd"));

    return reconciliation;
}

} // ledger namespace
} // ub namespace
