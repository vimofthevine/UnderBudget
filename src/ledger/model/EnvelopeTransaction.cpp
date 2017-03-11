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
#include "Envelope.hpp"
#include "EnvelopeTransaction.hpp"
#include "Transaction.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
EnvelopeTransaction::EnvelopeTransaction() : id_(-1) {}

//--------------------------------------------------------------------------------------------------
EnvelopeTransaction::EnvelopeTransaction(int64_t id) : id_(id) {}

//--------------------------------------------------------------------------------------------------
Money EnvelopeTransaction::amount() const {
    return amount_;
}

//--------------------------------------------------------------------------------------------------
Money EnvelopeTransaction::balance() const {
    return balance_;
}

//--------------------------------------------------------------------------------------------------
Envelope EnvelopeTransaction::envelope() const {
    return envelope_;
}

//--------------------------------------------------------------------------------------------------
int64_t EnvelopeTransaction::id() const {
    return id_;
}

//--------------------------------------------------------------------------------------------------
QString EnvelopeTransaction::memo() const {
    return memo_;
}

//--------------------------------------------------------------------------------------------------
void EnvelopeTransaction::setAmount(const Money & amount) {
    amount_ = amount;
}

//--------------------------------------------------------------------------------------------------
void EnvelopeTransaction::setBalance(const Money & balance) {
    balance_ = balance;
}

//--------------------------------------------------------------------------------------------------
void EnvelopeTransaction::setEnvelope(const Envelope & envelope) {
    envelope_ = envelope;
}

//--------------------------------------------------------------------------------------------------
void EnvelopeTransaction::setMemo(const QString & memo) {
    memo_ = memo;
}

//--------------------------------------------------------------------------------------------------
void EnvelopeTransaction::setTransaction(const Transaction & transaction) {
    transaction_ = transaction;
}

//--------------------------------------------------------------------------------------------------
Transaction EnvelopeTransaction::transaction() const {
    return transaction_;
}

} // ledger namespace
} // ub namespace
