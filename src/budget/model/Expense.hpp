#pragma once

// Qt include(s)
#include <QDate>
#include <QString>

// UnderBudget include(s)
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
    Expense(int id);

    // Copy constructor
    Expense(const Expense & orig) = default;

    // Assignment operator
    Expense & operator=(const Expense & orig) = default;

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
    int id() const;

    /**
     * Returns the recurrence of this expense.
     *
     * @return Expense recurrence
     */
    Recurrence recurrence() const;

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
    int id_;
    /** Budgeted expense recurrence */
    Recurrence recurrence_;
};

} // budget namespace
} // ub namespace
