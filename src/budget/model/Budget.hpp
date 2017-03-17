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
