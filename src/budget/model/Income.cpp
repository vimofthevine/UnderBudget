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
