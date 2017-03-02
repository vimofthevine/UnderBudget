#pragma once

// Qt include(s)
#include <QString>

// UnderBudget include(s)
#include "Account.hpp"
#include "Money.hpp"
#include "Transaction.hpp"

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
     * Returns the account against which this transaction applies.
     *
     * @return Account against which this transaction applies
     */
    Account account() const;

    /**
     * Returns the amount of this transaction.
     *
     * @return Transaction amount
     */
    Money amount() const;

    /**
     * Returns the account balance after applying this transaction.
     *
     * @return Account balance
     */
    Money balance() const;

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
     * Updates the account to which this transaction applies.
     *
     * @param[in] account New account
     */
    void setAccount(const Account & account);

    /**
     * Updates the account balance after this transaction is applied.
     *
     * @param[in] balance New balance
     */
    void setBalance(const Money & balance);

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
     * Updates the double-entry transaction associated with this account transaction.
     *
     * @param[in] transaction New transaction
     */
    void setTransaction(const Transaction & transaction);

    /**
     * Returns the double-entry transaction to which this account tranaction belongs.
     *
     * @return Double-entry transaction
     */
    Transaction transaction() const;

private:
    /** Account */
    Account account_;
    /** Transaction amount */
    Money amount_;
    /** Account balance after applying this transaction */
    Money balance_;
    /** Cleared */
    bool cleared_;
    /** Account transaction ID */
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
