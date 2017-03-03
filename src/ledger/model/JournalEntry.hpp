#pragma once

// Standard include(s)
#include <memory>
#include <vector>

// Qt include(s)
#include <QString>

// UnderBudget include(s)
#include "AccountTransaction.hpp"
#include "EnvelopeTransaction.hpp"
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

    /** Account transaction splits to be removed */
    std::vector<AccountTransaction> account_splits_to_remove_;
    /** Envelope transaction splits to be removed */
    std::vector<EnvelopeTransaction> envelope_splits_to_remove_;
};

} // ledger namespace
} // ub namespace
