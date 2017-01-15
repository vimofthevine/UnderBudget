#pragma once

// Standard include(s)
#include <memory>
#include <vector>

// Qt include(s)
#include <QDate>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QString>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountTransaction.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/model/EnvelopeTransaction.hpp>
#include <ledger/model/Money.hpp>
#include <ledger/model/Transaction.hpp>
#include <ledger/model/AccountRepository.hpp>
#include <ledger/model/EnvelopeRepository.hpp>
#include <ledger/model/TransactionRepository.hpp>

namespace ub {
namespace ledger {

/**
 * Transaction repository implementation that uses an SQLite database.
 *
 * @ingroup ledger
 */
class SQLTransactionRepository : public TransactionRepository {
public:
    /**
     * Initializes the repository with the given database connection.
     *
     * This will set up the transaction tables if they do not exist.
     *
     * @param[in] db SQL database connection
     * @throw std::runtime_error if any table could not be set up
     */
    SQLTransactionRepository(QSqlDatabase & db, std::shared_ptr<AccountRepository> accounts,
                             std::shared_ptr<EnvelopeRepository> envelopes);

    /**
     * Creates a new account transaction.
     *
     * @param[in] transaction Account transaction to be created
     * @return ID assigned to the newly created transaction
     */
    int create(const AccountTransaction & transaction) override;

    /**
     * Creates a new envelope transaction.
     *
     * @param[in] transaction Envelope transaction to be created
     * @return ID assigned to the newly created transaction
     */
    int create(const EnvelopeTransaction & transaction) override;

    /**
     * Creates a new double-entry transaction.
     *
     * @param[in] transaction Transaction to be created
     * @return ID assigned to the newly created transaction
     */
    int create(const Transaction & transaction) override;

    /**
     * Retrieves an account transaction from the repository with the given ID.
     *
     * If no account transaction exists for the given ID, a transaction with a -1 ID will be
     * returned.
     *
     * @param[in] id Account transaction ID to be retrieved
     * @return Account transaction for the given ID
     */
    AccountTransaction getAccountTransaction(int id) override;

    /**
     * Retrieves all account transactions as part of the given double-entry transaction.
     *
     * @param[in] transaction Double-entry transaction for which to retrieve account transactions
     * @return Account transactions in the specified transaction
     */
    std::vector<AccountTransaction> getAccountTransactions(
            const Transaction & transaction) override;

    /**
     * Retrieves the balance of the given account as of the specified date.
     *
     * @param[in] date    Date for which to calculate the balance
     * @param[in] account Account for which to calculate the balance
     * @return Balance of the account as of the specified date
     */
    Money getBalance(const QDate & date, const Account & account) override;

    /**
     * Retrieves the balance of the given envelope as of the specified date.
     *
     * @param[in] date     Date for which to calculate the balance
     * @param[in] envelope Envelope for which to calculate the balance
     * @return Balance of the envelope as of the specified date
     */
    Money getBalance(const QDate & date, const Envelope & envelope) override;

    /**
     * Retrieves an envelope transaction from the repository with the given ID.
     *
     * If no envelope transaction exists for the given ID, a transaction with a -1 ID will be
     * returned.
     *
     * @param[in] id Envelope transaction ID to be retrieved
     * @return Envelope transaction for the given ID
     */
    EnvelopeTransaction getEnvelopeTransaction(int id) override;

    /**
     * Retrieves all envelope transactions as part of the given double-entry transaction.
     *
     * @param[in] transaction Double-entry transaction for which to retrieve envelope transactions
     * @return Envelope transactions in the specified transaction
     */
    std::vector<EnvelopeTransaction> getEnvelopeTransactions(
            const Transaction & transaction) override;

    /**
     * Retrieves the reconciled balance of the given account as of the specified date.
     *
     * @param[in] date    Date for which to calculate the balance
     * @param[in] account Account for which to calculate the balance
     * @return Reconciled balance of the account as of the specified date
     */
    Money getReconciledBalance(const QDate & date, const Account & account) override;

    /**
     * Retrieves an double-entry transaction from the repository with the given ID.
     *
     * If no transaction exists for the given ID, a transaction with a -1 ID will be returned.
     *
     * @param[in] id Transaction ID to be retrieved
     * @return Transaction for the given ID
     */
    Transaction getTransaction(int id) override;

    /**
     * Retrieves all transactions against the given account.
     *
     * @param[in] account Account for which to retrieve associated transactions
     * @return Transactions against the given account
     */
    std::vector<AccountTransaction> getTransactions(const Account & account) override;

    /**
     * Retrieves all transactions against the given envelope.
     *
     * @param[in] envelope Envelope for which to retrieve associated transactions
     * @return Transactions against the given envelope
     */
    std::vector<EnvelopeTransaction> getTransactions(const Envelope & envelope) override;

    /**
     * Returns the message of the last error to have occurred.
     *
     * @return Last error message
     */
    QString lastError() override;

    /**
     * Removes the given double-entry transaction from the repository.
     *
     * The ID is the only parameter required to match the existing entry.
     *
     * @param transaction Transaction to be removed
     * @return @c true if successful
     */
    bool remove(const Transaction & transaction) override;

    /**
     * Removes the given account transaction from the repository.
     *
     * The ID is the only parameter required to match the existing entry.
     *
     * @param transaction Transaction to be removed
     * @return @c true if successful
     */
    bool remove(const AccountTransaction & transaction) override;

    /**
     * Removes the given envelope transaction from the repository.
     *
     * The ID is the only parameter required to match the existing entry.
     *
     * @param transaction Transaction to be removed
     * @return @c true if successful
     */
    bool remove(const EnvelopeTransaction & transaction) override;

    /**
     * Applies all changes to the repository.
     *
     * @return @c true if successful
     */
    bool save() override;

    /** Updates the given transaction in the repository.
     *
     * The ID must match an existing entry. All other parameters will be updated.
     *
     * @param transaction Transaction to be updated
     * @return @c true if successful
     */
    bool update(const Transaction & transaction) override;

    /** Updates the given account transaction in the repository.
     *
     * The ID must match an existing entry. All other parameters will be updated.
     *
     * @param transaction Transaction to be updated
     * @return @c true if successful
     */
    bool update(const AccountTransaction & transaction) override;

    /** Updates the given envelope transaction in the repository.
     *
     * The ID must match an existing entry. All other parameters will be updated.
     *
     * @param transaction Transaction to be updated
     * @return @c true if successful
     */
    bool update(const EnvelopeTransaction & transaction) override;

private:
    /** Database connection */
    QSqlDatabase db_;
    /** Last error message */
    QString last_error_;
    /** Account repository */
    std::shared_ptr<AccountRepository> accounts_;
    /** Envelope repository */
    std::shared_ptr<EnvelopeRepository> envelopes_;
    /** Currenty in a database transaction */
    bool in_transaction_;

    /**
     * Creates an account transaction from the given SQL record.
     *
     * @param record SQL record
     * @return Account transaction
     */
    AccountTransaction toAccountTransaction(const QSqlRecord & record);

    /**
     * Creates an envelope transaction from the given SQL record.
     *
     * @param record SQL record
     * @return Envelope transaction
     */
    EnvelopeTransaction toEnvelopeTransaction(const QSqlRecord & record);
};

} // ledger namespace
} // ub namespace
