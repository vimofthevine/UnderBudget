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
