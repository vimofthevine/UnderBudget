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
#include <set>
#include <vector>

// Qt include(s)
#include <QDate>
#include <QtCharts>

// UnderBudget include(s)
#include <budget/model/Impact.hpp>
#include <budget/model/Recurrence.hpp>
#include <ledger/model/Currency.hpp>
#include <ledger/model/Money.hpp>
#include <ledger/model/TransactionRepository.hpp>
#include "Compiler.hpp"

namespace ub {
namespace report {

//--------------------------------------------------------------------------------------------------
ledger::Money Compiler::beginningBalance() const {
    ledger::Money balance;
    auto start = balance_by_date_.cbegin();
    if (start != balance_by_date_.cend()) {
        balance = start->second;
    }
    return balance;
}

//--------------------------------------------------------------------------------------------------
void Compiler::budgetedBalances(QXYSeries * series) {
    auto balance = beginningBalance();
    // Add an initial point at the historical balance to get a nicer-looking graph
    if (not balance.isZero()) {
        QDateTime time(beginning_date_.addDays(-1));
        series->append(time.toMSecsSinceEpoch(), balance.amount());
    }
    for (auto & entry : impacts_by_date_) {
        for (auto & impact : entry.second) {
            if (impact.type() == budget::Impact::Expense) {
                balance -= impact.amount();
            } else {
                balance += impact.amount();
            }
        }
        QDateTime time(entry.first);
        series->append(time.toMSecsSinceEpoch(), balance.amount());
    }
}

//--------------------------------------------------------------------------------------------------
void Compiler::compile(const QDate & start, const QDate & stop,
                       const budget::Recurrence & recurrence,
                       const std::vector<budget::Impact> & impacts) {
    beginning_date_ = start;
    ending_date_ = stop;
    impacts_by_date_.clear();
    balance_by_date_.clear();
    expenses_by_envelope_.clear();

    std::set<QDate> dates;
    // Add an initial point with the balance as of the day prior to the beginning date
    auto prev = start.addDays(-1);
    if (transactions_) {
        balance_by_date_[prev] = transactions_->getBalance(prev, ledger::Currency());
    }
    // Get all the historical balances
    auto date = start;
    while (date <= stop) {
        dates.insert(date);
        if (transactions_) {
            balance_by_date_[date] = transactions_->getBalance(date, ledger::Currency());
        }
        prev = date;
        date = recurrence.nextOccurrence(date);
    }
    // If we didn't happen to land on the ending date, add the ending date specifically
    if (prev < stop) {
        dates.insert(stop);
        if (transactions_) {
            balance_by_date_[stop] = transactions_->getBalance(stop, ledger::Currency());
        }
    }
    for (auto & impact : impacts) {
        date = start;
        bool added = false;
        for (auto & next : dates) {
            if (impact.date() < next) {
                impacts_by_date_[date].push_back(impact);
                added = true;
                break;
            }
            date = next;
        }
        if (not added) {
            impacts_by_date_[date].push_back(impact);
        }
    }
}

//--------------------------------------------------------------------------------------------------
ledger::Money Compiler::endingBalance() const {
    ledger::Money balance;
    auto start = balance_by_date_.crbegin();
    if (start != balance_by_date_.crend()) {
        balance = start->second;
    }
    return balance;
}

//--------------------------------------------------------------------------------------------------
void Compiler::historicalBalances(QXYSeries * series) {
    for (auto & entry : balance_by_date_) {
        QDateTime time(entry.first);
        series->append(time.toMSecsSinceEpoch(), entry.second.amount());
    }
}

//--------------------------------------------------------------------------------------------------
void Compiler::setRepository(std::shared_ptr<ledger::TransactionRepository> transactions) {
    transactions_ = transactions;
}

} // report namespace
} // ub namespace
