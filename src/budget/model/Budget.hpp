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
#include <vector>

// Qt include(s)
#include <QDate>

// UnderBudget include(s)
#include <ledger/model/Money.hpp>
#include "Impact.hpp"

namespace ub {
namespace budget {

// Forward declaration(s)
class Expense;
class Income;

/**
 * Model of the effective budget over a particular period of time.
 *
 * @ingroup budget
 */
class Budget {
public:
    /**
     * Initializes a budget with default values.
     */
    Budget() = default;

    /**
     * Returns the individual budgeted balance impacts.
     *
     * @return Budgeted balance impacts
     */
    std::vector<Impact> impacts() const;

    /**
     * Returns the amount of net change that is budgeted to occur.
     *
     * @return Budgeted net change
     */
    ledger::Money netChange() const;

    /**
     * Returns the amount of net decrease that is budgeted to occur.
     *
     * @return Budgeted net decrease
     */
    ledger::Money netDecrease() const;

    /**
     * Returns the amount of net increase that is budgeted to occur.
     *
     * @return Budgeted net increase
     */
    ledger::Money netIncrease() const;

public slots:
    /**
     * Adds the given budgeted expense to the budget.
     *
     * If the expense's applicability dates fall within the range of this budget,
     * the expense's impact is determined based on the recurrence of the expense
     * within the range of this budget.
     *
     * @param expense Expense to be added to the budget
     */
    void add(const Expense & expense);

    /**
     * Adds the given budgeted income to the budget.
     *
     * If the income's applicability dates fall within the range of this budget,
     * the income's impact is determined based on the recurrence of the income
     * within the range of this budget.
     *
     * @param income Income to be added to the budget
     */
    void add(const Income & income);

    /**
     * Resets the budget for the given beginning and ending dates.
     *
     * @param start Beginning date, inclusive
     * @param stop  Ending date, inclusive
     */
    void reset(const QDate & start, const QDate & end);

private:
    /** Beginning date */
    QDate beginning_date_;
    /** Ending date */
    QDate ending_date_;
    /** Budget impacts */
    std::vector<Impact> impacts_;
    /** Net increase amount */
    ledger::Money net_increase_;
    /** Net decrease amount */
    ledger::Money net_decrease_;

    /**
     * Creates an impact of the given expense on the given date.
     *
     * @param date    Date of impact
     * @param expense Expense
     * @return Impact of the expense on the given date
     */
    Impact impact(const QDate & date, const Expense & expense);

    /**
     * Creates an impact of the given income on the given date.
     *
     * @param date   Date of impact
     * @param income Income
     * @return Impact of the income on the given date
     */
    Impact impact(const QDate & date, const Income & income);
};

} // budget namespace
} // ub namespace
