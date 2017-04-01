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
