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

    connect(tree_, &TreeView::selectItem, this, &EnvelopeListWidget::selectEnvelope);
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
