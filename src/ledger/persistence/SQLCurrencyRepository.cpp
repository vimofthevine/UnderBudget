/*
 * Copyright 2016 Kyle Treubig
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

// Qt include(s)
#include <QtCore>
#include <QtSql>

// UnderBudget include(s)
#include <ledger/model/Currency.hpp>
#include "SQLCurrencyRepository.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
QString SQLCurrencyRepository::table_name_ = "currency";

//--------------------------------------------------------------------------------------------------
SQLCurrencyRepository::SQLCurrencyRepository(QSqlDatabase & db) : db_(db) {
    QSqlQuery query(db_);
    bool success =
        query.exec("CREATE TABLE IF NOT EXISTS " + table_name_ + "("
                                                                 "id INTEGER PRIMARY KEY, "
                                                                 "code VARCHAR);");

    if (not success) {
        last_error_ = query.lastError().text();
        throw std::runtime_error(last_error_.toStdString());
    }

    query.exec("SELECT id FROM " + table_name_ + " WHERE id=1;");
    if (not query.first() && not query.isValid()) {
        query.prepare("INSERT INTO " + table_name_ + "(id, code) VALUES(:id, :code);");
        query.bindValue(":id", 1);
        query.bindValue(":code", QLocale().currencySymbol(QLocale::CurrencyIsoCode));
        if (not query.exec()) {
            last_error_ = query.lastError().text();
            throw std::runtime_error(last_error_.toStdString());
        }
    }
}

//--------------------------------------------------------------------------------------------------
int64_t SQLCurrencyRepository::create(const Currency & currency) {
    {
        // First check if a non-default entry exists for the currency code
        QSqlQuery query(db_);
        query.prepare("SELECT id FROM " + table_name_ + " WHERE code=:code;");
        query.bindValue(":code", currency.code());
        if (query.exec() && query.first()) {
            QSqlRecord record = query.record();
            int64_t id = record.value("id").value<int64_t>();
            if (id > 1) {
                return id;
            }
        }
    }

    {
        QSqlQuery query(db_);
        query.prepare("INSERT INTO " + table_name_ + "(code) VALUES(:code);");
        query.bindValue(":code", currency.code());

        if (query.exec()) {
            return query.lastInsertId().value<int64_t>();
        }

        last_error_ = query.lastError().text();
    }

    return -1;
}

//--------------------------------------------------------------------------------------------------
Currency SQLCurrencyRepository::getCurrency(int64_t id) {
    QSqlQuery query(db_);
    query.prepare("SELECT * FROM " + table_name_ + " WHERE id=:id;");
    query.bindValue(":id", QVariant::fromValue(id));
    if (query.exec() && query.first()) {
        QSqlRecord record = query.record();
        return Currency(record.value("id").value<int64_t>(), record.value("code").toString());
    }
    last_error_ = query.lastError().text();
    throw std::invalid_argument(last_error_.toStdString());
}

//--------------------------------------------------------------------------------------------------
QString SQLCurrencyRepository::lastError() {
    return last_error_;
}

//--------------------------------------------------------------------------------------------------
bool SQLCurrencyRepository::remove(const Currency & currency) {
    QSqlQuery query(db_);
    query.prepare("DELETE FROM " + table_name_ + " WHERE id=:id;");
    query.bindValue(":id", QVariant::fromValue(currency.id()));
    if (query.exec()) {
        return true;
    }
    last_error_ = query.lastError().text();
    return false;
}

//--------------------------------------------------------------------------------------------------
bool SQLCurrencyRepository::update(const Currency & currency) {
    if (currency.id() != 1) {
        throw std::invalid_argument("Cannot update non-default currency");
    }
    QSqlQuery query(db_);
    query.prepare("UPDATE " + table_name_ + " SET code=:code WHERE id=1;");
    query.bindValue(":code", currency.code());
    if (query.exec()) {
        return true;
    }
    last_error_ = query.lastError().text();
    return false;
}

} // ledger namespace
} // ub namespace
