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

// Qt include(s)
#include <QObject>
#include <QString>

namespace ub {

// Forward declaration(s)
class MainWindow;
class Repositories;

namespace budget {
// Forward declaration(s)
class ExpenseListWidget;
class ExpenseModel;
class IncomeListWidget;
class IncomeModel;
}

namespace ledger {
// Forward declaration(s)
class AccountListWidget;
class AccountModel;
class AccountTransactionModel;
class EnvelopeListWidget;
class EnvelopeModel;
class EnvelopeTransactionModel;
class JournalEntryDialog;
}

namespace report {
// Forward declaration(s)
class ReportWidget;
}

/**
 * Main application window model.
 */
class MainWindowModel : public QObject {
    Q_OBJECT

public:
    /**
     * Initializes the main window model.
     *
     * @param window Main window
     */
    MainWindowModel(MainWindow * window);

    /**
     * Updates the repositories used by the application.
     *
     * @param repositories
     */
    void setRepositories(std::shared_ptr<Repositories> repositories);

protected slots:
    /**
     * Prompts the user to select a database file to be opened.
     */
    void openDatabase();

    /**
     * Displays the accounts list view.
     */
    void showAccounts();

    /**
     * Displays the envelopes list view.
     */
    void showEnvelopes();

    /**
     * Displays the budgeted incomes list view.
     */
    void showBudgetedIncomes();

    /**
     * Displays the budgeted expenses list view.
     */
    void showBudgetedExpenses();

    /**
     * Displays the reports view.
     */
    void showReports();

    /**
     * Displays an error popup dialog with the given message.
     *
     * @param message Error message
     */
    void showError(const QString & message);

private:
    /** Account model */
    ledger::AccountModel * account_model_;
    /** Account transaction model */
    ledger::AccountTransactionModel * account_transaction_model_;
    /** Envelope model */
    ledger::EnvelopeModel * envelope_model_;
    /** Envelope transaction model */
    ledger::EnvelopeTransactionModel * envelope_transaction_model_;
    /** Budgeted income model */
    budget::IncomeModel * income_model_;
    /** Budgeted expense model */
    budget::ExpenseModel * expense_model_;

    /** Main window */
    MainWindow * window_;
    /** Account list widget */
    ledger::AccountListWidget * account_list_;
    /** Envelope list widget */
    ledger::EnvelopeListWidget * envelope_list_;
    /** Journal entry dialog */
    ledger::JournalEntryDialog * journal_entry_;
    /** Budgeted income list widget */
    budget::IncomeListWidget * income_list_;
    /** Budgeted expense list widget */
    budget::ExpenseListWidget * expense_list_;
    /** Report widget */
    report::ReportWidget * reports_;
};

} // ub namespace
