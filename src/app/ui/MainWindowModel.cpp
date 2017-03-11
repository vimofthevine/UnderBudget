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
#include <app/model/Repositories.hpp>
#include <budget/ui/ExpenseListWidget.hpp>
#include <budget/ui/ExpenseModel.hpp>
#include <ledger/ui/AccountListWidget.hpp>
#include <ledger/ui/AccountModel.hpp>
#include <ledger/ui/AccountTransactionModel.hpp>
#include <ledger/ui/EnvelopeListWidget.hpp>
#include <ledger/ui/EnvelopeModel.hpp>
#include <ledger/ui/EnvelopeTransactionModel.hpp>
#include <ledger/ui/JournalEntryDialog.hpp>
#include "DatabaseFileChooser.hpp"
#include "MainWindow.hpp"
#include "MainWindowModel.hpp"
#include "MenuBar.hpp"

namespace ub {

//--------------------------------------------------------------------------------------------------
MainWindowModel::MainWindowModel(MainWindow * window)
        : QObject(window), account_model_(new ledger::AccountModel),
          account_transaction_model_(new ledger::AccountTransactionModel),
          envelope_model_(new ledger::EnvelopeModel),
          envelope_transaction_model_(new ledger::EnvelopeTransactionModel),
          expense_model_(new budget::ExpenseModel), window_(window),
          account_list_(
              new ledger::AccountListWidget(account_model_, account_transaction_model_, window_)),
          envelope_list_(new ledger::EnvelopeListWidget(envelope_model_,
                                                        envelope_transaction_model_, window_)),
          journal_entry_(new ledger::JournalEntryDialog(window_)),
          expense_list_(new budget::ExpenseListWidget(envelope_model_, expense_model_, window_)){
    window_->contentWidget()->addWidget(account_list_);
    window_->contentWidget()->addWidget(envelope_list_);
    window_->contentWidget()->addWidget(expense_list_);

    journal_entry_->hide();
    journal_entry_->setModal(true);

    auto menu = window_->menu();
    connect(menu, &MenuBar::openDatabase, this, &MainWindowModel::openDatabase);
    connect(menu, &MenuBar::addTransaction, journal_entry_,
            &ledger::JournalEntryDialog::prepareForNewEntry);
    connect(menu, &MenuBar::viewAccounts, this, &MainWindowModel::showAccounts);
    connect(menu, &MenuBar::viewEnvelopes, this, &MainWindowModel::showEnvelopes);
    connect(menu, &MenuBar::viewBudgetedExpenses, this, &MainWindowModel::showBudgetedExpenses);

    connect(account_model_, &ledger::AccountModel::error, this, &MainWindowModel::showError);
    connect(account_transaction_model_, &ledger::AccountTransactionModel::error, this,
            &MainWindowModel::showError);
    connect(account_list_, &ledger::AccountListWidget::modifyTransaction, journal_entry_,
            &ledger::JournalEntryDialog::prepareForModification);
    connect(account_list_, &ledger::AccountListWidget::duplicateTransaction, journal_entry_,
            &ledger::JournalEntryDialog::prepareForDuplication);
    connect(account_list_, &ledger::AccountListWidget::deleteTransaction, journal_entry_,
            &ledger::JournalEntryDialog::deleteTransaction);
    connect(journal_entry_, &ledger::JournalEntryDialog::accepted, account_transaction_model_,
            &ledger::AccountTransactionModel::refresh);

    connect(envelope_model_, &ledger::EnvelopeModel::error, this, &MainWindowModel::showError);
    connect(envelope_transaction_model_, &ledger::EnvelopeTransactionModel::error, this,
            &MainWindowModel::showError);
    connect(envelope_list_, &ledger::EnvelopeListWidget::modifyTransaction, journal_entry_,
            &ledger::JournalEntryDialog::prepareForModification);
    connect(envelope_list_, &ledger::EnvelopeListWidget::duplicateTransaction, journal_entry_,
            &ledger::JournalEntryDialog::prepareForDuplication);
    connect(envelope_list_, &ledger::EnvelopeListWidget::deleteTransaction, journal_entry_,
            &ledger::JournalEntryDialog::deleteTransaction);
    connect(journal_entry_, &ledger::JournalEntryDialog::accepted, envelope_transaction_model_,
            &ledger::EnvelopeTransactionModel::refresh);

    connect(expense_model_, &budget::ExpenseModel::error, this, &MainWindowModel::showError);
}

//--------------------------------------------------------------------------------------------------
void MainWindowModel::setRepositories(std::shared_ptr<Repositories> repositories) {
    account_model_->setRepository(repositories);
    account_transaction_model_->setRepository(repositories);
    envelope_model_->setRepository(repositories);
    envelope_transaction_model_->setRepository(repositories);
    journal_entry_->setRepository(repositories);
    expense_model_->setRepository(repositories);
}

//--------------------------------------------------------------------------------------------------
void MainWindowModel::openDatabase() {
    auto file = ub::DatabaseFileChooser::getFileToOpen(window_);
}

//--------------------------------------------------------------------------------------------------
void MainWindowModel::showAccounts() {
    window_->contentWidget()->setCurrentWidget(account_list_);
}

//--------------------------------------------------------------------------------------------------
void MainWindowModel::showEnvelopes() {
    window_->contentWidget()->setCurrentWidget(envelope_list_);
}

//--------------------------------------------------------------------------------------------------
void MainWindowModel::showBudgetedExpenses() {
    window_->contentWidget()->setCurrentWidget(expense_list_);
}

//--------------------------------------------------------------------------------------------------
void MainWindowModel::showError(const QString & message) {
    QMessageBox::warning(window_, tr("Error"), message);
}

} // ub namespace
