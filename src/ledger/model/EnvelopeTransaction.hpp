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

#pragma once

// Standard include(s)
#include <cstdint>

// Qt include(s)
#include <QString>

// UnderBudget include(s)
#include "Envelope.hpp"
#include "Money.hpp"
#include "Transaction.hpp"

namespace ub {
namespace ledger {

/**
 * A transaction against a single envelope, as part of a double-entry transaction.
 *
 * @ingroup ledger
 */
class EnvelopeTransaction {
public:
    /**
     * Initializes a transaction with default values.
     */
    EnvelopeTransaction();

    /**
     * Initializes a transaction with the given ID.
     *
     * @param[in] id Envelope transaction ID
     */
    explicit EnvelopeTransaction(int64_t id);

    // Copy constructor
    EnvelopeTransaction(const EnvelopeTransaction & orig) = default;

    // Assignment operator
    EnvelopeTransaction & operator=(const EnvelopeTransaction & orig) = default;

    /**
     * Returns the amount of this transaction.
     *
     * @return Transaction amount
     */
    Money amount() const;

    /**
     * Returns the envelope balance after applying this transaction.
     *
     * @return Envelope balance
     */
    Money balance() const;

    /**
     * Returns the envelope against which this transaction applies.
     *
     * @return Envelope against which this transaction applies
     */
    Envelope envelope() const;

    /**
     * Returns the ID of this envelope transaction.
     *
     * @return Envelope transaction ID
     */
    int64_t id() const;

    /**
     * Returns the memo of this envelope transaction.
     *
     * @return Envelope transaction memo
     */
    QString memo() const;

    /**
     * Updates the amount of this transaction.
     *
     * @param[in] amount New amount
     */
    void setAmount(const Money & amount);

    /**
     * Updates the envelope balance after this transaction is applied.
     *
     * @param[in] balance New balance
     */
    void setBalance(const Money & balance);

    /**
     * Updates the envelope to which this transaction applies.
     *
     * @param[in] envelope New envelope
     */
    void setEnvelope(const Envelope & envelope);

    /**
     * Updates the memo of this transaction.
     *
     * @param[in] memo New memo
     */
    void setMemo(const QString & memo);

    /**
     * Updates the double-entry transaction associated with this envelope transaction.
     *
     * @param[in] transaction New transaction
     */
    void setTransaction(const Transaction & transaction);

    /**
     * Returns the double-entry transaction to which this envelope tranaction belongs.
     *
     * @return Double-entry transaction
     */
    Transaction transaction() const;

private:
    /** Envelope */
    Envelope envelope_;
    /** Transaction amount */
    Money amount_;
    /** Envelope balance after applying this transaction */
    Money balance_;
    /** Envelope transaction ID */
    int64_t id_;
    /** Memo */
    QString memo_;
    /** Transaction */
    Transaction transaction_;
};

} // ledger namespace
} // ub namespace
