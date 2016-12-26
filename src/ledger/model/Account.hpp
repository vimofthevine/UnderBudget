#pragma once

// Qt include(s)
#include <QString>

// UnderBudget include(s)
#include "Currency.hpp"
#include "Money.hpp"

namespace ub {
namespace ledger {

/**
 * An account corresponds to cash on hand or a real-world bank, credit card, or loan account.
 *
 * The current balance of any given account is fully known, and the sum of all accounts
 * represents the user's net worth.
 *
 * @ingroup ledger
 */
class Account {
public:
    /**
     * Initializes an account with default values.
     */
    Account();

    /**
     * Initializes an account with the given ID.
     *
     * @param[in] id Account ID
     */
    explicit Account(int id);

    // Copy constructor
    Account(const Account & orig) = default;

    // Assignment operator
    Account & operator=(const Account & orig) = default;

    /**
     * Returns the category of this account.
     *
     * @return Category of this account
     */
    QString category() const;

    /**
     * Returns the currency of this account.
     *
     * @return Currency of this account
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
     * Updates the category of this account.
     *
     * @param[in] category New category
     */
    void setCategory(const QString & category);

    /**
     * Updates the currency of this account.
     *
     * @param[in] currency New currency
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
     * Updates the name of this account.
     *
     * @param[in] name New account name
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
    /** Category */
    QString category_;
    /** Currency */
    Currency currency_;
    /** Current balance */
    Money current_balance_;
    /** Account ID */
    int id_;
    /** Reconciled balance */
    Money reconciled_balance_;
};

} // ledger namespace
} // ub namespace
