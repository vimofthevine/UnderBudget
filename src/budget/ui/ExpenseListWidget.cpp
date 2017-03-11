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
#include "ExpenseListWidget.hpp"
#include "ExpenseModel.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
ExpenseListWidget::ExpenseListWidget(ledger::EnvelopeModel * envelopes,
                                     ExpenseModel * expenses, QWidget * parent)
        : QSplitter(Qt::Horizontal, parent), envelopes_(envelopes), expenses_(expenses),
          details_(new ledger::EnvelopeDetailsDialog(envelopes_, parent)),
          tree_(new ledger::TreeView(this)), table_(new BudgetTableView(this)) {
    tree_->setModel(envelopes_);
    table_->setModel(expenses_);

    details_->hide();
    details_->setModal(true);

    connect(tree_, &ledger::TreeView::selectItem, this, &ExpenseListWidget::selectEnvelope);
    connect(tree_, &ledger::TreeView::selectItem, this, &ExpenseListWidget::setExpenseFilter);
    connect(tree_, &ledger::TreeView::createItem, details_,
            &ledger::EnvelopeDetailsDialog::resetForNewEnvelope);
    connect(tree_, &ledger::TreeView::modifyItem, details_,
            &ledger::EnvelopeDetailsDialog::showEnvelope);
    connect(tree_, &ledger::TreeView::deleteItem, this, &ExpenseListWidget::deleteEnvelope);

    connect(table_, &BudgetTableView::modifyItem, this,
            &ExpenseListWidget::modifyExpense);
    connect(table_, &BudgetTableView::duplicateItem, this,
            &ExpenseListWidget::duplicateExpense);
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
    auto acct = envelopes_->envelope(current);
    expenses_->filterForEnvelope(acct);
}

//--------------------------------------------------------------------------------------------------
void ExpenseListWidget::modifyExpense(const QModelIndex & index) {
    qDebug() << "modify expense";
}

//--------------------------------------------------------------------------------------------------
void ExpenseListWidget::duplicateExpense(const QModelIndex & index) {
    qDebug() << "duplicate expense";
}

//--------------------------------------------------------------------------------------------------
void ExpenseListWidget::deleteExpense(const QModelIndex & index) {
    qDebug() << "delete expense";
}

} // budget namespace
} // ub namespace
