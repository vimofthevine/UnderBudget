// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/LedgerRepository.hpp>
#include <ledger/model/TransactionRepository.hpp>
#include "AccountTransactionModel.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountTransactionModel::AccountTransactionModel() {
    headers_ << tr("Date") << tr("Payee") << tr("Memo") << tr("Amount");
}

//--------------------------------------------------------------------------------------------------
void AccountTransactionModel::setRepository(std::shared_ptr<LedgerRepository> repository) {
    beginResetModel();
    account_ = Account();
    transactions_ = repository->transactions();
    cache_.clear();
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
void AccountTransactionModel::filterForAccount(const Account &account) {
    beginResetModel();
    account_ = account;
    if (transactions_) {
        cache_ = transactions_->getTransactions(account);
    }
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
int AccountTransactionModel::columnCount(const QModelIndex &parent) const {
    return headers_.size();
}

//--------------------------------------------------------------------------------------------------
QVariant AccountTransactionModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if ((orientation == Qt::Horizontal) and (role == Qt::DisplayRole)) {
        return headers_.at(section);
    }
    return QVariant();
}

//--------------------------------------------------------------------------------------------------
QVariant AccountTransactionModel::data(const QModelIndex &index, int role) const {
    if (not index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    int row = index.row();
    if ((row < 0) or (row >= cache_.size())) {
        return QVariant();
    }

    auto transaction = cache_.at(row);
    switch (index.column()) {
    case DATE:
        return transaction.transaction().date();
    case PAYEE:
        return transaction.transaction().payee();
    case MEMO:
        return transaction.memo();
    case AMOUNT:
        return transaction.amount().toString();
    default:
        return QVariant();
    }
}

//--------------------------------------------------------------------------------------------------
int AccountTransactionModel::rowCount(const QModelIndex &parent) const {
    if (parent.column() > 0) {
        return 0;
    }

    return cache_.size();
}

//--------------------------------------------------------------------------------------------------
Qt::ItemFlags AccountTransactionModel::flags(const QModelIndex &index) const {
    return QAbstractTableModel::flags(index);
}

} // ledger namespace
} // ub namespace
