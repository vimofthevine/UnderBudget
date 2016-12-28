#pragma once

// Qt include(s)
#include <QSqlDatabase>
#include <QString>

// UnderBudget include(s)
#include <ledger/model/Currency.hpp>
#include <ledger/model/CurrencyRepository.hpp>

namespace ub {
namespace ledger {

/**
 * Currency repository implementation that uses an SQLite database.
 *
 * @ingroup ledger
 */
class SQLCurrencyRepository : public CurrencyRepository {
public:
    /**
     * Initializes the repository with the given database connection.
     *
     * This will set up the currency table and add an entry for the default
     * currency if one does not exist.
     *
     * @param[in] db SQL database connection
     * @throw std::runtime_error if the database table could not be set up
     */
    SQLCurrencyRepository(QSqlDatabase & db);

    /**
     * Creates a new currency in the repository with the parameters of the given currency.
     *
     * @param[in] currency Currency to be created
     * @return ID assigned to the newly created currency
     */
    int create(const Currency & currency) override;

    /**
     * Retrieves a currency from the repository with the given ID.
     *
     * @param[in] id Currency ID to be retrieved
     * @return Currency for the given ID
     * @throw std::invalid_argument if no currency exists for the given ID
     */
    Currency getCurrency(int id) override;

    /**
     * Returns the message of the last error to have occurred.
     *
     * @return Last error message
     */
    QString lastError() override;

    /**
     * Removes the given currency from the repository.
     *
     * The ID is the only parameter required to match the existing entry.
     *
     * @param currency Currency to be removed
     * @return @c true if successful
     */
    bool remove(const Currency & currency) override;

    /**
     * Updates the given currency in the repository.
     *
     * The ID must match an existing entry. All other parameters will be updated.
     *
     * Only the default (0 ID) currency may be updated. All others are unmodifiable.
     *
     * @param currency Currency to be updated
     * @return @c true if successful
     * @throw std::invalid_argument if currency is not the default (0 ID) currency
     */
    bool update(const Currency & currency) override;

private:
    /** Currency table name */
    static QString table_name_;

    /** Database connection */
    QSqlDatabase db_;
    /** Last error message */
    QString last_error_;
};

} // ledger namespace
} // ub namespace
