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
#include <vector>

// Qt include(s)
#include <QDate>
#include <QtCharts>

// UnderBudget include(s)
#include <budget/model/Impact.hpp>
#include <ledger/model/Money.hpp>

namespace ub {
namespace budget {
// Forward declaration(s)
class Recurrence;
} // budget namespace

namespace ledger {
// Forward declaration(s)
class TransactionRepository;
} // ledger namespace

namespace report {

class Compiler {
public:
    /**
     * Initializes the compiler.
     */
    Compiler() = default;

    /**
     * Returns the beginning historical balance.
     *
     * @return Beginning historical balance
     */
    ledger::Money beginningBalance() const;

    /**
     * Populates the given X/Y series with the budgeted balances from the compiled data.
     *
     * @param series X/Y series
     */
    void budgetedBalances(QXYSeries * series);

    /**
     * Compiles the report data for the given date range.
     *
     * If no transaction repository has been set yet, no historical balances will be produced.
     *
     * @param start      Beginning date for the report
     * @param stop       Ending date for the report
     * @param recurrence Recurrence through the date range for which to produce data
     * @param impacts    Budgeted balance impacts for the given date range
     */
    void compile(const QDate & start, const QDate & stop, const budget::Recurrence & recurrence,
                 const std::vector<budget::Impact> & impacts);

    /**
     * Returns the ending historical balance.
     *
     * @return Ending historical balance
     */
    ledger::Money endingBalance() const;

    /**
     * Populates the given X/Y series with the historical balances from the compiled data.
     *
     * @param series X/Y series
     */
    void historicalBalances(QXYSeries * series);

    /**
     * Updates the transaction repository to be used by the report compiler.
     *
     * @param transactions Transaction reository
     */
    void setRepository(std::shared_ptr<ledger::TransactionRepository> transactions);

private:
    /** Transaction repository */
    std::shared_ptr<ledger::TransactionRepository> transactions_;
    /** Budgeted balance impacts by date */
    std::map<QDate, std::vector<budget::Impact>> impacts_by_date_;
    /** Historical balance by date */
    std::map<QDate, ledger::Money> balance_by_date_;
    /** Expense totals by envelope */
    std::map<int64_t, ledger::Money> expenses_by_envelope_;
    /** Beginning date of the compiled report data */
    QDate beginning_date_;
    /** Ending date of the compiled report data */
    QDate ending_date_;
};

} // report namespace
} // ub namespace
