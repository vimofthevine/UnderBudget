#pragma once

// Qt include(s)
#include <QString>

// UnderBudget include(s)
#include "Currency.hpp"
#include "Money.hpp"

namespace ub {
namespace ledger {

/**
 * An asset account corresponds to the real-world location of immediately available funds, such as
 * a bank account or cash on hand.
 *
 * The current balance of any given asset account is fully known, and the sum of all asset accounts
 * represents the user's net worth.
 *
 * @ingroup ledger
 */
class AssetAccount {
public:
    /**
     * Initializes an account with default values.
     */
    AssetAccount();

    /**
     * Initializes an account with the given ID.
     *
     * @param[in] id Account ID
     */
    explicit AssetAccount(int id);

    /**
     * Returns the account number for this account.
     *
     * @return Account number
     */
    QString accountNumber() const;

    /**
     * Returns the currency type of this account.
     *
     * @return Currency type of this account
     */
    Currency currency() const;

    /**
     * Returns the current balance of this account.
     *
     * @return Current balance
     */
    Money currentBalance() const;

    /**
     * Returns the ID of this account.
     *
     * @return Account ID
     */
    int id() const;

    /**
     * Returns the financial institution at which this account resides.
     *
     * @return Financial institution
     */
    QString financialInstitution() const;

    /**
     * Returns the name of this account.
     *
     * @return Account name
     */
    QString name() const;

    /**
     * Returns the reconciled balance of this account.
     *
     * @return Reconciled balance
     */
    Money reconciledBalance() const;

    /**
     * Updates the account number for this account.
     *
     * @param[in] number New account number
     * @return @c true if successful
     */
    void setAccountNumber(const QString & number);

    /**
     * Updates the currenty type of this account.
     *
     * @param[in] currency New currency type
     */
    void setCurrency(const Currency & currency);

    /**
     * Updates the current balance of this account.
     *
     * @param[in] balance New current balance
     * @throw std::invalid_argument if the balance currency does not match the
     *        account currency
     */
    void setCurrentBalance(const Money & balance);

    /**
     * Updates the financial institution of this account.
     *
     * @param[in] institution New financial institution
     * @return @c true if successful
     */
    void setFinancialInstitution(const QString & institution);

    /**
     * Updates the name of this account.
     *
     * @param[in] name New account name
     * @return @c true if successful
     */
    void setName(const QString & name);

    /**
     * Updates the reconciled balance of this account.
     *
     * @param[in] balance New reconciled balance
     * @throw std::invalid_argument if the balance currency does not match the
     *        account currency
     */
    void setReconciledBalance(const Money & balance);

private:
    /** Name of this account */
    QString account_name_;
    /** Account number */
    QString account_number_;
    /** Currency type */
    Currency currency_;
    /** Current balance */
    Money current_balance_;
    /** Account ID */
    int id_;
    /** Financial institution */
    QString institution_;
    /** Reconciled balance */
    Money reconciled_balance_;
};

} // ledger namespace
} // ub namespace
