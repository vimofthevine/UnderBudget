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
        : QDialog(parent),
          date_(new QDateEdit(this)),
          payee_(new QLineEdit(this)),
          account_(new QComboBox(this)),
          account_amount_(new DoubleLineEdit(this)),
          account_memo_(new QLineEdit(this)),
          account_cleared_(new QCheckBox(this)),
          account_split_add_(new QPushButton(tr("Save"))),
          account_split_clear_(new QPushButton(tr("Clear"))),
          account_split_delete_(new QPushButton(tr("Delete"))),
          account_splits_(new AccountSplitModel(this)),
          account_split_table_(new QTableView(this)),
          envelope_(new QComboBox(this)),
          envelope_amount_(new DoubleLineEdit(this)),
          envelope_memo_(new QLineEdit(this)),
          envelope_split_add_(new QPushButton(tr("Save"))),
          envelope_split_clear_(new QPushButton(tr("Clear"))),
          envelope_split_delete_(new QPushButton(tr("Delete"))),
          envelope_splits_(new EnvelopeSplitModel(this)),
          envelope_split_table_(new QTableView(this)),
          buttons_(new QDialogButtonBox(QDialogButtonBox::Save
                                        | QDialogButtonBox::Reset
                                        | QDialogButtonBox::Cancel)) {
    date_->setCalendarPopup(true);

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

    connect(account_split_table_->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &JournalEntryDialog::selectAccountSplit);
    connect(account_split_add_, &QPushButton::clicked, this, &JournalEntryDialog::saveAccountSplit);
    connect(account_split_clear_, &QPushButton::clicked,
            this, &JournalEntryDialog::clearAccountSplit);
    connect(account_split_delete_, &QPushButton::clicked,
            this, &JournalEntryDialog::deleteAccountSplit);

    connect(envelope_split_table_->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &JournalEntryDialog::selectEnvelopeSplit);
    connect(envelope_split_add_, &QPushButton::clicked, this, &JournalEntryDialog::saveEnvelopeSplit);
    connect(envelope_split_clear_, &QPushButton::clicked,
            this, &JournalEntryDialog::clearEnvelopeSplit);
    connect(envelope_split_delete_, &QPushButton::clicked,
            this, &JournalEntryDialog::deleteEnvelopeSplit);

    QHBoxLayout * account_inputs = new QHBoxLayout;
    account_inputs->addWidget(account_);
    account_inputs->addWidget(new QLabel(tr("Amount:")));
    account_inputs->addWidget(account_amount_);
    account_inputs->addWidget(new QLabel(tr("Memo:")));
    account_inputs->addWidget(account_memo_);
    account_inputs->addWidget(new QLabel(tr("Cleared?")));
    account_inputs->addWidget(account_cleared_);

    QHBoxLayout * account_buttons = new QHBoxLayout;
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
        date_->setDate(QDate::currentDate());
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
void JournalEntryDialog::selectAccountSplit(const QModelIndex &current, const QModelIndex &previous) {
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
void JournalEntryDialog::saveAccountSplit() {
    if (entry_) {
        int id = account_->currentData().toInt();
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
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::clearAccountSplit() {
    // No sense in resetting the account combobox
    account_amount_->clear();
    account_cleared_->setChecked(false);
    account_memo_->clear();
    account_split_ = AccountTransaction();
    account_split_table_->clearSelection();
    account_split_delete_->setEnabled(false);
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
void JournalEntryDialog::selectEnvelopeSplit(const QModelIndex &current, const QModelIndex &previous) {
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
void JournalEntryDialog::saveEnvelopeSplit() {
    if (entry_) {
        int id = envelope_->currentData().toInt();
        Envelope env = repository_->envelopes()->getEnvelope(id);
        if (env.id() == -1) {
            QMessageBox::warning(this, tr("Error"), tr("Select an envelope for the envelope split"));
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
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::clearEnvelopeSplit() {
    // No sense in resetting the envelope combobox
    envelope_amount_->clear();
    envelope_memo_->clear();
    envelope_split_ = EnvelopeTransaction();
    envelope_split_table_->clearSelection();
    envelope_split_delete_->setEnabled(false);
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
                QMessageBox::warning(this, tr("Error"), tr("Specify valid date for the journal entry"));
                return;
            }

            QString payee = payee_->text();
            if (payee.isEmpty()) {
                QMessageBox::warning(this, tr("Error"), tr("Specify payee for the journal entry"));
                return;
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
    for (auto account : accounts) {
        account_->addItem(account.name(), account.id());
    }
}

//--------------------------------------------------------------------------------------------------
void JournalEntryDialog::populateEnvelopeComboBox() {
    envelope_->clear();
    auto envelopes = repository_->envelopes()->getLeafEnvelopes();
    for (auto envelope : envelopes) {
        envelope_->addItem(envelope.name(), envelope.id());
    }
}

} // ledger namespace
} // ub namespace
