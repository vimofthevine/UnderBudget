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

// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include <app/model/Repositories.hpp>
#include <budget/ui/ExpenseListWidget.hpp>
#include <budget/ui/ExpenseModel.hpp>
#include <budget/ui/IncomeListWidget.hpp>
#include <budget/ui/IncomeModel.hpp>
#include <ledger/ui/AccountListWidget.hpp>
#include <ledger/ui/AccountModel.hpp>
#include <ledger/ui/AccountTransactionModel.hpp>
#include <ledger/ui/EnvelopeListWidget.hpp>
#include <ledger/ui/EnvelopeModel.hpp>
#include <ledger/ui/EnvelopeTransactionModel.hpp>
#include <ledger/ui/JournalEntryDialog.hpp>
#include <report/ui/ReportWidget.hpp>
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
          income_model_(new budget::IncomeModel),
          expense_model_(new budget::ExpenseModel), window_(window),
          account_list_(
              new ledger::AccountListWidget(account_model_, account_transaction_model_, window_)),
          envelope_list_(new ledger::EnvelopeListWidget(envelope_model_,
                                                        envelope_transaction_model_, window_)),
          journal_entry_(new ledger::JournalEntryDialog(window_)),
          income_list_(new budget::IncomeListWidget(account_model_, income_model_, window_)),
          expense_list_(new budget::ExpenseListWidget(envelope_model_, expense_model_, window_)),
          reports_(new report::ReportWidget(window_)) {
    window_->contentWidget()->addWidget(account_list_);
    window_->contentWidget()->addWidget(envelope_list_);
    window_->contentWidget()->addWidget(income_list_);
    window_->contentWidget()->addWidget(expense_list_);
    window_->contentWidget()->addWidget(reports_);

    journal_entry_->hide();
    journal_entry_->setModal(true);

    auto menu = window_->menu();
    connect(menu, &MenuBar::openDatabase, this, &MainWindowModel::openDatabase);
    connect(menu, &MenuBar::addTransaction, journal_entry_,
            &ledger::JournalEntryDialog::prepareForNewEntry);
    connect(menu, &MenuBar::viewAccounts, this, &MainWindowModel::showAccounts);
    connect(menu, &MenuBar::viewEnvelopes, this, &MainWindowModel::showEnvelopes);
    connect(menu, &MenuBar::viewBudgetedIncomes, this, &MainWindowModel::showBudgetedIncomes);
    connect(menu, &MenuBar::viewBudgetedExpenses, this, &MainWindowModel::showBudgetedExpenses);
    connect(menu, &MenuBar::viewReports, this, &MainWindowModel::showReports);

    connect(account_model_, &ledger::AccountModel::error, this, &MainWindowModel::showError);
    connect(account_transaction_model_, &ledger::AccountTransactionModel::error, this,
            &MainWindowModel::showError);
    connect(account_list_, &ledger::AccountListWidget::accountSelected, journal_entry_,
            &ledger::JournalEntryDialog::setSelectedAccount);
    connect(account_list_, &ledger::AccountListWidget::deleteTransaction, journal_entry_,
            &ledger::JournalEntryDialog::deleteTransaction);
    connect(account_list_, &ledger::AccountListWidget::duplicateTransaction, journal_entry_,
            &ledger::JournalEntryDialog::prepareForDuplication);
    connect(account_list_, &ledger::AccountListWidget::modifyTransaction, journal_entry_,
            &ledger::JournalEntryDialog::prepareForModification);
    connect(journal_entry_, &ledger::JournalEntryDialog::accepted, account_transaction_model_,
            &ledger::AccountTransactionModel::refresh);

    connect(envelope_model_, &ledger::EnvelopeModel::error, this, &MainWindowModel::showError);
    connect(envelope_transaction_model_, &ledger::EnvelopeTransactionModel::error, this,
            &MainWindowModel::showError);
    connect(envelope_list_, &ledger::EnvelopeListWidget::deleteTransaction, journal_entry_,
            &ledger::JournalEntryDialog::deleteTransaction);
    connect(envelope_list_, &ledger::EnvelopeListWidget::duplicateTransaction, journal_entry_,
            &ledger::JournalEntryDialog::prepareForDuplication);
    connect(envelope_list_, &ledger::EnvelopeListWidget::envelopeSelected, journal_entry_,
            &ledger::JournalEntryDialog::setSelectedEnvelope);
    connect(envelope_list_, &ledger::EnvelopeListWidget::modifyTransaction, journal_entry_,
            &ledger::JournalEntryDialog::prepareForModification);
    connect(journal_entry_, &ledger::JournalEntryDialog::accepted, envelope_transaction_model_,
            &ledger::EnvelopeTransactionModel::refresh);

    connect(expense_model_, &budget::ExpenseModel::error, this, &MainWindowModel::showError);
    connect(reports_, &report::ReportWidget::error, this, &MainWindowModel::showError);
}

//--------------------------------------------------------------------------------------------------
void MainWindowModel::setRepositories(std::shared_ptr<Repositories> repositories) {
    account_model_->setRepository(repositories);
    account_transaction_model_->setRepository(repositories);
    envelope_model_->setRepository(repositories);
    envelope_transaction_model_->setRepository(repositories);
    journal_entry_->setRepository(repositories);
    income_model_->setRepository(repositories);
    expense_model_->setRepository(repositories);
    reports_->setRepository(repositories);
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
void MainWindowModel::showBudgetedIncomes() {
    window_->contentWidget()->setCurrentWidget(income_list_);
}

//--------------------------------------------------------------------------------------------------
void MainWindowModel::showBudgetedExpenses() {
    window_->contentWidget()->setCurrentWidget(expense_list_);
}

//--------------------------------------------------------------------------------------------------
void MainWindowModel::showReports() {
    window_->contentWidget()->setCurrentWidget(reports_);
}

//--------------------------------------------------------------------------------------------------
void MainWindowModel::showError(const QString & message) {
    QMessageBox::warning(window_, tr("Error"), message);
}

} // ub namespace
