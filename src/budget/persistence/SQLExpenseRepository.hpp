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
