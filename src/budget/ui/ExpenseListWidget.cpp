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
#include <ledger/ui/EnvelopeDetailsDialog.hpp>
#include <ledger/ui/EnvelopeModel.hpp>
#include <ledger/ui/TreeView.hpp>
#include "BudgetTableView.hpp"
#include "ExpenseDetailsDialog.hpp"
#include "ExpenseListWidget.hpp"
#include "ExpenseModel.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
ExpenseListWidget::ExpenseListWidget(ledger::EnvelopeModel * envelopes,
                                     ExpenseModel * expenses, QWidget * parent)
        : QSplitter(Qt::Horizontal, parent), envelopes_(envelopes), expenses_(expenses),
          envelope_details_(new ledger::EnvelopeDetailsDialog(envelopes_, parent)),
          expense_details_(new ExpenseDetailsDialog(expenses_, parent)),
          tree_(new ledger::TreeView(this)), table_(new BudgetTableView(this)) {
    tree_->setModel(envelopes_);
    table_->setModel(expenses_);

    envelope_details_->hide();
    envelope_details_->setModal(true);

    expense_details_->hide();
    expense_details_->setModal(true);

    connect(tree_, &ledger::TreeView::selectItem, this, &ExpenseListWidget::selectEnvelope);
    connect(tree_, &ledger::TreeView::selectItem, this, &ExpenseListWidget::setExpenseFilter);
    connect(tree_, &ledger::TreeView::createItem, envelope_details_,
            &ledger::EnvelopeDetailsDialog::resetForNewEnvelope);
    connect(tree_, &ledger::TreeView::modifyItem, envelope_details_,
            &ledger::EnvelopeDetailsDialog::showEnvelope);
    connect(tree_, &ledger::TreeView::deleteItem, this, &ExpenseListWidget::deleteEnvelope);

    connect(table_, &BudgetTableView::createItem, this, &ExpenseListWidget::createExpense);
    connect(table_, &BudgetTableView::modifyItem, expense_details_,
            &ExpenseDetailsDialog::showExpense);
    connect(table_, &BudgetTableView::duplicateItem, expense_details_,
            &ExpenseDetailsDialog::duplicateExpense);
    connect(table_, &BudgetTableView::deleteItem, this,
            &ExpenseListWidget::deleteExpense);

    addWidget(tree_);
    addWidget(table_);

    // Give expense list stretch priority
    setStretchFactor(1, 1);
}

//--------------------------------------------------------------------------------------------------
void ExpenseListWidget::deleteEnvelope(const QModelIndex & index) {
    ledger::Envelope envelope = envelopes_->envelope(index);
    auto answer =
        QMessageBox::question(this->parentWidget(), tr("Delete Envelope?"),
                              tr("Are you sure you want to delete %0?").arg(envelope.name()));
    if (answer == QMessageBox::Yes) {
        envelopes_->remove(index);
    }
}

//--------------------------------------------------------------------------------------------------
void ExpenseListWidget::setExpenseFilter(const QModelIndex & current,
                                              const QModelIndex & previous) {
    envelope_ = envelopes_->envelope(current);
    expenses_->filterForEnvelope(envelope_);
}

//--------------------------------------------------------------------------------------------------
void ExpenseListWidget::createExpense() {
    if (envelope_.id() > 0) {
        expense_details_->resetForNewExpense(envelope_);
    }
}

//--------------------------------------------------------------------------------------------------
void ExpenseListWidget::deleteExpense(const QModelIndex & index) {
    Expense expense = expenses_->expense(index);
    auto answer =
            QMessageBox::question(this->parentWidget(), tr("Delete Budgeted Expense?"),
                                  tr("Are you sure you want to delete %0?").arg(expense.description()));
    if (answer == QMessageBox::Yes) {
        expenses_->remove(index);
    }
}

} // budget namespace
} // ub namespace
