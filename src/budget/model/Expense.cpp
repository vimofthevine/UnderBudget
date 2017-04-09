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
#include <ledger/model/Envelope.hpp>
#include <ledger/model/Money.hpp>
#include "Expense.hpp"
#include "Recurrence.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
Expense::Expense() : Expense(-1) {}

//--------------------------------------------------------------------------------------------------
Expense::Expense(int64_t id) : id_(id) {}

//--------------------------------------------------------------------------------------------------
ledger::Account Expense::account() const {
    return account_;
}

//--------------------------------------------------------------------------------------------------
ledger::Money Expense::amount() const {
    return amount_;
}

//--------------------------------------------------------------------------------------------------
QDate Expense::beginningDate() const {
    return beginning_date_;
}

//--------------------------------------------------------------------------------------------------
QString Expense::description() const {
    return description_;
}

//--------------------------------------------------------------------------------------------------
QDate Expense::endingDate() const {
    return ending_date_;
}

//--------------------------------------------------------------------------------------------------
ledger::Envelope Expense::envelope() const {
    return envelope_;
}

//--------------------------------------------------------------------------------------------------
int64_t Expense::id() const {
    return id_;
}

//--------------------------------------------------------------------------------------------------
Recurrence Expense::recurrence() const {
    return recurrence_;
}

//--------------------------------------------------------------------------------------------------
void Expense::setAccount(const ledger::Account & account) {
    account_ = account;
}

//--------------------------------------------------------------------------------------------------
void Expense::setAmount(const ledger::Money & amount) {
    amount_ = amount;
}

//--------------------------------------------------------------------------------------------------
void Expense::setBeginningDate(const QDate & date) {
    beginning_date_ = date;
}

//--------------------------------------------------------------------------------------------------
void Expense::setDescription(const QString & description) {
    description_ = description;
}

//--------------------------------------------------------------------------------------------------
void Expense::setEndingDate(const QDate & date) {
    ending_date_ = date;
}

//--------------------------------------------------------------------------------------------------
void Expense::setEnvelope(const ledger::Envelope & envelope) {
    envelope_ = envelope;
}

//--------------------------------------------------------------------------------------------------
void Expense::setRecurrence(const Recurrence & recurrence) {
    recurrence_ = recurrence;
}

} // budget namespace
} // ub namespace
