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
#include <QDate>
#include <QString>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/model/Money.hpp>
#include "Recurrence.hpp"

namespace ub {
namespace budget {

/**
 * A budgeted expense.
 *
 * @ingroup budget
 */
class Expense {
public:
    /**
     * Initializes an expense with default values.
     */
    Expense();

    /**
     * Initializes an expense with the given ID.
     *
     * @param id Expense ID
     */
    Expense(int64_t id);

    // Copy constructor
    Expense(const Expense & orig) = default;

    // Assignment operator
    Expense & operator=(const Expense & orig) = default;

    /**
     * Returns the account to which this expense applies.
     *
     * @return Account to which this expense applies
     */
    ledger::Account account() const;

    /**
     * Returns the amount of the budgeted expense.
     *
     * @return Budgeted expense amount
     */
    ledger::Money amount() const;

    /**
     * Returns the beginning date for which this expense applies.
     *
     * @return Beginning date for which this expense applies
     */
    QDate beginningDate() const;

    /**
     * Returns the description of this budgeted expense.
     *
     * @return Description of this budgeted expense
     */
    QString description() const;

    /**
     * Returns the ending date for which this expense applies.
     *
     * @return Ending date for which this expense applies
     */
    QDate endingDate() const;

    /**
     * Returns the envelope to which this expense applies.
     *
     * @return Envelope to which this expense applies
     */
    ledger::Envelope envelope() const;

    /**
     * Returns the ID of this expense.
     *
     * @return Expense ID
     */
    int64_t id() const;

    /**
     * Returns the recurrence of this expense.
     *
     * @return Expense recurrence
     */
    Recurrence recurrence() const;

    /**
     * Updates the account to which this expense applies.
     *
     * @param account New account
     */
    void setAccount(const ledger::Account & account);

    /**
     * Updates the amount of the budgeted expense.
     *
     * @param amount New amount
     */
    void setAmount(const ledger::Money & amount);

    /**
     * Updates the beginning date for which this expense applies.
     *
     * @param date New date
     */
    void setBeginningDate(const QDate & date);

    /**
     * Updates the description of this budgeted expense.
     *
     * @param description New description
     */
    void setDescription(const QString & description);

    /**
     * Updates the ending date for which this expense applies.
     *
     * @param date New date
     */
    void setEndingDate(const QDate & date);

    /**
     * Updates the envelope to which this expense applies.
     *
     * @param envelope New envelope
     */
    void setEnvelope(const ledger::Envelope & envelope);

    /**
     * Updates the recurrence of this expense.
     *
     * @param recurrence New recurrence
     */
    void setRecurrence(const Recurrence & recurrence);

private:
    /** Account */
    ledger::Account account_;
    /** Budgeted expense amount */
    ledger::Money amount_;
    /** Beginning date */
    QDate beginning_date_;
    /** Description */
    QString description_;
    /** Ending date */
    QDate ending_date_;
    /** Envelope */
    ledger::Envelope envelope_;
    /** Expense ID */
    int64_t id_;
    /** Budgeted expense recurrence */
    Recurrence recurrence_;
};

} // budget namespace
} // ub namespace
