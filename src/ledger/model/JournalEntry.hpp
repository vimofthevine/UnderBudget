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
#include <memory>
#include <vector>

// Qt include(s)
#include <QString>

// UnderBudget include(s)
#include "AccountTransaction.hpp"
#include "EnvelopeTransaction.hpp"
#include "Money.hpp"
#include "Transaction.hpp"
#include "TransactionRepository.hpp"

namespace ub {
namespace ledger {

/**
 * A journal entry with account and envelope transactions.
 *
 * The journal entry ensures that the transaction has all required splits (account or
 * envelope transactions) and has a zero-sum. That is, the sum of all account transactions
 * less the sum of all envelope transactions is zero.
 *
 * @ingroup ledger
 */
class JournalEntry {
public:
    /**
     * Initializes a new journal entry.
     *
     * @param[in] repo Transaction repository
     */
    JournalEntry(std::shared_ptr<TransactionRepository> repo);

    /**
     * Initializes a journal entry for the existing transaction for modification.
     *
     * @param[in] repo        Transaction repository
     * @param[in] transaction Transaction to be modified
     */
    JournalEntry(std::shared_ptr<TransactionRepository> repo, const Transaction & transaction);

    /**
     * Initializes a journal entry for the existing transaction for duplication.
     *
     * @param[in] repo        Transaction repository
     * @param[in] transaction Transaction to be modified
     * @param[in] copy        Indicates to copy the existing transaction
     */
    JournalEntry(std::shared_ptr<TransactionRepository> repo, const Transaction & transaction,
                 bool copy);

    /**
     * Returns the amount of imbalance that remains in the account transactions of the journal entry.
     *
     * The value that is returned could be used as the amount of an account transaction to satisfy
     * the zero-sum requirement of a journal entry.
     *
     * A zero value indicates that the journal entry is in a satisfied state with no imbalance.
     *
     * @return Account imbalance amount
     */
    Money accountImbalance() const;

    /**
     * Adds the given account transaction split to this journal entry.
     *
     * @param transaction Account transaction split
     */
    void addSplit(const AccountTransaction & transaction);

    /**
     * Adds the given envelope transaction split to this journal entry.
     *
     * @param transaction Envelope transaction split
     */
    void addSplit(const EnvelopeTransaction & transaction);

    /**
     * Returns the amount of imbalance that remains in the envelope transactions of the journal
     * entry.
     *
     * The value that is returned could be used as the amount of an envelope transaction to satisfy
     * the zero-sum requirement of a journal entry.
     *
     * A zero value indicates that the journal entry is in a satisfied state with no imbalance.
     *
     * @return Envelope imbalance amount
     */
    Money envelopeImbalance() const;

    /**
     * Returns the list of account transaction splits in this journal entry.
     *
     * @return List of account transaction splits
     */
    std::vector<AccountTransaction> getAccountSplits() const;

    /**
     * Returns the list of envelope transaction splits in this journal entry.
     *
     * @return List of envelope transaction splits
     */
    std::vector<EnvelopeTransaction> getEnvelopeSplits() const;

    /**
     * Returns the double-entry transaction representing this journal entry.
     *
     * @return Double-entry transaction
     */
    Transaction getTransaction() const;

    /**
     * Checks if the account and envelope splits in this journal entry satisfy all required
     * conditions.
     *
     * @return @c true if the transaction splits satisfy all required conditions
     */
    bool isValid() const;

    /**
     * Returns the last validation error message.
     *
     * @return Last validation error message
     */
    QString lastError() const;

    /**
     * Removes the given account transaction split from this journal entry.
     *
     * @param transaction Account transaction split to be removed
     */
    void removeSplit(const AccountTransaction & transaction);

    /**
     * Removes the given envelope transaction split from this journal entry.
     *
     * @param transaction Envelope transaction split to be removed
     */
    void removeSplit(const EnvelopeTransaction & transaction);

    /**
     * Applies all necessary changes to the transaction repository.
     *
     * @return @c true if successful
     */
    bool save();

    /**
     * Updates the account transaction split at the given position in the splits list.
     *
     * @param transaction Account transaction to be updated
     * @param pos         Index in the account transaction splits list
     */
    void updateSplit(const AccountTransaction & transaction, size_t pos);

    /**
     * Updates the envelope transaction split at the given position in the splits list.
     *
     * @param transaction Envelope transaction to be updated
     * @param pos         Index in the envelope transaction splits list
     */
    void updateSplit(const EnvelopeTransaction & transaction, size_t pos);

    /**
     * Updates the double-entry transaction in this journal entry.
     *
     * @param transaction Transaction to be updated
     */
    void updateTransaction(const Transaction & transaction);

private:
    /** Transaction repository */
    std::shared_ptr<TransactionRepository> transactions_;
    /** Transaction */
    Transaction transaction_;
    /** Account transaction splits */
    std::vector<AccountTransaction> account_splits_;
    /** Envelope transaction splits */
    std::vector<EnvelopeTransaction> envelope_splits_;
    /** Last error */
    mutable QString last_error_;
    /** Current account total amount */
    mutable Money account_total_;
    /** Current envelope total amount */
    mutable Money envelope_total_;

    /** Account transaction splits to be removed */
    std::vector<AccountTransaction> account_splits_to_remove_;
    /** Envelope transaction splits to be removed */
    std::vector<EnvelopeTransaction> envelope_splits_to_remove_;
};

} // ledger namespace
} // ub namespace
