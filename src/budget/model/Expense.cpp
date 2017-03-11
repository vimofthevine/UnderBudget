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
