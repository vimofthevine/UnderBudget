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
#include "Account.hpp"
#include "Money.hpp"
#include "Reconciliation.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
Reconciliation::Reconciliation() : Reconciliation(-1) {}

//--------------------------------------------------------------------------------------------------
Reconciliation::Reconciliation(int64_t id) : id_(id) {}

//--------------------------------------------------------------------------------------------------
Account Reconciliation::account() const {
    return account_;
}

//--------------------------------------------------------------------------------------------------
Money Reconciliation::beginningBalance() const {
    return beginning_balance_;
}

//--------------------------------------------------------------------------------------------------
QDate Reconciliation::beginningDate() const {
    return beginning_date_;
}

//--------------------------------------------------------------------------------------------------
Money Reconciliation::endingBalance() const {
    return ending_balance_;
}

//--------------------------------------------------------------------------------------------------
QDate Reconciliation::endingDate() const {
    return ending_date_;
}

//--------------------------------------------------------------------------------------------------
int64_t Reconciliation::id() const {
    return id_;
}

//--------------------------------------------------------------------------------------------------
void Reconciliation::setAccount(const Account & account) {
    account_ = account;
}

//--------------------------------------------------------------------------------------------------
void Reconciliation::setBeginningBalance(const Money & balance) {
    beginning_balance_ = balance;
}

//--------------------------------------------------------------------------------------------------
void Reconciliation::setBeginningDate(const QDate & date) {
    beginning_date_ = date;
}

//--------------------------------------------------------------------------------------------------
void Reconciliation::setEndingBalance(const Money & balance) {
    ending_balance_ = balance;
}

//--------------------------------------------------------------------------------------------------
void Reconciliation::setEndingDate(const QDate & date) {
    ending_date_ = date;
}

} // ledger namespace
} // ub namespace
