#pragma once

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
    explicit EnvelopeTransaction(int id);

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
    int id() const;

    /**
     * Returns whether the transaction has been marked cleared.
     *
     * @return @c true if cleared
     */
    bool isCleared() const;

    /**
     * Returns the memo of this envelope transaction.
     *
     * @return Envelope transaction memo
     */
    QString memo() const;

    /**
     * Returns the ID of the associated reconciliation, if any.
     *
     * @return Reconciliation ID, or -1 if not associated with a reconciliation
     */
    int reconciliation() const;

    /**
     * Updates the amount of this transaction.
     *
     * @param[in] amount New amount
     */
    void setAmount(const Money & amount);

    /**
     * Updates whether the transaction has cleared.
     *
     * @param[in] cleared @c true if the transaction has cleared
     */
    void setCleared(bool cleared);

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
     * Updates the reconciliation ID associated with this transaction.
     *
     * @param[in] id New reconciliation ID
     */
    void setReconciliation(int id);

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
    /** Cleared */
    bool cleared_;
    /** Envelope transaction ID */
    int id_;
    /** Memo */
    QString memo_;
    /** Reconciliation ID */
    int reconciliation_;
    /** Transaction */
    Transaction transaction_;
};

} // ledger namespace
} // ub namespace
