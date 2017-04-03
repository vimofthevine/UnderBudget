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

// Qt include(s)
#include <QDate>
#include <QDebug>
#include <QString>

// UnderBudget include(s)
#include <ledger/model/Money.hpp>
#include "Budget.hpp"
#include "Expense.hpp"
#include "Impact.hpp"
#include "Income.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
void Budget::add(const Expense & expense) {
    // Start the day before the expense's beginning date so it could be the first next occurrence
    auto date = expense.beginningDate().addDays(-1);
    date = expense.recurrence().nextOccurrence(date);
    if (not date.isValid()) {
        // Not a recurring expense, only care about the beginning date of the non-recurring expenses
        date = expense.beginningDate();
        if ((date >= beginning_date_) and (date <= ending_date_)) {
            net_decrease_ += expense.amount();
            impacts_.push_back(impact(date, expense));
        }
    } else {
        auto end = expense.endingDate();
        while ((not end.isValid() or (date <= end)) and (date <= ending_date_)) {
            if (date >= beginning_date_) {
                net_decrease_ += expense.amount();
                impacts_.push_back(impact(date, expense));
            }
            date = expense.recurrence().nextOccurrence(date);
        }
    }
}

//--------------------------------------------------------------------------------------------------
void Budget::add(const Income & income) {
    // Start the day before the income's beginning date so it could be the first next occurrence
    auto date = income.beginningDate().addDays(-1);
    date = income.recurrence().nextOccurrence(date);
    if (not date.isValid()) {
        // Not a recurring income, only care about the beginning date of the non-recurring incomes
        date = income.beginningDate();
        if ((date >= beginning_date_) and (date <= ending_date_)) {
            net_increase_ += income.amount();
            impacts_.push_back(impact(date, income));
        }
    } else {
        auto end = income.endingDate();
        while ((not end.isValid() or (date <= end)) and (date <= ending_date_)) {
            if (date >= beginning_date_) {
                net_increase_ += income.amount();
                impacts_.push_back(impact(date, income));
            }
            date = income.recurrence().nextOccurrence(date);
        }
    }
}

//--------------------------------------------------------------------------------------------------
Impact Budget::impact(const QDate & date, const Expense & expense) {
    Impact impact;
    impact.setAmount(expense.amount());
    impact.setCategory(expense.envelope().name());
    impact.setCategoryID(expense.envelope().id());
    impact.setDate(date);
    impact.setDescription(expense.description());
    impact.setType(Impact::Expense);
    return impact;
}

//--------------------------------------------------------------------------------------------------
Impact Budget::impact(const QDate & date, const Income & income) {
    Impact impact;
    impact.setAmount(income.amount());
    impact.setCategory(income.account().name());
    impact.setCategoryID(income.account().id());
    impact.setDate(date);
    impact.setDescription(income.description());
    impact.setType(Impact::Income);
    return impact;
}

//--------------------------------------------------------------------------------------------------
std::vector<Impact> Budget::impacts() const {
    return impacts_;
}

//--------------------------------------------------------------------------------------------------
ledger::Money Budget::netChange() const {
    return (net_increase_ - net_decrease_);
}

//--------------------------------------------------------------------------------------------------
ledger::Money Budget::netDecrease() const {
    return net_decrease_;
}

//--------------------------------------------------------------------------------------------------
ledger::Money Budget::netIncrease() const {
    return net_increase_;
}

//--------------------------------------------------------------------------------------------------
void Budget::reset(const QDate & start, const QDate & end) {
    beginning_date_ = start;
    ending_date_ = end;
    impacts_.clear();
    net_increase_ = ledger::Money();
    net_decrease_ = ledger::Money();
}

} // budget namespace
} // ub namespace
