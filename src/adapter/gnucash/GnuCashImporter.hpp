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

#pragma once

// Standard include(s)
#include <map>
#include <memory>

// Qt include(s)
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QString>

// UnderBudget include(s)
#include <app/model/Repositories.hpp>
#include <ledger/model/Account.hpp>
#include <ledger/model/Envelope.hpp>

// Forward declaration(s)
class QSqlDatabase;

namespace ub {
namespace adapter {

/**
 * Importer of accounts and transactions from GnuCash
 */
class GnuCashImporter {
public:
    /**
     * Initializes the importer with the given application repositories
     *
     * @param repos Application repositories
     */
    GnuCashImporter(std::shared_ptr<Repositories> repos);

    /**
     * Imports from the specified GnuCash SQLite database location
     *
     * @param db        GnuCash SQLite database location
     * @return @c true if successful
     */
    bool importFromSqlite(const QString & db);

private:
    /** Application repositories */
    std::shared_ptr<Repositories> repos_;
    /** Imported currencies */
    std::map<QString, ledger::Currency> currencies_;
    /** Imported accounts */
    std::map<QString, ledger::Account> accounts_;
    /** Imported envelopes */
    std::map<QString, ledger::Envelope> envelopes_;

    /**
     * Recursively imports child accounts of the specified parent account
     *
     * @param parent_ext_id GnuCash ID of the parent account
     * @param db            GnuCash database
     * @return @c true if successful
     */
    bool importChildAccountsOf(const QString & parent_ext_id, QSqlDatabase & db);

    /**
     * Imports the transaction in the given record from the transactions table
     *
     * @param trn_record Transaction table record
     * @param db         GnuCash database
     * @return @c true if successful
     */
    bool importTransaction(QSqlRecord trn_record, QSqlDatabase & db);
};

} // namespace adapter
} // namespace ub
