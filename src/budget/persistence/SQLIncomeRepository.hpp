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
#include <budget/model/Income.hpp>
#include <budget/model/IncomeRepository.hpp>
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountRepository.hpp>

namespace ub {
namespace budget {

/**
 * Income repository implementation that uses an SQLite database.
 *
 * @ingroup budget
 */
class SQLIncomeRepository : public IncomeRepository {
public:
    /**
     * Initializes the repository with the given database connection.
     *
     * This will set up the budget_income table.
     *
     * @param db       SQL database connection
     * @param accounts Account repository
     * @throw std::runtime_error if the database table could not be set up
     */
    SQLIncomeRepository(QSqlDatabase & db, std::shared_ptr<ledger::AccountRepository> accounts);

    int64_t create(const Income & income) override;

    Income income(int64_t id) override;

    std::vector<Income> incomes(const ledger::Account & account) override;

    std::vector<Income> incomes(const QDate & start, const QDate & stop) override;

    QString lastError() const override;

    bool remove(const Income & income) override;

    bool update(const Income & income) override;

private:
    /** Budgeted income table name */
    static QString table_name_;

    /** Database connection */
    QSqlDatabase db_;
    /** Last error message */
    QString last_error_;
    /** Account repository */
    std::shared_ptr<ledger::AccountRepository> accounts_;

    /**
     * Creates a budgeted income from the given SQL record.
     *
     * @param record SQL record
     * @return Budgeted income
     */
    Income toIncome(const QSqlRecord & record);
};

} // budget namespace
} // ub namespace
