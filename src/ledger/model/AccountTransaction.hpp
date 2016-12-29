#pragma once

// Qt include(s)
#include <QString>

namespace ub {
namespace ledger {

/**
 * A transaction against a single account, as part of a double-entry transaction.
 *
 * @ingroup ledger
 */
class AccountTransaction {
public:
    /**
     * Initializes a transaction with default values.
     */
    AccountTransaction();

    /**
     * Initializes a transaction with the given ID.
     *
     * @param[in] id Account transaction ID
     */
    explicit AccountTransaction(int id);

    // Copy constructor
    AccountTransaction(const AccountTransaction & orig) = default;

    // Assignment operator
    AccountTransaction & operator=(const AccountTransaction & orig) = default;

    /**
     * Returns the ID of the account associated with this transaction.
     *
     * @return Account ID
     */
    int account() const;

    /**
     * Returns the scaled amount of this transaction.
     *
     * @return Scaled transaction amount
     */
    int amount() const;

    /**
     * Returns the ID of this account transaction.
     *
     * @return Account transaction ID
     */
    int id() const;

    /**
     * Returns whether the transaction has been marked cleared.
     *
     * @return @c true if cleared
     */
    bool isCleared() const;

    /**
     * Returns the memo of this account transaction.
     *
     * @return Account transaction memo
     */
    QString memo() const;

    /**
     * Returns the ID of the associated reconciliation, if any.
     *
     * @return Reconciliation ID, or -1 if not associated with a reconciliation
     */
    int reconciliation() const;

    /**
     * Updates the account ID associated with this account transaction.
     *
     * @param[in] id New account ID
     */
    void setAccount(int id);

    /**
     * Updates the scaled amount of this transaction.
     *
     * @param[in] amount New scaled amount
     */
    void setAmount(int amount);

    /**
     * Updates whether the transaction has cleared.
     *
     * @param[in] cleared @c true if the transaction has cleared
     */
    void setCleared(bool cleared);

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
     * Updates the double-entry transaction ID associated with this account transaction.
     *
     * @param[in] id New transaction ID
     */
    void setTransaction(int id);

    /**
     * Returns the ID of the double-entry transaction.
     *
     * @return Transaction ID
     */
    int transaction() const;

private:
    /** Account ID */
    int account_;
    /** Scaled amount */
    int amount_;
    /** Cleared */
    bool cleared_;
    /** Account transaction ID */
    int id_;
    /** Memo */
    QString memo_;
    /** Reconciliation ID */
    int reconciliation_;
    /** Transaction ID */
    int transaction_;
};

} // ledger namespace
} // ub namespace
