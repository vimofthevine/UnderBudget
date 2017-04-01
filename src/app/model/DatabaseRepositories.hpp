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
#include <memory>

// Qt include(s)
#include <QSqlDatabase>
#include <QString>

// UnderBudget include(s)
#include "Repositories.hpp"

namespace ub {

namespace budget {
// Forward declaration(s)
class ExpenseRepository;
class IncomeRepository;
class SQLExpenseRepository;
class SQLIncomeRepository;
}

namespace ledger {
// Forward declaration(s)
class AccountRepository;
class CurrencyRepository;
class EnvelopeRepository;
class ReconciliationRepository;
class TransactionRepository;
class SQLAccountRepository;
class SQLCurrencyRepository;
class SQLEnvelopeRepository;
class SQLReconciliationRepository;
class SQLTransactionRepository;
}

/**
 * Collection of SQL repositories acting on a shared database.
 */
class DatabaseRepositories : public Repositories {
public:
    /**
     * Initializes the database repositories for the specified database file.
     *
     * @param loc Database file location
     */
    DatabaseRepositories(const QString & loc);

    /**
     * Closes the database.
     */
    ~DatabaseRepositories();

    /**
     * Checks if the database was opened successfully.
     *
     * @return @c true if database was successfully opened
     */
    bool isOpen() const;

    QString location() const override;

    std::shared_ptr<ledger::AccountRepository> accounts() const override;

    std::shared_ptr<ledger::CurrencyRepository> currencies() const override;

    std::shared_ptr<ledger::EnvelopeRepository> envelopes() const override;

    std::shared_ptr<ledger::ReconciliationRepository> reconciliations() const override;

    std::shared_ptr<ledger::TransactionRepository> transactions() const override;

    std::shared_ptr<budget::ExpenseRepository> expenses() const override;

    std::shared_ptr<budget::IncomeRepository> incomes() const override;

private:
    /** Database location */
    QString location_;
    /** Database connection */
    QSqlDatabase db_;
    /** Account repository */
    std::shared_ptr<ledger::SQLAccountRepository> accounts_;
    /** Currency repository */
    std::shared_ptr<ledger::SQLCurrencyRepository> currencies_;
    /** Envelope repository */
    std::shared_ptr<ledger::SQLEnvelopeRepository> envelopes_;
    /** Reconciliation repository */
    std::shared_ptr<ledger::SQLReconciliationRepository> reconciliations_;
    /** Transaction repository */
    std::shared_ptr<ledger::SQLTransactionRepository> transactions_;
    /** Budgeted expense repository */
    std::shared_ptr<budget::SQLExpenseRepository> expenses_;
    /** Budgeted income repository */
    std::shared_ptr<budget::SQLIncomeRepository> incomes_;
};
} // ub namespace
