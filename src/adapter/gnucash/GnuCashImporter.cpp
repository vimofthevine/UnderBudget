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
#include <map>
#include <memory>

// Qt include(s)
#include <QtDebug>
#include <QtSql>

// UnderBudget include(s)
#include <app/model/Repositories.hpp>
#include <ledger/model/Account.hpp>
#include <ledger/model/Currency.hpp>
#include <ledger/model/JournalEntry.hpp>
#include <ledger/model/Transaction.hpp>
#include "GnuCashImporter.hpp"

namespace ub {
namespace adapter {

//--------------------------------------------------------------------------------------------------
GnuCashImporter::GnuCashImporter(std::shared_ptr<Repositories> repos) : repos_(repos) {}

//--------------------------------------------------------------------------------------------------
bool GnuCashImporter::importFromSqlite(const QString & db) {
    qDebug() << "Importing" << db;
    QSqlDatabase gnucash = QSqlDatabase::addDatabase("QSQLITE", db);
    gnucash.setDatabaseName(db);
    if (not gnucash.open()) {
        qCritical() << "Unable to open GnuCash database file:" << db
                    << "error:" << gnucash.lastError();
        return false;
    }

    // Import currencies
    currencies_.clear();
    {
        auto repo = repos_->currencies();
        QSqlQuery query(gnucash);
        QSqlRecord record;
        if (not query.exec("SELECT * FROM commodities WHERE namespace=\"CURRENCY\";")) {
            qWarning() << query.lastError().text();
            return false;
        }
        while (query.next()) {
            record = query.record();
            ledger::Currency currency(0, record.value("mnemonic").toString(),
                                      record.value("guid").toString());
            auto id = repo->create(currency);
            currencies_[currency.externalId()] = repo->getCurrency(id);
        }
    }

    // Import accounts
    accounts_.clear();
    {
        auto repo = repos_->accounts();
        QSqlQuery query(gnucash);
        query.exec("SELECT * FROM accounts WHERE name=\"Root Account\";");
        if (query.first()) {
            auto record = query.record();
            auto ext_id = record.value("guid").toString();
            accounts_[ext_id] = repo->getAccount(1);
            if (not importChildAccountsOf(ext_id, gnucash)) {
                return false;
            }
        } else {
            return false;
        }
    }

    return true;
}

//--------------------------------------------------------------------------------------------------
bool GnuCashImporter::importChildAccountsOf(const QString & parent_ext_id, QSqlDatabase & db) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM accounts WHERE parent_guid=:id;");
    query.bindValue(":id", parent_ext_id);
    if (not query.exec()) {
        qWarning() << query.lastError().text();
        return false;
    }
    while (query.next()) {
        auto record = query.record();
        auto ext_id = record.value("guid").toString();
        ledger::Account account;
        account.setCurrency(currencies_[record.value("commodity_guid").toString()]);
        account.setExternalId(ext_id);
        account.setName(record.value("name").toString());
        account.setParent(accounts_[parent_ext_id].id());
        auto id = repos_->accounts()->create(account, accounts_[parent_ext_id]);
        accounts_[ext_id] = repos_->accounts()->getAccount(id);
        if (not importChildAccountsOf(ext_id, db)) {
            return false;
        }
    }
    return true;
}

} // namespace adapter
} // namespace ub
