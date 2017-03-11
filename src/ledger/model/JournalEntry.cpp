/*
 * Copyright 2017 Kyle Treubig
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

// Standard include(s)
#include <vector>

// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "AccountTransaction.hpp"
#include "EnvelopeTransaction.hpp"
#include "JournalEntry.hpp"
#include "Transaction.hpp"
#include "TransactionRepository.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
JournalEntry::JournalEntry(std::shared_ptr<TransactionRepository> repo) : transactions_(repo) {}

//--------------------------------------------------------------------------------------------------
JournalEntry::JournalEntry(std::shared_ptr<TransactionRepository> repo,
                           const Transaction & transaction)
        : transactions_(repo) {
    transaction_ = transactions_->getTransaction(transaction.id());
    account_splits_ = transactions_->getAccountTransactions(transaction);
    envelope_splits_ = transactions_->getEnvelopeTransactions(transaction);
}

//--------------------------------------------------------------------------------------------------
JournalEntry::JournalEntry(std::shared_ptr<TransactionRepository> repo,
                           const Transaction & transaction, bool copy)
        : transactions_(repo) {
    auto trn = transactions_->getTransaction(transaction.id());
    transaction_.setDate(trn.date());
    transaction_.setPayee(trn.payee());

    auto acct_splits = transactions_->getAccountTransactions(transaction);
    for (auto acct_split : acct_splits) {
        AccountTransaction split;
        split.setAccount(acct_split.account());
        split.setAmount(acct_split.amount());
        split.setCleared(acct_split.isCleared());
        split.setMemo(acct_split.memo());
        account_splits_.push_back(split);
    }

    auto env_splits = transactions_->getEnvelopeTransactions(transaction);
    for (auto env_split : env_splits) {
        EnvelopeTransaction split;
        split.setEnvelope(env_split.envelope());
        split.setAmount(env_split.amount());
        split.setMemo(env_split.memo());
        envelope_splits_.push_back(split);
    }
}

//--------------------------------------------------------------------------------------------------
Money JournalEntry::accountImbalance() const {
    if (isValid()) {
        return Money();
    } else {
        return (envelope_total_ - account_total_);
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntry::addSplit(const AccountTransaction & transaction) {
    account_splits_.push_back(transaction);
}

//--------------------------------------------------------------------------------------------------
void JournalEntry::addSplit(const EnvelopeTransaction & transaction) {
    envelope_splits_.push_back(transaction);
}

//--------------------------------------------------------------------------------------------------
Money JournalEntry::envelopeImbalance() const {
    if (isValid()) {
        return Money();
    } else {
        return (account_total_ - envelope_total_);
    }
}

//--------------------------------------------------------------------------------------------------
std::vector<AccountTransaction> JournalEntry::getAccountSplits() const {
    return account_splits_;
}

//--------------------------------------------------------------------------------------------------
std::vector<EnvelopeTransaction> JournalEntry::getEnvelopeSplits() const {
    return envelope_splits_;
}

//--------------------------------------------------------------------------------------------------
Transaction JournalEntry::getTransaction() const {
    return transaction_;
}

//--------------------------------------------------------------------------------------------------
bool JournalEntry::isValid() const {
    if (account_splits_.empty() and envelope_splits_.empty()) {
        last_error_ = QObject::tr("No account or envelope splits defined");
        return false;
    }

    if ((account_splits_.size() > 1u) and (envelope_splits_.size() > 1u)) {
        last_error_ = QObject::tr("Multiple account and multiple envelope splits defined");
        return false;
    }

    Currency currency = (account_splits_.empty() ? envelope_splits_[0].amount().currency()
                                                 : account_splits_[0].amount().currency());
    account_total_ = Money(0, currency);
    envelope_total_ = Money(0, currency);
    for (AccountTransaction transaction : account_splits_) {
        if (currency != transaction.amount().currency()) {
            last_error_ = QObject::tr("Currency conversion would be required but is not supported");
            return false;
        }
        account_total_ += transaction.amount();
    }
    for (EnvelopeTransaction transaction : envelope_splits_) {
        if (currency != transaction.amount().currency()) {
            last_error_ = QObject::tr("Currency conversion would be required but is not supported");
            return false;
        }
        envelope_total_ += transaction.amount();
    }

    if (not (account_total_ - envelope_total_).isZero()) {
        last_error_ = QObject::tr("Account split sum less the envelope split sum must equal zero");
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------
QString JournalEntry::lastError() const {
    return last_error_;
}

//--------------------------------------------------------------------------------------------------
void JournalEntry::removeSplit(const AccountTransaction & transaction) {
    auto iter = account_splits_.begin();
    while (iter != account_splits_.end()) {
        if ((transaction.id() == iter->id()) and
            (transaction.account().id() == iter->account().id()) and
            (transaction.amount() == iter->amount()) and (transaction.memo() == iter->memo())) {
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
void JournalEntry::removeSplit(const EnvelopeTransaction & transaction) {
    auto iter = envelope_splits_.begin();
    while (iter != envelope_splits_.end()) {
        if ((transaction.id() == iter->id()) and
            (transaction.envelope().id() == iter->envelope().id()) and
            (transaction.amount() == iter->amount()) and (transaction.memo() == iter->memo())) {
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
bool JournalEntry::save() {
    if (not isValid()) {
        qDebug() << "entry is not valid";
        return false;
    }

    if (transaction_.id() > 0) {
        if (not transactions_->update(transaction_)) {
            last_error_ = "Transaction update error: " + transactions_->lastError();
            return false;
        }
    } else {
        int id = transactions_->create(transaction_);
        if (id <= 0) {
            last_error_ = "Transaction create error: " + transactions_->lastError();
            return false;
        } else {
            transaction_ = transactions_->getTransaction(id);
        }
    }

    for (auto transaction : account_splits_to_remove_) {
        if (not transactions_->remove(transaction)) {
            last_error_ = "Account transaction removal error: " + transactions_->lastError();
            return false;
        }
    }

    for (auto transaction : envelope_splits_to_remove_) {
        if (not transactions_->remove(transaction)) {
            last_error_ = "Envelope transaction removal error: " + transactions_->lastError();
            return false;
        }
    }

    for (auto transaction : account_splits_) {
        if (transaction.id() > 0) {
            if (not transactions_->update(transaction)) {
                last_error_ = "Account transaction update error: " + transactions_->lastError();
                return false;
            }
        } else {
            transaction.setTransaction(transaction_);
            if (transactions_->create(transaction) <= 0) {
                last_error_ = "Account transaction create error: " + transactions_->lastError();
                return false;
            }
        }
    }

    for (auto transaction : envelope_splits_) {
        if (transaction.id() > 0) {
            if (not transactions_->update(transaction)) {
                last_error_ = "Envelope transaction update error: " + transactions_->lastError();
                return false;
            }
        } else {
            transaction.setTransaction(transaction_);
            if (transactions_->create(transaction) <= 0) {
                last_error_ = "Envelope transaction create error: " + transactions_->lastError();
                return false;
            }
        }
    }

    bool saved = transactions_->save();
    if (not saved) {
        last_error_ = "Ledger entry create error: " + transactions_->lastError();
    }
    return saved;
}

//--------------------------------------------------------------------------------------------------
void JournalEntry::updateSplit(const AccountTransaction & transaction, size_t pos) {
    if ((pos < account_splits_.size()) and (transaction.id() == account_splits_[pos].id())) {
        account_splits_[pos] = transaction;
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntry::updateSplit(const EnvelopeTransaction & transaction, size_t pos) {
    if ((pos < envelope_splits_.size()) and (transaction.id() == envelope_splits_[pos].id())) {
        envelope_splits_[pos] = transaction;
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntry::updateTransaction(const Transaction & transaction) {
    if (transaction.id() == transaction_.id()) {
        transaction_ = transaction;
    }
}

} // ledger namespace
} // ub namespace
