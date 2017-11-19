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
IncomeListWidget::IncomeListWidget(ledger::AccountModel * accounts, IncomeModel * incomes,
                                   QWidget * parent)
        : QSplitter(Qt::Horizontal, parent), accounts_(accounts), incomes_(incomes),
          account_details_(new ledger::AccountDetailsDialog(accounts_, parent)),
          income_details_(new IncomeDetailsDialog(incomes_, parent)),
          beginning_date_(new QDateEdit(this)), ending_date_(new QDateEdit(this)),
          filter_by_date_(new QCheckBox(this)), tree_(new ledger::TreeView(this)),
          table_(new BudgetTableView(this)) {
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
    connect(table_, &BudgetTableView::deleteItem, this, &IncomeListWidget::deleteIncome);

    connect(filter_by_date_, &QCheckBox::toggled, beginning_date_, &QDateEdit::setEnabled);
    connect(filter_by_date_, &QCheckBox::toggled, ending_date_, &QDateEdit::setEnabled);
    connect(beginning_date_, &QDateEdit::dateChanged, this, &IncomeListWidget::refresh);
    connect(ending_date_, &QDateEdit::dateChanged, this, &IncomeListWidget::refresh);
    connect(filter_by_date_, &QCheckBox::toggled, this, &IncomeListWidget::refresh);

    auto today = QDate::currentDate();
    beginning_date_->setCalendarPopup(true);
    beginning_date_->setDate(QDate(today.year(), today.month(), 1));
    ending_date_->setCalendarPopup(true);
    ending_date_->setDate(QDate(today.year(), today.month(), 1).addMonths(1).addDays(-1));
    filter_by_date_->setText(tr("Enable Date Filter"));
    filter_by_date_->setChecked(true);

    auto params = new QHBoxLayout;
    params->addWidget(new QLabel(tr("Begin")));
    params->addWidget(beginning_date_);
    params->addWidget(new QLabel(tr("End")));
    params->addWidget(ending_date_);
    params->addStretch();
    params->addWidget(filter_by_date_);

    auto side_layout = new QVBoxLayout;
    side_layout->addLayout(params);
    side_layout->addWidget(table_);
    auto side = new QWidget;
    side->setLayout(side_layout);

    addWidget(tree_);
    addWidget(side);

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
void IncomeListWidget::setIncomeFilter(const QModelIndex & current, const QModelIndex & previous) {
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

//--------------------------------------------------------------------------------------------------
void IncomeListWidget::refresh() {
    if (filter_by_date_->isChecked()) {
        incomes_->filterForDates(beginning_date_->date(), ending_date_->date());
    } else {
        incomes_->filterForDates(QDate(), QDate());
    }
}

} // namespace budget
} // namespace ub
