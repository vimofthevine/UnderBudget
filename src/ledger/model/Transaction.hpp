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
#include <vector>

// Qt include(s)
#include <QDate>
#include <QString>

namespace ub {
namespace ledger {

/**
 * A double-entry transaction containing transactions against accounts and envelopes.
 *
 * @ingroup ledger
 */
class Transaction {
public:
    /**
     * Initializes a transaction with default values.
     */
    Transaction();

    /**
     * Initializes a transaction with the given ID.
     *
     * @param[in] id Transaction ID
     */
    explicit Transaction(int64_t id);

    // Copy constructor
    Transaction(const Transaction & orig) = default;

    // Assignment operator
    Transaction & operator=(const Transaction & orig) = default;

    /**
     * Returns the date on which this transaction occurred.
     *
     * @return Transaction date
     */
    QDate date() const;

    /**
     * Returns the ID of this transaction.
     *
     * @return Transaction ID
     */
    int64_t id() const;

    /**
     * Returns the payee to which this transction applies.
     *
     * @return Payee
     */
    QString payee() const;

    /**
     * Updates the date on which this transaction occurred.
     *
     * @param[in] date New date
     */
    void setDate(const QDate & date);

    /**
     * Updates the payee to which this transaction applies.
     *
     * @param[in] payee New payee
     */
    void setPayee(const QString & payee);

private:
    /** Transaction Date */
    QDate date_;
    /** Transaction ID */
    int64_t id_;
    /** Payee */
    QString payee_;
};

} // ledger namespace
} // ub namespace
