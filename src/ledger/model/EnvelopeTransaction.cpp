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
