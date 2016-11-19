#pragma once

// Qt include(s)
#include <QSqlDatabase>
#include <QString>

namespace ub {
namespace ledger {

/**
 * An asset account corresponds to the real-world location of immediately available funds, such as
 * a bank account or cash on hand.
 *
 * The current balance of any given asset account is fully known, and the sum of all asset accounts
 * represents the user's net worth.
 *
 * All balances are stored as scaled integer values. The scaling factory is determined by the
 * currency associated with the account.
 *
 * @ingroup ledger
 */
class AssetAccount {
public:
    /**
     * Ensures that the asset_accounts table exists and contains the proper columns.
     *
     * @return @c true if successful
     */
    static bool prepareDatabase();

    /**
     * Initializes an account with the given ID.
     *
     * If the ID exists in the database, all fields are intialized from the database values.
     *
     * @param[in] id Account ID
     */
    explicit AssetAccount(int id);

    /**
     * Initializes an account with the given name.
     *
     * The balance of the account is initialized to zero, and uses the default currency.
     *
     * @param[in] name Account name
     */
    explicit AssetAccount(const QString & name);

    /**
     * Returns the account number for this account.
     *
     * @return Account number
     */
    QString accountNumber() const;

    /**
     * Returns the ID of the currency of this account.
     *
     * A value of 0 indicates that the global default currency is used.
     *
     * @return Currency ID
     */
    int currencyType() const;

    /**
     * Returns the scaled current balance of this account.
     *
     * @return Scaled current balance
     */
    int currentBalance() const;

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
     * Returns the scaled reconciled balance of this account.
     *
     * @return Scaled reconciled balance
     */
    int reconciledBalance() const;

    /**
     * Saves the current account fields in the database.
     *
     * If this account has not yet been added to the database, it will be inserted.
     * Otherwise the appropriate database row will be updated.
     *
     * @return @c true if successful
     */
    bool save();

    /**
     * Updates the account number for this account.
     *
     * @param[in] number New account number
     * @return @c true if successful
     */
    bool setAccountNumber(const QString & number);

    /**
     * Updates the currenty type ID of this account.
     *
     * @param[in] id New currency type ID
     * @return @c true if successful
     */
    bool setCurrencyType(int id);

    /**
     * Updates the scaled current balance of this account.
     *
     * @param[in] balance New current balance
     * @return @c true if successful
     */
    bool setCurrentBalance(int balance);

    /**
     * Updates the financial institution of this account.
     *
     * @param[in] institution New financial institution
     * @return @c true if successful
     */
    bool setFinancialInstitution(const QString & institution);

    /**
     * Updates the name of this account.
     *
     * @param[in] name New account name
     * @return @c true if successful
     */
    bool setName(const QString & name);

    /**
     * Updates the scaled reconciled balance of this account.
     *
     * @param[in] balance New reconciled balance
     * @return @c true if successful
     */
    bool setReconciledBalance(int balance);

private:
    /** Asset accounts table name */
    static QString table_name_;

    /** Database connection */
    QSqlDatabase db_;

    /** Name of this account */
    QString account_name_;
    /** Account number */
    QString account_number_;
    /** Currency ID */
    int currency_id_;
    /** Current balance, as a scaled value */
    int current_balance_;
    /** Account ID */
    int id_;
    /** Financial institution */
    QString institution_;
    /** Reconciled balance, as a scaled value */
    int reconciled_balance_;
};

} // ledger namespace
} // ub namespace
