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

#pragma once

// Qt include(s)
#include <QDate>

// UnderBudget include(s)
#include "Account.hpp"
#include "Money.hpp"

namespace ub {
namespace ledger {

/**
 * A reconciliation of a single account against a statement from the financial institution
 * for that account.
 *
 * @ingroup ledger
 */
class Reconciliation {
public:
    /**
     * Initializes a reconciliation with default values.
     */
    Reconciliation();

    /**
     * Initializes a reconciliation with the given ID.
     *
     * @param id Reconciliation ID
     */
    Reconciliation(int id);

    // Copy constructor
    Reconciliation(const Reconciliation & orig) = default;

    // Assignment operator
    Reconciliation & operator=(const Reconciliation & orig) = default;

    /**
     * Returns the account to which this reconciliation applies.
     *
     * @return Account to which this reconciliation applies
     */
    Account account() const;

    /**
     * Returns the beginning account balance.
     *
     * @return Beginning account balance
     */
    Money beginningBalance() const;

    /**
     * Returns the beginning date for which this reconciliation applies.
     *
     * @return Beginning date for which this reconciliation applies.
     */
    QDate beginningDate() const;

    /**
     * Returns the ending account balance.
     *
     * @return Ending account balance
     */
    Money endingBalance() const;

    /**
     * Returns the ending date for which this reconciliation applies.
     *
     * @return Ending date for which this reconciliation applies.
     */
    QDate endingDate() const;

    /**
     * Returns the ID of this reconciliation.
     *
     * @return Reconciliation ID
     */
    int id() const;

    /**
     * Updates the account to which this reconciliation applies.
     *
     * @param account New account
     */
    void setAccount(const Account & account);

    /**
     * Updates the beginning account balance.
     *
     * @param balance New beginning balance
     */
    void setBeginningBalance(const Money & balance);

    /**
     * Updates the beginning date for which this reconciliation applies.
     *
     * @param date New beginning date
     */
    void setBeginningDate(const QDate & date);

    /**
     * Updates the ending account balance.
     *
     * @param balance New ending balance
     */
    void setEndingBalance(const Money & balance);

    /**
     * Updates the ending date for which this reconciliation applies.
     *
     * @param date New ending date
     */
    void setEndingDate(const QDate & date);

private:
    /** Account */
    Account account_;
    /** Beginning account balance */
    Money beginning_balance_;
    /** Beginning date */
    QDate beginning_date_;
    /** Ending account balance */
    Money ending_balance_;
    /** Ending date */
    QDate ending_date_;
    /** Reconciliation ID */
    int id_;
};

} // ledger namespace
} // ub namespace
