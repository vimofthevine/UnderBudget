/*
 * Copyright 2016 Kyle Treubig
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
#include <vector>

// Qt include(s)
#include <QString>

// UnderBudget include(s)
#include "Account.hpp"

namespace ub {
namespace ledger {

/**
 * Repository of accounts.
 *
 * @ingroup ledger
 */
class AccountRepository {
public:
    // Virtual destructor
    virtual ~AccountRepository() {}

    /**
     * Creates a new account as a child of the specified parent account.
     *
     * @param[in] account Account to be created
     * @param[in] parent  Account under which to create the new account
     * @return ID assigned to the newly created account
     */
    virtual int64_t create(const Account & account, const Account & parent) = 0;

    /**
     * Retrieves an account from the repository with the given ID.
     *
     * If no account exists for the given ID, an account with a -1 ID will be returned.
     *
     * @param[in] id Account ID to be retrieved
     * @return Account for the given ID
     */
    virtual Account getAccount(int64_t id) = 0;

    /**
     * Retrieves all leaf accounts.
     *
     * @return List of all leaf accounts
     */
    virtual std::vector<Account> getLeafAccounts() = 0;

    /**
     * Retrieves the root account from the repository.
     *
     * @return Root account
     * @throw std::invalid_argument if no root account exists
     */
    virtual Account getRoot() = 0;

    /**
     * Returns the message of the last error to have occurred.
     *
     * @return Last error message
     */
    virtual QString lastError() const = 0;

    /**
     * Moves the given account to the specified parent account.
     *
     * @param[in] account Account to be moved
     * @param[in] parent  New parent account
     * @return @c true if successful
     */
    virtual bool move(const Account & account, const Account & parent) = 0;

    /**
     * Removes the given account from the repository.
     *
     * The ID is the only parameter required to match the existing entry.
     *
     * @param account Account to be removed
     * @return @c true if successful
     */
    virtual bool remove(const Account & account) = 0;

    /**
     * Updates the given account in the repository.
     *
     * The ID must match an existing entry. All other parameters will be updated.
     *
     * @param account Account to be updated
     * @return @c true if successful
     */
    virtual bool update(const Account & account) = 0;
};

} // ledger namespace
} // ub namespace
