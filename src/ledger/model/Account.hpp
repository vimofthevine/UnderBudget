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
#include <vector>

// Qt include(s)
#include <QString>

// UnderBudget include(s)
#include "Currency.hpp"

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
    explicit Account(int64_t id);

    // Copy constructor
    Account(const Account & orig) = default;

    // Assignment operator
    Account & operator=(const Account & orig) = default;

    /**
     * Returns whether the account has been archived.
     *
     * @return @c true if the account has been archived
     */
    bool archived() const;

    /**
     * Returns the IDs of the children of this account.
     *
     * @return Children account IDs
     */
    std::vector<int64_t> children() const;

    /**
     * Returns the currency of this account.
     *
     * @return Currency of this account
     */
    Currency currency() const;

    /**
     * Returns the external ID for this currency.
     *
     * @return External ID
     */
    QString externalId() const;

    /**
     * Returns the ID of this account.
     *
     * @return Account ID
     */
    int64_t id() const;

    /**
     * Returns the name of this account.
     *
     * @return Account name
     */
    QString name() const;

    /**
     * Returns the ID of the parent to this account.
     *
     * @return Parent account ID
     */
    int64_t parent() const;

    /**
     * Updates the archive state of this account.
     *
     * @param[in] archive @c true if archived
     */
    void setArchived(bool archived);

    /**
     * Updates the children of this account.
     *
     * @param[in] children New children IDs
     */
    void setChildren(const std::vector<int64_t> & children);

    /**
     * Updates the currency of this account.
     *
     * @param[in] currency New currency
     */
    void setCurrency(const Currency & currency);

    /**
     * Sets the external ID of this currency.
     *
     * @param[in] id External ID
     */
    void setExternalId(const QString & id);

    /**
     * Updates the name of this account.
     *
     * @param[in] name New account name
     */
    void setName(const QString & name);

    /**
     * Updates the parent ID of this account.
     *
     * @param[in] id New parent ID
     */
    void setParent(int64_t id);

private:
    /** Name of this account */
    QString account_name_;
    /** Is archived */
    bool archived_;
    /** Child IDs */
    std::vector<int64_t> children_;
    /** Currency */
    Currency currency_;
    /** External ID */
    QString ext_id_;
    /** Account ID */
    int64_t id_;
    /** Parent ID */
    int64_t parent_;
};

} // namespace ledger
} // namespace ub
