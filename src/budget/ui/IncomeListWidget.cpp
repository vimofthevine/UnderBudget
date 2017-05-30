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
#include <ledger/ui/AccountDetailsDialog.hpp>
#include <ledger/ui/AccountModel.hpp>
#include <ledger/ui/TreeView.hpp>
#include "BudgetTableView.hpp"
#include "IncomeDetailsDialog.hpp"
#include "IncomeListWidget.hpp"
#include "IncomeModel.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
IncomeListWidget::IncomeListWidget(ledger::AccountModel * accounts,
                                     IncomeModel * incomes, QWidget * parent)
        : QSplitter(Qt::Horizontal, parent), accounts_(accounts), incomes_(incomes),
          account_details_(new ledger::AccountDetailsDialog(accounts_, parent)),
          income_details_(new IncomeDetailsDialog(incomes_, parent)),
          tree_(new ledger::TreeView(this)), table_(new BudgetTableView(this)) {
    tree_->setModel(accounts_);
    table_->setModel(incomes_);

    account_details_->hide();
    account_details_->setModal(true);

    income_details_->hide();
    income_details_->setModal(true);

    connect(tree_, &ledger::TreeView::selectItem, this, &IncomeListWidget::selectAccount);
    connect(tree_, &ledger::TreeView::selectItem, this, &IncomeListWidget::setIncomeFilter);
    connect(tree_, &ledger::TreeView::createItem, account_details_,
            &ledger::AccountDetailsDialog::resetForNewAccount);
    connect(tree_, &ledger::TreeView::modifyItem, account_details_,
            &ledger::AccountDetailsDialog::showAccount);
    connect(tree_, &ledger::TreeView::deleteItem, this, &IncomeListWidget::deleteAccount);

    connect(table_, &BudgetTableView::createItem, this, &IncomeListWidget::createIncome);
    connect(table_, &BudgetTableView::modifyItem, income_details_,
            &IncomeDetailsDialog::showIncome);
    connect(table_, &BudgetTableView::duplicateItem, income_details_,
            &IncomeDetailsDialog::duplicateIncome);
    connect(table_, &BudgetTableView::deleteItem, this,
            &IncomeListWidget::deleteIncome);

    addWidget(tree_);
    addWidget(table_);

    // Give income list stretch priority
    setStretchFactor(1, 1);
}

//--------------------------------------------------------------------------------------------------
void IncomeListWidget::showAccount(const ledger::Account & account) {
    tree_->select(accounts_->index(account));
}

//--------------------------------------------------------------------------------------------------
void IncomeListWidget::deleteAccount(const QModelIndex & index) {
    ledger::Account account = accounts_->account(index);
    auto answer =
        QMessageBox::question(this->parentWidget(), tr("Delete Account?"),
                              tr("Are you sure you want to delete %0?").arg(account.name()));
    if (answer == QMessageBox::Yes) {
        accounts_->remove(index);
    }
}

//--------------------------------------------------------------------------------------------------
void IncomeListWidget::setIncomeFilter(const QModelIndex & current,
                                              const QModelIndex & previous) {
    account_ = accounts_->account(current);
    incomes_->filterForAccount(account_);
}

//--------------------------------------------------------------------------------------------------
void IncomeListWidget::createIncome() {
    if (account_.id() > 0) {
        income_details_->resetForNewIncome(account_);
    }
}

//--------------------------------------------------------------------------------------------------
void IncomeListWidget::deleteIncome(const QModelIndex & index) {
    Income income = incomes_->income(index);
    auto answer =
            QMessageBox::question(this->parentWidget(), tr("Delete Budgeted Income?"),
                                  tr("Are you sure you want to delete %0?").arg(income.description()));
    if (answer == QMessageBox::Yes) {
        incomes_->remove(index);
    }
}

} // budget namespace
} // ub namespace
