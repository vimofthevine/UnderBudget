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
#include <cstdint>

// Qt include(s)
#include <QDate>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/Money.hpp>
#include "Income.hpp"
#include "Recurrence.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
Income::Income() : Income(-1) {}

//--------------------------------------------------------------------------------------------------
Income::Income(int64_t id) : id_(id) {}

//--------------------------------------------------------------------------------------------------
ledger::Account Income::account() const {
    return account_;
}

//--------------------------------------------------------------------------------------------------
ledger::Money Income::amount() const {
    return amount_;
}

//--------------------------------------------------------------------------------------------------
QDate Income::beginningDate() const {
    return beginning_date_;
}

//--------------------------------------------------------------------------------------------------
QString Income::description() const {
    return description_;
}

//--------------------------------------------------------------------------------------------------
QDate Income::endingDate() const {
    return ending_date_;
}

//--------------------------------------------------------------------------------------------------
int64_t Income::id() const {
    return id_;
}

//--------------------------------------------------------------------------------------------------
Recurrence Income::recurrence() const {
    return recurrence_;
}

//--------------------------------------------------------------------------------------------------
void Income::setAccount(const ledger::Account & account) {
    account_ = account;
}

//--------------------------------------------------------------------------------------------------
void Income::setAmount(const ledger::Money & amount) {
    amount_ = amount;
}

//--------------------------------------------------------------------------------------------------
void Income::setBeginningDate(const QDate & date) {
    beginning_date_ = date;
}

//--------------------------------------------------------------------------------------------------
void Income::setDescription(const QString & description) {
    description_ = description;
}

//--------------------------------------------------------------------------------------------------
void Income::setEndingDate(const QDate & date) {
    ending_date_ = date;
}

//--------------------------------------------------------------------------------------------------
void Income::setRecurrence(const Recurrence & recurrence) {
    recurrence_ = recurrence;
}

} // budget namespace
} // ub namespace
