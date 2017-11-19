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
ExpenseListWidget::ExpenseListWidget(ledger::EnvelopeModel * envelopes, ExpenseModel * expenses,
                                     QWidget * parent)
        : QSplitter(Qt::Horizontal, parent), envelopes_(envelopes), expenses_(expenses),
          envelope_details_(new ledger::EnvelopeDetailsDialog(envelopes_, parent)),
          expense_details_(new ExpenseDetailsDialog(expenses_, parent)),
          beginning_date_(new QDateEdit(this)), ending_date_(new QDateEdit(this)),
          filter_by_date_(new QCheckBox(this)), tree_(new ledger::TreeView(this)),
          table_(new BudgetTableView(this)) {
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
    connect(table_, &BudgetTableView::deleteItem, this, &ExpenseListWidget::deleteExpense);

    connect(filter_by_date_, &QCheckBox::toggled, beginning_date_, &QDateEdit::setEnabled);
    connect(filter_by_date_, &QCheckBox::toggled, ending_date_, &QDateEdit::setEnabled);
    connect(beginning_date_, &QDateEdit::dateChanged, this, &ExpenseListWidget::refresh);
    connect(ending_date_, &QDateEdit::dateChanged, this, &ExpenseListWidget::refresh);
    connect(filter_by_date_, &QCheckBox::toggled, this, &ExpenseListWidget::refresh);

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

    // Give expense list stretch priority
    setStretchFactor(1, 1);
}

//--------------------------------------------------------------------------------------------------
void ExpenseListWidget::showEnvelope(const ledger::Envelope & envelope) {
    tree_->select(envelopes_->index(envelope));
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
                                         const QModelIndex & /* previous */) {
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

//--------------------------------------------------------------------------------------------------
void ExpenseListWidget::refresh() {
    if (filter_by_date_->isChecked()) {
        expenses_->filterForDates(beginning_date_->date(), ending_date_->date());
    } else {
        expenses_->filterForDates(QDate(), QDate());
    }
}

} // budget namespace
} // ub namespace
