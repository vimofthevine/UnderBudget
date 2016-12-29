#pragma once

// Standard include(s)
#include <map>
#include <memory>

// Qt include(s)
#include <QSqlDatabase>
#include <QString>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountRepository.hpp>
#include <ledger/model/CurrencyRepository.hpp>

namespace ub {
namespace ledger {

/**
 * Account repository implementation that uses an SQLite database.
 *
 * @ingroup ledger
 */
class SQLAccountRepository : public AccountRepository {
public:
    /**
     * Initializes the repository with the given database connection.
     *
     * This will set up the account table and add an entry for the root
     * account if one does not exist.
     *
     * @param[in] db         SQL database connection
     * @param[in] currencies Currency repository
     * @throw std::runtime_error if the database table could not be set up
     */
    SQLAccountRepository(QSqlDatabase & db, std::shared_ptr<CurrencyRepository> currencies);

    /**
     * Creates a new account as a child of the specified parent account.
     *
     * @param[in] account Account to be created
     * @param[in] parent  Account under which to create the new account
     * @return ID assigned to the newly created account
     */
    int create(const Account & account, const Account & parent) override;

    /**
     * Retrieves an account from the repository with the given ID.
     *
     * If no account exists for the given ID, an account with a -1 ID will be returned.
     *
     * @param[in] id Account ID to be retrieved
     * @return Account for the given ID
     */
    Account getAccount(int id) override;

    /**
     * Retrieves all leaf accounts.
     *
     * @return List of all leaf accounts
     */
    std::vector<Account> getLeafAccounts() override;

    /**
     * Retrieves the root account from the repository.
     *
     * @return Root account
     * @throw std::invalid_argument if no root account exists
     */
    Account getRoot() override;

    /**
     * Returns the message of the last error to have occurred.
     *
     * @return Last error message
     */
    QString lastError() override;

    /**
     * Moves the given account to the specified parent account.
     *
     * @param[in] account Account to be moved
     * @param[in] parent  New parent account
     * @return @c true if successful
     */
    bool move(const Account & account, const Account & parent) override;

    /**
     * Removes the given account from the repository.
     *
     * The ID is the only parameter required to match the existing entry.
     *
     * @param account Account to be removed
     * @return @c true if successful
     */
    bool remove(const Account & account) override;

    /**
     * Updates the given account in the repository.
     *
     * The ID must match an existing entry. All other parameters will be updated.
     *
     * @param account Account to be updated
     * @return @c true if successful
     */
    bool update(const Account & account) override;

private:
    /** Currency table name */
    static QString table_name_;

    /** Account with additional nested set model fields */
    class NestedSetAccount : public Account {
    public:
        /** Left value */
        int lft = 0;
        /** Right value */
        int rgt = 0;

        /**
         * Initializes an account with default parameters.
         */
        NestedSetAccount() : Account() {}

        /**
         * Initializes an account with the given ID.
         *
         * @param[in] id Account ID
         */
        NestedSetAccount(int id) : Account(id) {}
    };

    /** Database connection */
    QSqlDatabase db_;
    /** Currency repository */
    std::shared_ptr<CurrencyRepository> currencies_;
    /** Last error message */
    QString last_error_;
    /** Cached accounts */
    std::map<int, NestedSetAccount> accounts_;

    /**
     * Populates the cache with all account content from the database.
     */
    void cache();
};

} // ledger namespace
} // ub namespace
