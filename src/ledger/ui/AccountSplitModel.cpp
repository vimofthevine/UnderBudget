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

// Standard include(s)
#include <memory>

// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountTransaction.hpp>
#include <ledger/model/JournalEntry.hpp>
#include "AccountSplitModel.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountSplitModel::AccountSplitModel(QObject * parent) : QAbstractTableModel(parent) {
    headers_ << tr("Account") << tr("Amount") << tr("Memo") << tr("Cleared");
}

//--------------------------------------------------------------------------------------------------
void AccountSplitModel::setJournalEntry(std::shared_ptr<JournalEntry> entry) {
    beginResetModel();
    entry_ = entry;
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
int AccountSplitModel::columnCount(const QModelIndex & parent) const {
    return headers_.size();
}

//--------------------------------------------------------------------------------------------------
QVariant AccountSplitModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if ((orientation == Qt::Horizontal) and (role == Qt::DisplayRole)) {
        return headers_.at(section);
    }
    return QVariant();
}

//--------------------------------------------------------------------------------------------------
QVariant AccountSplitModel::data(const QModelIndex & index, int role) const {
    if ((not entry_) or (not index.isValid()) or (role != Qt::DisplayRole)) {
        return QVariant();
    }

    auto splits = entry_->getAccountSplits();
    int row = index.row();
    if ((row < 0) or (row >= splits.size())) {
        return QVariant();
    }

    auto split = splits.at(row);
    switch (index.column()) {
    case 0:
        return split.account().name();
    case 1:
        return split.amount().toString();
    case 2:
        return split.memo();
    case 3:
        return split.isCleared();
    default:
        return QVariant();
    }
}

//--------------------------------------------------------------------------------------------------
int AccountSplitModel::rowCount(const QModelIndex & parent) const {
    if ((not entry_) or (parent.column() > 0)) {
        return 0;
    }
    return entry_->getAccountSplits().size();
}

} // ledger namespace
} // ub namespace
