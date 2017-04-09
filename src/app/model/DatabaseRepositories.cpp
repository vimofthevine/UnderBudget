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
#include <memory>

// Qt include(s)
#include <QtCore>
#include <QtSql>

// UnderBudget include(s)
#include <budget/persistence/SQLExpenseRepository.hpp>
#include <budget/persistence/SQLIncomeRepository.hpp>
#include <ledger/persistence/SQLAccountRepository.hpp>
#include <ledger/persistence/SQLCurrencyRepository.hpp>
#include <ledger/persistence/SQLEnvelopeRepository.hpp>
#include <ledger/persistence/SQLReconciliationRepository.hpp>
#include <ledger/persistence/SQLTransactionRepository.hpp>
#include "DatabaseRepositories.hpp"

namespace ub {

//--------------------------------------------------------------------------------------------------
DatabaseRepositories::DatabaseRepositories(const QString & loc) : location_(loc) {
    db_ = QSqlDatabase::addDatabase("QSQLITE", loc);
    db_.setDatabaseName(loc);
    if (not db_.open()) {
        qCritical() << "Unable to open database file:" << loc << "error:" << db_.lastError();
    } else {
        qDebug() << "Opened database file:" << loc;
        currencies_.reset(new ledger::SQLCurrencyRepository(db_));
        accounts_.reset(new ledger::SQLAccountRepository(db_));
        envelopes_.reset(new ledger::SQLEnvelopeRepository(db_));
        reconciliations_.reset(new ledger::SQLReconciliationRepository(db_, accounts_));
        transactions_.reset(new ledger::SQLTransactionRepository(db_, accounts_, envelopes_));
        expenses_.reset(new budget::SQLExpenseRepository(db_, accounts_, envelopes_));
        incomes_.reset(new budget::SQLIncomeRepository(db_, accounts_));
    }
}

//--------------------------------------------------------------------------------------------------
DatabaseRepositories::~DatabaseRepositories() {
    if (db_.isOpen()) {
        db_.close();
    }
}

//--------------------------------------------------------------------------------------------------
bool DatabaseRepositories::isOpen() const {
    return db_.isOpen();
}

//--------------------------------------------------------------------------------------------------
QString DatabaseRepositories::location() const {
    return location_;
}

//--------------------------------------------------------------------------------------------------
std::shared_ptr<ledger::AccountRepository> DatabaseRepositories::accounts() const {
    return accounts_;
}

//--------------------------------------------------------------------------------------------------
std::shared_ptr<ledger::CurrencyRepository> DatabaseRepositories::currencies() const {
    return currencies_;
}

//--------------------------------------------------------------------------------------------------
std::shared_ptr<ledger::EnvelopeRepository> DatabaseRepositories::envelopes() const {
    return envelopes_;
}

//--------------------------------------------------------------------------------------------------
std::shared_ptr<ledger::ReconciliationRepository> DatabaseRepositories::reconciliations() const {
    return reconciliations_;
}

//--------------------------------------------------------------------------------------------------
std::shared_ptr<ledger::TransactionRepository> DatabaseRepositories::transactions() const {
    return transactions_;
}

//--------------------------------------------------------------------------------------------------
std::shared_ptr<budget::ExpenseRepository> DatabaseRepositories::expenses() const {
    return expenses_;
}

//--------------------------------------------------------------------------------------------------
std::shared_ptr<budget::IncomeRepository> DatabaseRepositories::incomes() const {
    return incomes_;
}

} // ub namespace
