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

// Standard include(s)
#include <memory>

// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include <ledger/model/Envelope.hpp>
#include <ledger/model/EnvelopeTransaction.hpp>
#include <ledger/model/JournalEntry.hpp>
#include "EnvelopeSplitModel.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
EnvelopeSplitModel::EnvelopeSplitModel(QObject * parent) : QAbstractTableModel(parent) {
    headers_ << tr("Envelope") << tr("Amount") << tr("Memo");
}

//--------------------------------------------------------------------------------------------------
void EnvelopeSplitModel::setJournalEntry(std::shared_ptr<JournalEntry> entry) {
    beginResetModel();
    entry_ = entry;
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
int EnvelopeSplitModel::columnCount(const QModelIndex & parent) const {
    return headers_.size();
}

//--------------------------------------------------------------------------------------------------
QVariant EnvelopeSplitModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if ((orientation == Qt::Horizontal) and (role == Qt::DisplayRole)) {
        return headers_.at(section);
    }
    return QVariant();
}

//--------------------------------------------------------------------------------------------------
QVariant EnvelopeSplitModel::data(const QModelIndex & index, int role) const {
    if ((not entry_) or (not index.isValid()) or (role != Qt::DisplayRole)) {
        return QVariant();
    }

    auto splits = entry_->getEnvelopeSplits();
    int row = index.row();
    if ((row < 0) or (row >= splits.size())) {
        return QVariant();
    }

    auto split = splits.at(row);
    switch (index.column()) {
    case 0:
        return split.envelope().name();
    case 1:
        return split.amount().toString();
    case 2:
        return split.memo();
    default:
        return QVariant();
    }
}

//--------------------------------------------------------------------------------------------------
int EnvelopeSplitModel::rowCount(const QModelIndex & parent) const {
    if ((not entry_) or (parent.column() > 0)) {
        return 0;
    }
    return entry_->getEnvelopeSplits().size();
}

} // ledger namespace
} // ub namespace
