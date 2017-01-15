// Standard include(s)
#include <vector>

// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "AccountTransaction.hpp"
#include "EnvelopeTransaction.hpp"
#include "LedgerEntry.hpp"
#include "Transaction.hpp"
#include "TransactionRepository.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
LedgerEntry::LedgerEntry(std::shared_ptr<TransactionRepository> repo) : transactions_(repo) {}

//--------------------------------------------------------------------------------------------------
LedgerEntry::LedgerEntry(std::shared_ptr<TransactionRepository> repo,
                         const Transaction & transaction)
    : transactions_(repo) {
    transaction_ = transactions_->getTransaction(transaction.id());
    account_splits_ = transactions_->getAccountTransactions(transaction);
    envelope_splits_ = transactions_->getEnvelopeTransactions(transaction);
}

//--------------------------------------------------------------------------------------------------
void LedgerEntry::addSplit(const AccountTransaction & transaction) {
    account_splits_.push_back(transaction);
}

//--------------------------------------------------------------------------------------------------
void LedgerEntry::addSplit(const EnvelopeTransaction & transaction) {
    envelope_splits_.push_back(transaction);
}

//--------------------------------------------------------------------------------------------------
std::vector<AccountTransaction> LedgerEntry::getAccountSplits() const {
    return account_splits_;
}

//--------------------------------------------------------------------------------------------------
std::vector<EnvelopeTransaction> LedgerEntry::getEnvelopeSplits() const {
    return envelope_splits_;
}

//--------------------------------------------------------------------------------------------------
Transaction LedgerEntry::getTransaction() const {
    return transaction_;
}

//--------------------------------------------------------------------------------------------------
bool LedgerEntry::isValid() const {
    if (account_splits_.empty() and envelope_splits_.empty()) {
        last_error_ = QObject::tr("No account or envelope splits defined");
        return false;
    }

    if ((account_splits_.size() > 1u) and (envelope_splits_.size() > 1u)) {
        last_error_ = QObject::tr("Multiple account and multiple envelope splits defined");
        return false;
    }

    Money total(0, account_splits_[0].amount().currency());
    for (AccountTransaction transaction : account_splits_) {
        if (total.currency() != transaction.amount().currency()) {
            last_error_ = QObject::tr("Currency conversion would be required but is not supported");
            return false;
        }
        total += transaction.amount();
    }
    for (EnvelopeTransaction transaction : envelope_splits_) {
        if (total.currency() != transaction.amount().currency()) {
            last_error_ = QObject::tr("Currency conversion would be required but is not supported");
            return false;
        }
        total -= transaction.amount();
    }

    if (not total.isZero()) {
        last_error_ = QObject::tr("Account split sum less the envelope split sum must equal zero");
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------
QString LedgerEntry::lastError() const {
    return last_error_;
}

//--------------------------------------------------------------------------------------------------
void LedgerEntry::removeSplit(const AccountTransaction & transaction) {
    auto iter = account_splits_.begin();
    while (iter != account_splits_.end()) {
        if ((transaction.id() == iter->id())
                and (transaction.account().id() == iter->account().id())
                and (transaction.amount() == iter->amount())
                and (transaction.memo() == iter->memo())) {
            break;
        }
        ++iter;
    }

    if (iter != account_splits_.end()) {
        account_splits_.erase(iter);

        if (transaction.id() > 0) {
            account_splits_to_remove_.push_back(transaction);
        }
    }
}

//--------------------------------------------------------------------------------------------------
void LedgerEntry::removeSplit(const EnvelopeTransaction & transaction) {
    auto iter = envelope_splits_.begin();
    while (iter != envelope_splits_.end()) {
        if ((transaction.id() == iter->id())
                and (transaction.envelope().id() == iter->envelope().id())
                and (transaction.amount() == iter->amount())
                and (transaction.memo() == iter->memo())) {
            break;
        }
        ++iter;
    }

    if (iter != envelope_splits_.end()) {
        envelope_splits_.erase(iter);

        if (transaction.id() > 0) {
            envelope_splits_to_remove_.push_back(transaction);
        }
    }
}

//--------------------------------------------------------------------------------------------------
bool LedgerEntry::save() {
    if (not isValid()) {
        return false;
    }

    if (transaction_.id() > 0) {
        if (not transactions_->update(transaction_)) {
            last_error_ = transactions_->lastError();
            return false;
        }
    } else {
        if (not transactions_->create(transaction_)) {
            last_error_ = transactions_->lastError();
            return false;
        }
    }

    for (auto transaction : account_splits_to_remove_) {
        if (not transactions_->remove(transaction)) {
            last_error_ = transactions_->lastError();
            return false;
        }
    }

    for (auto transaction : envelope_splits_to_remove_) {
        if (not transactions_->remove(transaction)) {
            last_error_ = transactions_->lastError();
            return false;
        }
    }

    for (auto transaction : account_splits_) {
        if (transaction.id() > 0) {
            if (not transactions_->update(transaction)) {
                last_error_ = transactions_->lastError();
                return false;
            }
        } else {
            if (not transactions_->create(transaction)) {
                last_error_ = transactions_->lastError();
                return false;
            }
        }
    }

    for (auto transaction : envelope_splits_) {
        if (transaction.id() > 0) {
            if (not transactions_->update(transaction)) {
                last_error_ = transactions_->lastError();
                return false;
            }
        } else {
            if (not transactions_->create(transaction)) {
                last_error_ = transactions_->lastError();
                return false;
            }
        }
    }

    return transactions_->save();
}

//--------------------------------------------------------------------------------------------------
void LedgerEntry::updateSplit(const AccountTransaction & transaction, size_t pos) {
    if ((pos < account_splits_.size()) and (transaction.id() == account_splits_[pos].id())) {
        account_splits_[pos] = transaction;
    }
}

//--------------------------------------------------------------------------------------------------
void LedgerEntry::updateSplit(const EnvelopeTransaction & transaction, size_t pos) {
    if ((pos < envelope_splits_.size()) and (transaction.id() == envelope_splits_[pos].id())) {
        envelope_splits_[pos] = transaction;
    }
}

//--------------------------------------------------------------------------------------------------
void LedgerEntry::updateTransaction(const Transaction & transaction) {
    if (transaction.id() == transaction_.id()) {
        transaction_ = transaction;
    }
}

} // ledger namespace
} // ub namespace
