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
#include "EnvelopeDetailsDialog.hpp"
#include "EnvelopeListWidget.hpp"
#include "EnvelopeModel.hpp"
#include "EnvelopeTransactionModel.hpp"
#include "TransactionTableView.hpp"
#include "TreeView.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
EnvelopeListWidget::EnvelopeListWidget(EnvelopeModel * model,
                                       EnvelopeTransactionModel * transactions, QWidget * parent)
        : QSplitter(Qt::Horizontal, parent), model_(model), transactions_(transactions),
          details_(new EnvelopeDetailsDialog(model_, parent)), tree_(new TreeView(this)),
          transaction_list_(new TransactionTableView(this)) {
    tree_->setModel(model_);
    transaction_list_->setModel(transactions_);

    details_->hide();
    details_->setModal(true);

    connect(tree_, &TreeView::selectItem, this, &EnvelopeListWidget::setTransactionFilter);
    connect(tree_, &TreeView::createItem, details_, &EnvelopeDetailsDialog::resetForNewEnvelope);
    connect(tree_, &TreeView::modifyItem, details_, &EnvelopeDetailsDialog::showEnvelope);
    connect(tree_, &TreeView::deleteItem, this, &EnvelopeListWidget::deleteEnvelope);

    connect(transaction_list_, &TransactionTableView::modifyItem, this,
            &EnvelopeListWidget::modifyEnvelopeTransaction);
    connect(transaction_list_, &TransactionTableView::duplicateItem, this,
            &EnvelopeListWidget::duplicateEnvelopeTransaction);
    connect(transaction_list_, &TransactionTableView::deleteItem, this,
            &EnvelopeListWidget::deleteEnvelopeTransaction);

    addWidget(tree_);
    addWidget(transaction_list_);

    // Give transaction list stretch priority
    setStretchFactor(1, 1);
}

//--------------------------------------------------------------------------------------------------
void EnvelopeListWidget::deleteEnvelope(const QModelIndex & index) {
    Envelope envelope = model_->envelope(index);
    auto answer =
        QMessageBox::question(this->parentWidget(), tr("Delete Envelope?"),
                              tr("Are you sure you want to delete %0?").arg(envelope.name()));
    if (answer == QMessageBox::Yes) {
        model_->remove(index);
    }
}

//--------------------------------------------------------------------------------------------------
void EnvelopeListWidget::setTransactionFilter(const QModelIndex & current,
                                              const QModelIndex & previous) {
    auto acct = model_->envelope(current);
    transactions_->filterForEnvelope(acct);
    emit envelopeSelected(model_->envelope(current));
}

//--------------------------------------------------------------------------------------------------
void EnvelopeListWidget::modifyEnvelopeTransaction(const QModelIndex & index) {
    emit modifyTransaction(transactions_->transaction(index));
}

//--------------------------------------------------------------------------------------------------
void EnvelopeListWidget::duplicateEnvelopeTransaction(const QModelIndex & index) {
    emit duplicateTransaction(transactions_->transaction(index));
}

//--------------------------------------------------------------------------------------------------
void EnvelopeListWidget::deleteEnvelopeTransaction(const QModelIndex & index) {
    emit deleteTransaction(transactions_->transaction(index));
}

} // ledger namespace
} // ub namespace
