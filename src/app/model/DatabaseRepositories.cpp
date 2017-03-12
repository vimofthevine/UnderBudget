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
        expenses_.reset(new budget::SQLExpenseRepository(db_, envelopes_));
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
