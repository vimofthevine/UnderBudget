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
#include <QDate>
#include <QString>

// UnderBudget include(s)
#include "Account.hpp"
#include "AccountTransaction.hpp"
#include "Envelope.hpp"
#include "EnvelopeTransaction.hpp"
#include "Money.hpp"
#include "Transaction.hpp"

namespace ub {
namespace ledger {

/**
 * Repository of transactions.
 *
 * Unlike the other repositories, multiple operations are typically performed that must be
 * applied atomically. Thus after all operations have been made, the @c save method must
 * be invoked to finalize and commit all changes.
 *
 * @ingroup ledger
 */
class TransactionRepository {
public:
    // Virtual destructor
    virtual ~TransactionRepository() {}

    /**
     * Creates a new account transaction.
     *
     * @param[in] transaction Account transaction to be created
     * @return ID assigned to the newly created transaction
     */
    virtual int64_t create(const AccountTransaction & transaction) = 0;

    /**
     * Creates a new envelope transaction.
     *
     * @param[in] transaction Envelope transaction to be created
     * @return ID assigned to the newly created transaction
     */
    virtual int64_t create(const EnvelopeTransaction & transaction) = 0;

    /**
     * Creates a new double-entry transaction.
     *
     * @param[in] transaction Transaction to be created
     * @return ID assigned to the newly created transaction
     */
    virtual int64_t create(const Transaction & transaction) = 0;

    /**
     * Retrieves an account transaction from the repository with the given ID.
     *
     * If no account transaction exists for the given ID, a transaction with a -1 ID will be
     * returned.
     *
     * @param[in] id Account transaction ID to be retrieved
     * @return Account transaction for the given ID
     */
    virtual AccountTransaction getAccountTransaction(int64_t id) = 0;

    /**
     * Retrieves all account transactions as part of the given double-entry transaction.
     *
     * @param[in] transaction Double-entry transaction for which to retrieve account transactions
     * @return Account transactions in the specified transaction
     */
    virtual std::vector<AccountTransaction>
    getAccountTransactions(const Transaction & transaction) = 0;

    /**
     * Retrieves all account transactions from the repository between the given dates.
     *
     * @param start Earliest date for which to retrieve transactions
     * @param stop  Latest date for which to retrieve transactions
     * @return Account transactions between the given dates
     */
    virtual std::vector<AccountTransaction> getAccountTransactions(const QDate & start,
                                                                   const QDate & stop) = 0;

    /**
     * Retrieves the total cash balance as of the specified date.
     *
     * @param[in] date     Date for which to calculate the balance
     * @param[in] currency Currency to be used for the balance
     * @return Total cash balance as of the specified date
     */
    virtual Money getBalance(const QDate & date, const Currency & currence) = 0;

    /**
     * Retrieves the balance of the given account as of the specified date.
     *
     * @param[in] date    Date for which to calculate the balance
     * @param[in] account Account for which to calculate the balance
     * @return Balance of the account as of the specified date
     */
    virtual Money getBalance(const QDate & date, const Account & account) = 0;

    /**
     * Retrieves the balance of the given envelope as of the specified date.
     *
     * @param[in] date     Date for which to calculate the balance
     * @param[in] envelope Envelope for which to calculate the balance
     * @return Balance of the envelope as of the specified date
     */
    virtual Money getBalance(const QDate & date, const Envelope & envelope) = 0;

    /**
     * Retrieves an envelope transaction from the repository with the given ID.
     *
     * If no envelope transaction exists for the given ID, a transaction with a -1 ID will be
     * returned.
     *
     * @param[in] id Envelope transaction ID to be retrieved
     * @return Envelope transaction for the given ID
     */
    virtual EnvelopeTransaction getEnvelopeTransaction(int64_t id) = 0;

    /**
     * Retrieves all envelope transactions as part of the given double-entry transaction.
     *
     * @param[in] transaction Double-entry transaction for which to retrieve envelope transactions
     * @return Envelope transactions in the specified transaction
     */
    virtual std::vector<EnvelopeTransaction>
    getEnvelopeTransactions(const Transaction & transaction) = 0;

    /**
     * Retrieves all envelope transactions from the repository between the given dates.
     *
     * @param start Earliest date for which to retrieve transactions
     * @param stop  Latest date for which to retrieve transactions
     * @return Envelope transactions between the given dates
     */
    virtual std::vector<EnvelopeTransaction> getEnvelopeTransactions(const QDate & start,
                                                                     const QDate & stop) = 0;

    /**
     * Retrieves the reconciled balance of the given account as of the specified date.
     *
     * @param[in] date    Date for which to calculate the balance
     * @param[in] account Account for which to calculate the balance
     * @return Reconciled balance of the account as of the specified date
     */
    virtual Money getReconciledBalance(const QDate & date, const Account & account) = 0;

    /**
     * Retrieves an double-entry transaction from the repository with the given ID.
     *
     * If no transaction exists for the given ID, a transaction with a -1 ID will be returned.
     *
     * @param[in] id Transaction ID to be retrieved
     * @return Transaction for the given ID
     */
    virtual Transaction getTransaction(int64_t id) = 0;

    /**
     * Retrieves all transactions against the given account.
     *
     * @param[in] account Account for which to retrieve associated transactions
     * @return Transactions against the given account
     */
    virtual std::vector<AccountTransaction> getTransactions(const Account & account) = 0;

    /**
     * Retrieves all transactions against the given envelope.
     *
     * @param[in] envelope Envelope for which to retrieve associated transactions
     * @return Transactions against the given envelope
     */
    virtual std::vector<EnvelopeTransaction> getTransactions(const Envelope & envelope) = 0;

    /**
     * Returns the message of the last error to have occurred.
     *
     * @return Last error message
     */
    virtual QString lastError() = 0;

    /**
     * Removes the given double-entry transaction from the repository.
     *
     * The ID is the only parameter required to match the existing entry.
     *
     * @param transaction Transaction to be removed
     * @return @c true if successful
     */
    virtual bool remove(const Transaction & transaction) = 0;

    /**
     * Removes the given account transaction from the repository.
     *
     * The ID is the only parameter required to match the existing entry.
     *
     * @param transaction Transaction to be removed
     * @return @c true if successful
     */
    virtual bool remove(const AccountTransaction & transaction) = 0;

    /**
     * Removes the given envelope transaction from the repository.
     *
     * The ID is the only parameter required to match the existing entry.
     *
     * @param transaction Transaction to be removed
     * @return @c true if successful
     */
    virtual bool remove(const EnvelopeTransaction & transaction) = 0;

    /**
     * Applies all changes to the repository.
     *
     * @return @c true if successful
     */
    virtual bool save() = 0;

    /** Updates the given transaction in the repository.
     *
     * The ID must match an existing entry. All other parameters will be updated.
     *
     * @param transaction Transaction to be updated
     * @return @c true if successful
     */
    virtual bool update(const Transaction & transaction) = 0;

    /** Updates the given account transaction in the repository.
     *
     * The ID must match an existing entry. All other parameters will be updated.
     *
     * @param transaction Transaction to be updated
     * @return @c true if successful
     */
    virtual bool update(const AccountTransaction & transaction) = 0;

    /** Updates the given envelope transaction in the repository.
     *
     * The ID must match an existing entry. All other parameters will be updated.
     *
     * @param transaction Transaction to be updated
     * @return @c true if successful
     */
    virtual bool update(const EnvelopeTransaction & transaction) = 0;
};

} // ledger namespace
} // ub namespace
