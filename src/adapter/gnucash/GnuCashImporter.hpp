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
#include <atomic>
#include <map>
#include <memory>

// Qt include(s)
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QString>
#include <QThread>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/Currency.hpp>
#include <ledger/persistence/SQLAccountRepository.hpp>
#include <ledger/persistence/SQLCurrencyRepository.hpp>
#include <ledger/persistence/SQLTransactionRepository.hpp>

// Forward declaration(s)
class QSqlDatabase;

namespace ub {

namespace ledger {
class SQLAccountRepository;
class SQLCurrencyRepository;
class SQLTransactionRepository;
} // namespace ledger

namespace adapter {

/**
 * Importer of accounts and transactions from GnuCash
 */
class GnuCashImporter : public QThread {
    Q_OBJECT

public:
    /**
     * Initializes the importer with the given application repositories
     *
     * @param name   Database connection name
     * @param parent Parent object
     */
    GnuCashImporter(const QString & name, QObject * parent = 0);

    /**
     * Closes all database connections.
     */
    ~GnuCashImporter();

    /**
     * Imports from the specified GnuCash SQLite database location
     *
     * @param loc GnuCash SQLite database location
     */
    void importFromSqlite(const QString & loc);

signals:
    /**
     * Emitted to display a status message.
     *
     * @param message Status message
     */
    void message(const QString & message);

    /**
     * Emitted to display an error message.
     *
     * @param message Error message
     */
    void error(const QString & message);

    /**
     * Emitted when the import has completed.
     *
     * @param success @c true if successful
     */
    void finished(bool success);

    /**
     * Emitted to update progress of the import.
     *
     * @param value Current progress value
     * @param max   Maximum progress value
     */
    void progress(int value, int max);

protected:
    /**
     * Performs the import operation.
     */
    void run() override;

private:
    /** Thread terminated */
    std::atomic<bool> terminated_{false};

    /** Application SQL database */
    QSqlDatabase app_db_;
    /** Account repository */
    std::shared_ptr<ledger::SQLAccountRepository> account_repo_;
    /** Currency repository */
    std::shared_ptr<ledger::SQLCurrencyRepository> currency_repo_;
    /** Transaction repository */
    std::shared_ptr<ledger::SQLTransactionRepository> transaction_repo_;

    /** GnuCash SQL database */
    QSqlDatabase gnucash_db_;
    /** Imported currencies */
    std::map<QString, ledger::Currency> currencies_;
    /** Imported accounts */
    std::map<QString, ledger::Account> accounts_;
    /** Number of imported transactions */
    int num_transactions_{0};

    /**
     * Recursively imports child accounts of the specified parent account
     *
     * @param parent_ext_id GnuCash ID of the parent account
     * @throw @c std::runtime_error if error occurs
     */
    void importChildAccountsOf(const QString & parent_ext_id);

    /**
     * Imports the transaction in the given record from the transactions table
     *
     * @param trn_record Transaction table record
     * @throw @c std::runtime_error if error occurs
     */
    void importTransaction(QSqlRecord trn_record);
};

} // namespace adapter
} // namespace ub
