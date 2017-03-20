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

// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountRepository.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/model/EnvelopeRepository.hpp>
#include <ledger/model/JournalEntry.hpp>
#include <ledger/model/LedgerRepository.hpp>
#include <ledger/model/Transaction.hpp>
#include <ledger/model/TransactionRepository.hpp>
#include "AccountSplitModel.hpp"
#include "DoubleLineEdit.hpp"
#include "EnvelopeSplitModel.hpp"
#include "JournalEntryDialog.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
JournalEntryDialog::JournalEntryDialog(QWidget * parent)
        : QDialog(parent), date_(new QDateEdit(this)), payee_(new QLineEdit(this)),
          account_(new QComboBox(this)), account_amount_(new DoubleLineEdit(this)),
          account_memo_(new QLineEdit(this)), account_cleared_(new QCheckBox(this)),
          account_split_auto_calc_(new QPushButton(tr("Auto-calculate"))),
          account_split_add_(new QPushButton(tr("Save Split"))),
          account_split_clear_(new QPushButton(tr("Clear"))),
          account_split_delete_(new QPushButton(tr("Delete"))),
          account_splits_(new AccountSplitModel(this)), account_split_table_(new QTableView(this)),
          envelope_(new QComboBox(this)), envelope_amount_(new DoubleLineEdit(this)),
          envelope_memo_(new QLineEdit(this)),
          envelope_split_add_(new QPushButton(tr("Save Split"))),
          envelope_split_auto_calc_(new QPushButton(tr("Auto-calculate"))),
          envelope_split_clear_(new QPushButton(tr("Clear"))),
          envelope_split_delete_(new QPushButton(tr("Delete"))),
          envelope_splits_(new EnvelopeSplitModel(this)),
          envelope_split_table_(new QTableView(this)),
          buttons_(new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Reset |
                                        QDialogButtonBox::Cancel)),
          selected_account_id_(0), selected_envelope_id_(0) {
    date_->setCalendarPopup(true);
    date_->setDate(QDate::currentDate());

    account_split_table_->setModel(account_splits_);
    account_split_table_->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    account_split_table_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    account_split_table_->setSelectionBehavior(QTableView::SelectRows);

    envelope_split_table_->setModel(envelope_splits_);
    envelope_split_table_->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    envelope_split_table_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    envelope_split_table_->setSelectionBehavior(QTableView::SelectRows);

    setWindowTitle(tr("Journal Entry"));

    connect(buttons_, &QDialogButtonBox::clicked, this, &JournalEntryDialog::clicked);

    connect(account_split_table_->selectionModel(), &QItemSelectionModel::currentRowChanged, this,
            &JournalEntryDialog::selectAccountSplit);
    connect(account_split_auto_calc_, &QPushButton::clicked, this,
            &JournalEntryDialog::autoCalculateAccountSplitAmount);
    connect(account_split_add_, &QPushButton::clicked, this, &JournalEntryDialog::saveAccountSplit);
    connect(account_split_clear_, &QPushButton::clicked, this,
            &JournalEntryDialog::clearAccountSplit);
    connect(account_split_delete_, &QPushButton::clicked, this,
            &JournalEntryDialog::deleteAccountSplit);

    connect(envelope_split_table_->selectionModel(), &QItemSelectionModel::currentRowChanged, this,
            &JournalEntryDialog::selectEnvelopeSplit);
    connect(envelope_split_auto_calc_, &QPushButton::clicked, this,
            &JournalEntryDialog::autoCalculateEnvelopeSplitAmount);
    connect(envelope_split_add_, &QPushButton::clicked, this,
            &JournalEntryDialog::saveEnvelopeSplit);
    connect(envelope_split_clear_, &QPushButton::clicked, this,
            &JournalEntryDialog::clearEnvelopeSplit);
    connect(envelope_split_delete_, &QPushButton::clicked, this,
            &JournalEntryDialog::deleteEnvelopeSplit);

    QHBoxLayout * account_inputs = new QHBoxLayout;
    account_inputs->addWidget(account_);
    account_inputs->addWidget(new QLabel(tr("Amount:")));
    account_inputs->addWidget(account_amount_);
    account_inputs->addWidget(new QLabel(tr("Memo:")));
    account_inputs->addWidget(account_memo_);
    account_inputs->addWidget(new QLabel(tr("Cleared?")));
    account_inputs->addWidget(account_cleared_);

    QHBoxLayout * account_buttons = new QHBoxLayout;
    account_buttons->addWidget(account_split_auto_calc_);
    account_buttons->addStretch();
    account_buttons->addWidget(account_split_add_);
    account_buttons->addWidget(account_split_clear_);
    account_buttons->addWidget(account_split_delete_);

    QVBoxLayout * account = new QVBoxLayout;
    account->addLayout(account_inputs);
    account->addLayout(account_buttons);
    account->addWidget(account_split_table_);

    QHBoxLayout * envelope_inputs = new QHBoxLayout;
    envelope_inputs->addWidget(envelope_);
    envelope_inputs->addWidget(new QLabel(tr("Amount:")));
    envelope_inputs->addWidget(envelope_amount_);
    envelope_inputs->addWidget(new QLabel(tr("Memo:")));
    envelope_inputs->addWidget(envelope_memo_);

    QHBoxLayout * envelope_buttons = new QHBoxLayout;
    envelope_buttons->addWidget(envelope_split_auto_calc_);
    envelope_buttons->addStretch();
    envelope_buttons->addWidget(envelope_split_add_);
    envelope_buttons->addWidget(envelope_split_clear_);
    envelope_buttons->addWidget(envelope_split_delete_);

    QVBoxLayout * envelope = new QVBoxLayout;
    envelope->addLayout(envelope_inputs);
    envelope->addLayout(envelope_buttons);
    envelope->addWidget(envelope_split_table_);

    QFormLayout * form = new QFormLayout;
    form->addRow(tr("Date"), date_);
    form->addRow(tr("Payee"), payee_);
    form->addRow(tr("Account Splits"), account);
    form->addRow(tr("Envelope Splits"), envelope);
    form->addRow("", buttons_);

    setLayout(form);
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::setRepository(std::shared_ptr<LedgerRepository> repository) {
    repository_ = repository;
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::prepareForNewEntry() {
    if (repository_) {
        // Leave the date alone for easier entry of multiple entries on the same day
        // date_->setDate(QDate::currentDate());
        payee_->clear();
        populateAccountComboBox();
        populateEnvelopeComboBox();
        transaction_ = Transaction();
        clearAccountSplit();
        clearEnvelopeSplit();
        entry_.reset(new JournalEntry(repository_->transactions()));
        account_splits_->setJournalEntry(entry_);
        envelope_splits_->setJournalEntry(entry_);
        show();
    } else {
        qWarning() << "No ledger repository has been given to the journal entry dialog";
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::prepareForModification(const Transaction & transaction) {
    if (repository_) {
        entry_.reset(new JournalEntry(repository_->transactions(), transaction));
        populateAccountComboBox();
        populateEnvelopeComboBox();
        clearAccountSplit();
        clearEnvelopeSplit();
        transaction_ = entry_->getTransaction();
        date_->setDate(transaction_.date());
        payee_->setText(transaction_.payee());
        account_splits_->setJournalEntry(entry_);
        envelope_splits_->setJournalEntry(entry_);
        show();
    } else {
        qWarning() << "No ledger repository has been given to the journal entry dialog";
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::prepareForDuplication(const Transaction & transaction) {
    if (repository_) {
        entry_.reset(new JournalEntry(repository_->transactions(), transaction, true));
        populateAccountComboBox();
        populateEnvelopeComboBox();
        clearAccountSplit();
        clearEnvelopeSplit();
        transaction_ = entry_->getTransaction();
        date_->setDate(transaction_.date());
        payee_->setText(transaction_.payee());
        account_splits_->setJournalEntry(entry_);
        envelope_splits_->setJournalEntry(entry_);
        show();
    } else {
        qWarning() << "No ledger repository has been given to the journal entry dialog";
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::deleteTransaction(const Transaction & transaction) {
    if (repository_) {
        auto answer =
            QMessageBox::question(parentWidget(), tr("Delete Transaction?"),
                                  tr("Are you sure you want to delete transaction %1 on %2")
                                      .arg(transaction.payee())
                                      .arg(transaction.date().toString("yyyy-MM-dd")));
        if (answer == QMessageBox::Yes) {
            repository_->transactions()->remove(transaction);
            emit accepted();
        }
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::selectAccountSplit(const QModelIndex & current,
                                            const QModelIndex & previous) {
    if (entry_ and current.isValid()) {
        int row = current.row();
        auto accts = entry_->getAccountSplits();
        if ((row >= 0) and (row < accts.size())) {
            account_split_ = accts.at(row);
            account_->setCurrentText(account_split_.account().name());
            account_amount_->setValue(account_split_.amount().amount());
            account_memo_->setText(account_split_.memo());
            account_cleared_->setChecked(account_split_.isCleared());
            account_split_delete_->setEnabled(true);
        }
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::autoCalculateAccountSplitAmount() {
    if (entry_) {
        if (envelope_amount_->value() != 0.0) {
            saveEnvelopeSplit();
        }
        auto imbalance = entry_->accountImbalance();
        if (account_split_table_->currentIndex().isValid()) {
            int row = account_split_table_->currentIndex().row();
            auto accts = entry_->getAccountSplits();
            if ((row >= 0) and (row < accts.size())) {
                imbalance += accts.at(row).amount();
            }
        }
        account_amount_->setValue(imbalance.amount());
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::saveAccountSplit() {
    if (entry_) {
        int64_t id = account_->currentData().value<int64_t>();
        Account acct = repository_->accounts()->getAccount(id);
        if (acct.id() == -1) {
            QMessageBox::warning(this, tr("Error"), tr("Select an account for the account split"));
            return;
        }
        Money amount = Money(account_amount_->value(), acct.currency());
        if (amount.isZero()) {
            QMessageBox::warning(this, tr("Error"), tr("Zero amount given for the account split"));
            return;
        }

        account_split_.setAccount(acct);
        account_split_.setAmount(amount);
        account_split_.setCleared(account_cleared_->isChecked());
        account_split_.setMemo(account_memo_->text());

        if (not account_split_table_->currentIndex().isValid()) {
            entry_->addSplit(account_split_);
        } else {
            entry_->updateSplit(account_split_, account_split_table_->currentIndex().row());
        }
        account_splits_->setJournalEntry(entry_);

        clearAccountSplit();
        autoCalculateEnvelopeSplitAmount();
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::clearAccountSplit() {
    // No sense in resetting the account combobox
    account_amount_->clear();
    account_cleared_->setChecked(false);
    account_memo_->clear();
    account_split_ = AccountTransaction();
    account_split_delete_->setEnabled(false);
    if (entry_) {
        account_splits_->setJournalEntry(entry_);
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::deleteAccountSplit() {
    if (entry_) {
        entry_->removeSplit(account_split_);
        account_splits_->setJournalEntry(entry_);
        clearAccountSplit();
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::selectEnvelopeSplit(const QModelIndex & current,
                                             const QModelIndex & previous) {
    if (entry_) {
        int row = current.row();
        auto envelopes = entry_->getEnvelopeSplits();
        if ((row >= 0) and (row < envelopes.size())) {
            envelope_split_ = envelopes.at(row);
            envelope_->setCurrentText(envelope_split_.envelope().name());
            envelope_amount_->setValue(envelope_split_.amount().amount());
            envelope_memo_->setText(envelope_split_.memo());
            envelope_split_delete_->setEnabled(true);
        }
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::autoCalculateEnvelopeSplitAmount() {
    if (entry_) {
        if (account_amount_->value() != 0.0) {
            saveAccountSplit();
        }
        auto imbalance = entry_->envelopeImbalance();
        if (envelope_split_table_->currentIndex().isValid()) {
            int row = envelope_split_table_->currentIndex().row();
            auto envs = entry_->getEnvelopeSplits();
            if ((row >= 0) and (row < envs.size())) {
                imbalance += envs.at(row).amount();
            }
        }
        envelope_amount_->setValue(imbalance.amount());
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::saveEnvelopeSplit() {
    if (entry_) {
        int64_t id = envelope_->currentData().value<int64_t>();
        Envelope env = repository_->envelopes()->getEnvelope(id);
        if (env.id() == -1) {
            QMessageBox::warning(this, tr("Error"),
                                 tr("Select an envelope for the envelope split"));
            return;
        }
        Money amount = Money(envelope_amount_->value(), env.currency());
        if (amount.isZero()) {
            QMessageBox::warning(this, tr("Error"), tr("Zero amount given for the envelope split"));
            return;
        }

        envelope_split_.setEnvelope(env);
        envelope_split_.setAmount(amount);
        envelope_split_.setMemo(envelope_memo_->text());

        if (not envelope_split_table_->currentIndex().isValid()) {
            entry_->addSplit(envelope_split_);
        } else {
            entry_->updateSplit(envelope_split_, envelope_split_table_->currentIndex().row());
        }
        envelope_splits_->setJournalEntry(entry_);

        clearEnvelopeSplit();
        autoCalculateAccountSplitAmount();
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::clearEnvelopeSplit() {
    // No sense in resetting the envelope combobox
    envelope_amount_->clear();
    envelope_memo_->clear();
    envelope_split_ = EnvelopeTransaction();
    envelope_split_delete_->setEnabled(false);
    if (entry_) {
        envelope_splits_->setJournalEntry(entry_);
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::deleteEnvelopeSplit() {
    if (entry_) {
        entry_->removeSplit(envelope_split_);
        envelope_splits_->setJournalEntry(entry_);
        clearAccountSplit();
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::clicked(QAbstractButton * button) {
    if (button == buttons_->button(QDialogButtonBox::Save)) {
        if (entry_) {
            QDate date = date_->date();
            if (not date.isValid()) {
                QMessageBox::warning(this, tr("Error"),
                                     tr("Specify valid date for the journal entry"));
                return;
            }

            QString payee = payee_->text();
            if (payee.isEmpty()) {
                QMessageBox::warning(this, tr("Error"), tr("Specify payee for the journal entry"));
                return;
            }

            if (account_amount_->value() != 0.0) {
                saveAccountSplit();
            }
            if (envelope_amount_->value() != 0.0) {
                saveEnvelopeSplit();
            }

            if (not entry_->isValid()) {
                QMessageBox::warning(this, tr("Error"), entry_->lastError());
                return;
            }

            transaction_.setDate(date);
            transaction_.setPayee(payee);
            entry_->updateTransaction(transaction_);

            if (not entry_->save()) {
                QMessageBox::warning(this, tr("Error"), entry_->lastError());
            } else {
                emit accept();
            }
        }
    } else if (button == buttons_->button(QDialogButtonBox::Reset)) {
        prepareForNewEntry();
    } else if (button == buttons_->button(QDialogButtonBox::Cancel)) {
        emit accept();
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::populateAccountComboBox() {
    account_->clear();
    auto accounts = repository_->accounts()->getLeafAccounts();
    int index = 0;
    for (auto account : accounts) {
        account_->addItem(account.name(), QVariant::fromValue(account.id()));
        if (account.id() == selected_account_id_) {
            account_->setCurrentIndex(index);
        }
        ++index;
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::populateEnvelopeComboBox() {
    envelope_->clear();
    auto envelopes = repository_->envelopes()->getLeafEnvelopes();
    int index = 0;
    for (auto envelope : envelopes) {
        envelope_->addItem(envelope.name(), QVariant::fromValue(envelope.id()));
        if (envelope.id() == selected_envelope_id_) {
            envelope_->setCurrentIndex(index);
        }
        ++index;
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::setSelectedAccount(const Account & account) {
    selected_account_id_ = account.id();
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::setSelectedEnvelope(const Envelope & envelope) {
    selected_envelope_id_ = envelope.id();
}

} // ledger namespace
} // ub namespace
