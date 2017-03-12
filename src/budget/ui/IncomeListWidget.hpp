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

#pragma once

// Qt include(s)
#include <QSplitter>

// Forward declaration(s)
class QWidget;

// UnderBudget include(s)
#include <ledger/model/Account.hpp>

namespace ub {

namespace ledger {
// Forward declaration(s)
class AccountDetailsDialog;
class AccountModel;
class TreeView;
} // ledger namespace

namespace budget {
// Forward declaration(s)
class BudgetTableView;
class IncomeDetailsDialog;
class IncomeModel;

/**
 * Widget for displaying a list of accounts and associated budgeted incomes.
 *
 * @ingroup budget
 */
class IncomeListWidget : public QSplitter {
    Q_OBJECT

public:
    /**
     * Initializes the income list widget.
     *
     * @param[in] accounts Account model
     * @param[in] incomes  Budgeted incomes model
     * @param[in] parent   Parent widget
     */
    IncomeListWidget(ledger::AccountModel * accounts, IncomeModel * incomes,
                      QWidget * parent);

signals:
    /**
     * Emitted to indicate the currently selected account.
     *
     * @param[in] current  Model index of the currently selected account
     * @param[in] previous Model index of the previoiusly selected account
     */
    void selectAccount(const QModelIndex & current, const QModelIndex & previous);

protected slots:
    /**
     * Prompts the user to confirm deletion of the requested account.
     *
     * @param index Model index of the account to be deleted
     */
    void deleteAccount(const QModelIndex & index);

    /**
     * Updates the income model to display incomes for the selected account.
     *
     * @param[in] current  Model index of the currently selected account
     * @param[in] previous Model index of the previoiusly selected account
     */
    void setIncomeFilter(const QModelIndex & current, const QModelIndex & previous);

    /**
     * Opens the income details dialog for a new income.
     */
    void createIncome();

    /**
     * Prompts the user to confirm deletion of the requested budgeted income.
     *
     * @param index Model index of the budgeted income to be deleted
     */
    void deleteIncome(const QModelIndex & index);

private:
    /** Account model */
    ledger::AccountModel * accounts_;
    /** Budgeted income model */
    IncomeModel * incomes_;

    /** Account details dialog */
    ledger::AccountDetailsDialog * account_details_;
    /** Income details dialog */
    IncomeDetailsDialog * income_details_;
    /** Account tree view */
    ledger::TreeView * tree_;
    /** Budgeted income table view */
    BudgetTableView * table_;

    /** Currently selected account */
    ledger::Account account_;
};

} // budget namespace
} // ub namespace
