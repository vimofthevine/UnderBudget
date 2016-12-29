#pragma once

// Standard include(s)
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
    explicit Transaction(int id);

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
    int id() const;

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
    int id_;
    /** Payee */
    QString payee_;
};

} // ledger namespace
} // ub namespace
