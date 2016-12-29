#pragma once

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
    virtual int create(const Account & account, const Account & parent) = 0;

    /**
     * Retrieves an account from the repository with the given ID.
     *
     * If no account exists for the given ID, an account with a -1 ID will be returned.
     *
     * @param[in] id Account ID to be retrieved
     * @return Account for the given ID
     */
    virtual Account getAccount(int id) = 0;

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
    virtual QString lastError() = 0;

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