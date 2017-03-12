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

// Standard include(s)
#include <cstdint>

// Qt include(s)
#include <QDate>
#include <QString>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/Money.hpp>
#include "Recurrence.hpp"

namespace ub {
namespace budget {

/**
 * A budgeted income.
 *
 * @ingroup budget
 */
class Income {
public:
    /**
     * Initializes an income with default values.
     */
    Income();

    /**
     * Initializes an income with the given ID.
     *
     * @param id Income ID
     */
    Income(int64_t id);

    // Copy constructor
    Income(const Income & orig) = default;

    // Assignment operator
    Income & operator=(const Income & orig) = default;

    /**
     * Returns the account to which this income applies.
     *
     * @return Account to which this income applies
     */
    ledger::Account account() const;

    /**
     * Returns the amount of the budgeted income.
     *
     * @return Budgeted income amount
     */
    ledger::Money amount() const;

    /**
     * Returns the beginning date for which this income applies.
     *
     * @return Beginning date for which this income applies
     */
    QDate beginningDate() const;

    /**
     * Returns the description of this budgeted income.
     *
     * @return Description of this budgeted income
     */
    QString description() const;

    /**
     * Returns the ending date for which this income applies.
     *
     * @return Ending date for which this income applies
     */
    QDate endingDate() const;

    /**
     * Returns the ID of this income.
     *
     * @return Income ID
     */
    int64_t id() const;

    /**
     * Returns the recurrence of this income.
     *
     * @return Income recurrence
     */
    Recurrence recurrence() const;

    /**
     * Updates the account to which this income applies.
     *
     * @param account New account
     */
    void setAccount(const ledger::Account & account);

    /**
     * Updates the amount of the budgeted income.
     *
     * @param amount New amount
     */
    void setAmount(const ledger::Money & amount);

    /**
     * Updates the beginning date for which this income applies.
     *
     * @param date New date
     */
    void setBeginningDate(const QDate & date);

    /**
     * Updates the description of this budgeted income.
     *
     * @param description New description
     */
    void setDescription(const QString & description);

    /**
     * Updates the ending date for which this income applies.
     *
     * @param date New date
     */
    void setEndingDate(const QDate & date);

    /**
     * Updates the recurrence of this income.
     *
     * @param recurrence New recurrence
     */
    void setRecurrence(const Recurrence & recurrence);

private:
    /** Account */
    ledger::Account account_;
    /** Budgeted income amount */
    ledger::Money amount_;
    /** Beginning date */
    QDate beginning_date_;
    /** Description */
    QString description_;
    /** Ending date */
    QDate ending_date_;
    /** Income ID */
    int64_t id_;
    /** Budgeted income recurrence */
    Recurrence recurrence_;
};

} // budget namespace
} // ub namespace
