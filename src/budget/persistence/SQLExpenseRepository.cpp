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
#include <stdexcept>
#include <vector>

// Qt include(s)
#include <QtCore>
#include <QtSql>

// UnderBudget include(s)
#include <budget/model/Expense.hpp>
#include <budget/model/Recurrence.hpp>
#include <ledger/model/AccountRepository.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/model/EnvelopeRepository.hpp>
#include <ledger/model/Money.hpp>
#include "SQLExpenseRepository.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
QString SQLExpenseRepository::table_name_ = "budget_expense";

//--------------------------------------------------------------------------------------------------
SQLExpenseRepository::SQLExpenseRepository(QSqlDatabase & db,
                                           std::shared_ptr<ledger::AccountRepository> accounts,
                                           std::shared_ptr<ledger::EnvelopeRepository> envelopes)
        : db_(db), accounts_(accounts), envelopes_(envelopes) {
    if (not db_.tables().contains("account")) {
        last_error_ = QObject::tr("Account table does not exist");
        throw std::runtime_error(last_error_.toStdString());
    }

    if (not db_.tables().contains("envelope")) {
        last_error_ = QObject::tr("Envelope table does not exist");
        throw std::runtime_error(last_error_.toStdString());
    }

    QSqlQuery query(db_);
    bool success =
        query.exec(QString("CREATE TABLE IF NOT EXISTS %1 ("
                           "id INTEGER PRIMARY KEY, "
                           "amount INTEGER NOT NULL, "
                           "description VARCHAR, "
                           "envelope_id INTEGER NOT NULL, "
                           "beginning_date DATE NOT NULL, "
                           "ending_date DATE, "
                           "day INTEGER DEFAULT 0, "
                           "month INTEGER DEFAULT 0, "
                           "periodicity INTEGER DEFAULT 0, "
                           "scope INTEGER DEFAULT 0, "
                           "week INTEGER DEFAULT 0, "
                           "FOREIGN KEY(envelope_id) REFERENCES envelope(id) ON DELETE RESTRICT);")
                       .arg(table_name_));
    if (not success) {
        last_error_ = query.lastError().text();
        throw std::runtime_error(last_error_.toStdString());
    }

    // Check if the account_id column exists
    bool has_account_id = false;
    query.exec(QString("PRAGMA table_info(%1)").arg(table_name_));
    while (query.next()) {
        if (query.record().value("name") == "account_id") {
            has_account_id = true;
        }
    }

    if (not has_account_id) {
        qDebug() << "Adding account_id column to expense table";
        success = query.exec(QString("ALTER TABLE %1 ADD COLUMN "
                                     "account_id INTEGER DEFAULT NULL "
                                     "REFERENCES account(id) ON DELETE RESTRICT;")
                                 .arg(table_name_));
        if (not success) {
            last_error_ = query.lastError().text();
            throw std::runtime_error(last_error_.toStdString());
        }
    }
}

//--------------------------------------------------------------------------------------------------
int64_t SQLExpenseRepository::create(const Expense & expense) {
    QSqlQuery query(db_);
    query.prepare(
        QString("INSERT INTO %1(amount, description, account_id, envelope_id, beginning_date, "
                " ending_date, day, month, periodicity, scope, week) "
                "VALUES(:amount, :description, :account, :envelope, strftime('%s', :start), "
                "strftime('%s', :stop), :day, :month, :periodicity, :scope, :week);")
            .arg(table_name_));
    query.bindValue(":amount", QVariant::fromValue(expense.amount().scaled()));
    query.bindValue(":description", expense.description());
    query.bindValue(":account", (expense.account().id() > 0)
                                    ? QVariant::fromValue(expense.account().id())
                                    : QVariant());
    query.bindValue(":envelope", QVariant::fromValue(expense.envelope().id()));
    query.bindValue(":start", expense.beginningDate().toString("yyyy-MM-dd"));
    query.bindValue(":stop", expense.endingDate().isValid()
                                 ? expense.endingDate().toString("yyyy-MM-dd")
                                 : QVariant(QDate()));
    auto rec = expense.recurrence();
    query.bindValue(":day", QVariant::fromValue(rec.day()));
    query.bindValue(":month", QVariant::fromValue(rec.month()));
    query.bindValue(":periodicity", QVariant::fromValue(rec.periodicity()));
    query.bindValue(":scope", QVariant::fromValue(static_cast<int32_t>(rec.scope())));
    query.bindValue(":week", QVariant::fromValue(rec.week()));

    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return -1;
    }

    return query.lastInsertId().value<int64_t>();
}

//--------------------------------------------------------------------------------------------------
Expense SQLExpenseRepository::expense(int64_t id) {
    QSqlQuery query(db_);
    query.prepare(QString("SELECT *, date(beginning_date, 'unixepoch') AS start, "
                          "date(ending_date, 'unixepoch') AS stop from %1 WHERE id=:id;")
                      .arg(table_name_));
    query.bindValue(":id", QVariant::fromValue(id));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    } else if (not query.first()) {
        last_error_ = QObject::tr("No budgeted expense found for ID %1").arg(id);
    } else {
        return toExpense(query.record());
    }
    return Expense();
}

//--------------------------------------------------------------------------------------------------
std::vector<Expense> SQLExpenseRepository::expenses(const ledger::Envelope & envelope) {
    std::vector<Expense> expenses;
    QSqlQuery query(db_);
    query.prepare(QString("SELECT *, date(beginning_date, 'unixepoch') AS start, "
                          "date(ending_date, 'unixepoch') AS stop from %1 WHERE "
                          "envelope_id=:id;")
                      .arg(table_name_));
    query.bindValue(":id", QVariant::fromValue(envelope.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    while (query.next()) {
        expenses.push_back(toExpense(query.record()));
    }
    return expenses;
}

//--------------------------------------------------------------------------------------------------
std::vector<Expense> SQLExpenseRepository::expenses(const QDate & start, const QDate & stop) {
    std::vector<Expense> expenses;
    QSqlQuery query(db_);
    query.prepare(QString("SELECT *, date(beginning_date, 'unixepoch') AS start, "
                          "date(ending_date, 'unixepoch') AS stop from %1 WHERE "
                          "beginning_date<=strftime('%s', :end) AND "
                          "(ending_date IS NULL OR ending_date>=strftime('%s', :begin));")
                      .arg(table_name_));
    query.bindValue(":begin", start.toString("yyyy-MM-dd"));
    query.bindValue(":end", stop.toString("yyyy-MM-dd"));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    while (query.next()) {
        expenses.push_back(toExpense(query.record()));
    }
    return expenses;
}

//--------------------------------------------------------------------------------------------------
QString SQLExpenseRepository::lastError() const {
    return last_error_;
}

//--------------------------------------------------------------------------------------------------
bool SQLExpenseRepository::remove(const Expense & expense) {
    QSqlQuery query(db_);
    query.prepare(QString("DELETE FROM %1 WHERE id=:id;").arg(table_name_));
    query.bindValue(":id", QVariant::fromValue(expense.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLExpenseRepository::update(const Expense & expense) {
    QSqlQuery query(db_);
    query.prepare(
        QString("UPDATE %1 SET amount=:amount, description=:descrip, account_id=:account, "
                "envelope_id=:envelope, beginning_date=strftime('%s', :start), "
                "ending_date=strftime('%s', :stop), day=:day, month=:month, "
                "periodicity=:periodicity, scope=:scope, week=:week WHERE id=:id;")
            .arg(table_name_));
    query.bindValue(":amount", QVariant::fromValue(expense.amount().scaled()));
    query.bindValue(":descrip", expense.description());
    query.bindValue(":account", (expense.account().id() > 0)
                                    ? QVariant::fromValue(expense.account().id())
                                    : QVariant());
    query.bindValue(":envelope", QVariant::fromValue(expense.envelope().id()));
    query.bindValue(":start", expense.beginningDate().toString("yyyy-MM-dd"));
    query.bindValue(":stop", expense.endingDate().toString("yyyy-MM-dd"));
    auto rec = expense.recurrence();
    query.bindValue(":day", QVariant::fromValue(rec.day()));
    query.bindValue(":month", QVariant::fromValue(rec.month()));
    query.bindValue(":periodicity", QVariant::fromValue(rec.periodicity()));
    query.bindValue(":scope", QVariant::fromValue(static_cast<int32_t>(rec.scope())));
    query.bindValue(":week", QVariant::fromValue(rec.week()));
    query.bindValue(":id", QVariant::fromValue(expense.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
Expense SQLExpenseRepository::toExpense(const QSqlRecord & record) {
    auto envelope = envelopes_->getEnvelope(record.value("envelope_id").value<int64_t>());
    ledger::Account account;
    if (not record.value("account_id").isNull()) {
        account = accounts_->getAccount(record.value("account_id").value<int64_t>());
    }

    Expense expense(record.value("id").value<int64_t>());
    expense.setAmount(ledger::Money(record.value("amount").value<int64_t>(), envelope.currency()));
    expense.setDescription(record.value("description").toString());
    expense.setAccount(account);
    expense.setEnvelope(envelope);
    expense.setBeginningDate(QDate::fromString(record.value("start").toString(), "yyyy-MM-dd"));
    expense.setEndingDate(QDate::fromString(record.value("stop").toString(), "yyyy-MM-dd"));

    Recurrence recurrence;
    recurrence.setDay(record.value("day").value<int32_t>());
    recurrence.setMonth(record.value("month").value<int32_t>());
    recurrence.setPeriodicity(record.value("periodicity").value<int32_t>());
    recurrence.setScope(static_cast<Recurrence::ScopeType>(record.value("scope").value<int32_t>()));
    recurrence.setWeek(record.value("week").value<int32_t>());
    expense.setRecurrence(recurrence);

    return expense;
}

} // budget namespace
} // ub namespace
