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
#include <budget/model/Income.hpp>
#include <budget/model/Recurrence.hpp>
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountRepository.hpp>
#include <ledger/model/Money.hpp>
#include "SQLIncomeRepository.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
QString SQLIncomeRepository::table_name_ = "budget_income";

//--------------------------------------------------------------------------------------------------
SQLIncomeRepository::SQLIncomeRepository(QSqlDatabase & db,
                                           std::shared_ptr<ledger::AccountRepository> accounts)
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
                           "amount INTEGER NOT NULL, "
                           "description VARCHAR, "
                           "beginning_date DATE NOT NULL, "
                           "ending_date DATE, "
                           "day INTEGER DEFAULT 0, "
                           "month INTEGER DEFAULT 0, "
                           "periodicity INTEGER DEFAULT 0, "
                           "scope INTEGER DEFAULT 0, "
                           "week INTEGER DEFAULT 0, "
                           "FOREIGN KEY(account_id) REFERENCES account(id) ON DELETE RESTRICT);")
                       .arg(table_name_));
    if (not success) {
        last_error_ = query.lastError().text();
        throw std::runtime_error(last_error_.toStdString());
    }
}

//--------------------------------------------------------------------------------------------------
int64_t SQLIncomeRepository::create(const Income & income) {
    QSqlQuery query(db_);
    query.prepare(
        QString("INSERT INTO %1(account_id, amount, description, beginning_date, ending_date, "
                " day, month, periodicity, scope, week) VALUES(:account, :amount, :description, "
                "strftime('%s', :start), strftime('%s', :stop), :day, :month, "
                ":periodicity, :scope, :week);")
            .arg(table_name_));
    query.bindValue(":amount", QVariant::fromValue(income.amount().scaled()));
    query.bindValue(":description", income.description());
    query.bindValue(":account", QVariant::fromValue(income.account().id()));
    query.bindValue(":start", income.beginningDate().toString("yyyy-MM-dd"));
    query.bindValue(":stop", income.endingDate().isValid()
                                 ? income.endingDate().toString("yyyy-MM-dd")
                                 : QVariant(QDate()));
    auto rec = income.recurrence();
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
Income SQLIncomeRepository::income(int64_t id) {
    QSqlQuery query(db_);
    query.prepare(QString("SELECT *, date(beginning_date, 'unixepoch') AS start, "
                          "date(ending_date, 'unixepoch') AS stop from %1 WHERE id=:id;")
                  .arg(table_name_));
    query.bindValue(":id", QVariant::fromValue(id));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    } else if (not query.first()) {
        last_error_ = QObject::tr("No budgeted income found for ID %1").arg(id);
    } else {
        return toIncome(query.record());
    }
    return Income();
}

//--------------------------------------------------------------------------------------------------
std::vector<Income> SQLIncomeRepository::incomes(const ledger::Account & account) {
    std::vector<Income> incomes;
    QSqlQuery query(db_);
    query.prepare(QString("SELECT *, date(beginning_date, 'unixepoch') AS start, "
                          "date(ending_date, 'unixepoch') AS stop from %1 WHERE "
                          "account_id=:id;").arg(table_name_));
    query.bindValue(":id", QVariant::fromValue(account.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    while (query.next()) {
        incomes.push_back(toIncome(query.record()));
    }
    return incomes;
}

//--------------------------------------------------------------------------------------------------
std::vector<Income> SQLIncomeRepository::incomes(const QDate & start, const QDate & stop) {
    std::vector<Income> incomes;
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
        incomes.push_back(toIncome(query.record()));
    }
    return incomes;
}

//--------------------------------------------------------------------------------------------------
QString SQLIncomeRepository::lastError() const {
    return last_error_;
}

//--------------------------------------------------------------------------------------------------
bool SQLIncomeRepository::remove(const Income & income) {
    QSqlQuery query(db_);
    query.prepare(QString("DELETE FROM %1 WHERE id=:id;").arg(table_name_));
    query.bindValue(":id", QVariant::fromValue(income.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLIncomeRepository::update(const Income & income) {
    QSqlQuery query(db_);
    query.prepare(
        QString("UPDATE %1 SET amount=:amount, description=:descrip, account_id=:account, "
                "beginning_date=strftime('%s', :start), ending_date=strftime('%s', :stop), "
                "day=:day, month=:month, periodicity=:periodicity, scope=:scope, week=:week "
                "WHERE id=:id;")
            .arg(table_name_));
    query.bindValue(":amount", QVariant::fromValue(income.amount().scaled()));
    query.bindValue(":descrip", income.description());
    query.bindValue(":account", QVariant::fromValue(income.account().id()));
    query.bindValue(":start", income.beginningDate().toString("yyyy-MM-dd"));
    query.bindValue(":stop", income.endingDate().toString("yyyy-MM-dd"));
    auto rec = income.recurrence();
    query.bindValue(":day", QVariant::fromValue(rec.day()));
    query.bindValue(":month", QVariant::fromValue(rec.month()));
    query.bindValue(":periodicity", QVariant::fromValue(rec.periodicity()));
    query.bindValue(":scope", QVariant::fromValue(static_cast<int32_t>(rec.scope())));
    query.bindValue(":week", QVariant::fromValue(rec.week()));
    query.bindValue(":id", QVariant::fromValue(income.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
Income SQLIncomeRepository::toIncome(const QSqlRecord & record) {
    auto account = accounts_->getAccount(record.value("account_id").value<int64_t>());

    Income income(record.value("id").value<int64_t>());
    income.setAccount(account);
    income.setAmount(ledger::Money(record.value("amount").value<int64_t>(), account.currency()));
    income.setDescription(record.value("description").toString());
    income.setBeginningDate(
        QDate::fromString(record.value("start").toString(), "yyyy-MM-dd"));
    income.setEndingDate(QDate::fromString(record.value("stop").toString(), "yyyy-MM-dd"));

    Recurrence recurrence;
    recurrence.setDay(record.value("day").value<int32_t>());
    recurrence.setMonth(record.value("month").value<int32_t>());
    recurrence.setPeriodicity(record.value("periodicity").value<int32_t>());
    recurrence.setScope(static_cast<Recurrence::ScopeType>(record.value("scope").value<int32_t>()));
    recurrence.setWeek(record.value("week").value<int32_t>());
    income.setRecurrence(recurrence);

    return income;
}

} // budget namespace
} // ub namespace
