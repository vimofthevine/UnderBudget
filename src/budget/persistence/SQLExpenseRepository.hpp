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

#pragma once

// Standard include(s)
#include <cstdint>
#include <memory>
#include <vector>

// Qt include(s)
#include <QDate>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QString>

// UnderBudget include(s)
#include <budget/model/Expense.hpp>
#include <budget/model/ExpenseRepository.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/model/EnvelopeRepository.hpp>

namespace ub {
namespace budget {

/**
 * Expense repository implementation that uses an SQLite database.
 *
 * @ingroup budget
 */
class SQLExpenseRepository : public ExpenseRepository {
public:
    /**
     * Initializes the repository with the given database connection.
     *
     * This will set up the budget_expense table.
     *
     * @param db        SQL database connection
     * @param envelopes Envelope repository
     * @throw std::runtime_error if the database table could not be set up
     */
    SQLExpenseRepository(QSqlDatabase & db, std::shared_ptr<ledger::EnvelopeRepository> envelopes);

    int64_t create(const Expense & expense) override;

    Expense expense(int64_t id) override;

    std::vector<Expense> expenses(const ledger::Envelope & account) override;

    std::vector<Expense> expenses(const QDate & start, const QDate & stop) override;

    QString lastError() const override;

    bool remove(const Expense & expense) override;

    bool update(const Expense & expense) override;

private:
    /** Budgeted expense table name */
    static QString table_name_;

    /** Database connection */
    QSqlDatabase db_;
    /** Last error message */
    QString last_error_;
    /** Envelope repository */
    std::shared_ptr<ledger::EnvelopeRepository> envelopes_;

    /**
     * Creates a budgeted expense from the given SQL record.
     *
     * @param record SQL record
     * @return Budgeted expense
     */
    Expense toExpense(const QSqlRecord & record);
};

} // budget namespace
} // ub namespace
