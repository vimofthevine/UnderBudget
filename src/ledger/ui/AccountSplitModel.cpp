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
int AccountSplitModel::columnCount(const QModelIndex &parent) const {
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
QVariant AccountSplitModel::data(const QModelIndex &index, int role) const {
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
