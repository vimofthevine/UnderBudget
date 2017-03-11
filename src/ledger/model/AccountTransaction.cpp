/*
 * Copyright 2016 Kyle Treubig
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
#include <QtCore>

// UnderBudget include(s)
#include "Account.hpp"
#include "AccountTransaction.hpp"
#include "Transaction.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountTransaction::AccountTransaction() : AccountTransaction(-1) {}

//--------------------------------------------------------------------------------------------------
AccountTransaction::AccountTransaction(int64_t id)
        : cleared_(false), id_(id), reconciliation_(-1) {}

//--------------------------------------------------------------------------------------------------
Account AccountTransaction::account() const {
    return account_;
}

//--------------------------------------------------------------------------------------------------
Money AccountTransaction::balance() const {
    return balance_;
}

//--------------------------------------------------------------------------------------------------
Money AccountTransaction::amount() const {
    return amount_;
}

//--------------------------------------------------------------------------------------------------
int64_t AccountTransaction::id() const {
    return id_;
}

//--------------------------------------------------------------------------------------------------
bool AccountTransaction::isCleared() const {
    return cleared_;
}

//--------------------------------------------------------------------------------------------------
QString AccountTransaction::memo() const {
    return memo_;
}

//--------------------------------------------------------------------------------------------------
int64_t AccountTransaction::reconciliation() const {
    return reconciliation_;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setAccount(const Account & account) {
    account_ = account;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setAmount(const Money & amount) {
    amount_ = amount;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setBalance(const Money & balance) {
    balance_ = balance;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setCleared(bool cleared) {
    cleared_ = cleared;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setMemo(const QString & memo) {
    memo_ = memo;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setReconciliation(int64_t id) {
    reconciliation_ = id;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setTransaction(const Transaction & transaction) {
    transaction_ = transaction;
}

//--------------------------------------------------------------------------------------------------
Transaction AccountTransaction::transaction() const {
    return transaction_;
}

} // ledger namespace
} // ub namespace
