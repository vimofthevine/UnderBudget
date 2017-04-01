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
#include <ledger/model/Account.hpp>
#include <ledger/model/LedgerRepository.hpp>
#include <ledger/model/TransactionRepository.hpp>
#include "AccountTransactionModel.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountTransactionModel::AccountTransactionModel() {
    headers_ << tr("Date") << tr("Payee") << tr("Memo") << tr("Amount") << tr("Balance");
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
Transaction AccountTransactionModel::transaction(const QModelIndex & index) {
    if (not transactions_) {
        return Transaction();
    }

    int row = index.row();
    if ((row < 0) or (row >= cache_.size())) {
        return Transaction();
    }

    return cache_.at(row).transaction();
}

//--------------------------------------------------------------------------------------------------
void AccountTransactionModel::filterForAccount(const Account & account) {
    beginResetModel();
    account_ = account;
    if (transactions_) {
        cache_ = transactions_->getTransactions(account);
    }
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
void AccountTransactionModel::refresh() {
    filterForAccount(account_);
}

//--------------------------------------------------------------------------------------------------
int AccountTransactionModel::columnCount(const QModelIndex & parent) const {
    return headers_.size();
}

//--------------------------------------------------------------------------------------------------
QVariant AccountTransactionModel::headerData(int section, Qt::Orientation orientation,
                                             int role) const {
    if ((orientation == Qt::Horizontal) and (role == Qt::DisplayRole)) {
        return headers_.at(section);
    }
    return QVariant();
}

//--------------------------------------------------------------------------------------------------
QVariant AccountTransactionModel::data(const QModelIndex & index, int role) const {
    if (not index.isValid()) {
        return QVariant();
    }

    if ((role != Qt::DisplayRole) and (role != Qt::EditRole)) {
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
        return (role == Qt::DisplayRole) ? transaction.amount().toString()
                                         : QVariant::fromValue(transaction.amount());
    case BALANCE:
        return (role == Qt::DisplayRole) ? transaction.balance().toString()
                                         : QVariant::fromValue(transaction.balance());
    default:
        return QVariant();
    }
}

//--------------------------------------------------------------------------------------------------
int AccountTransactionModel::rowCount(const QModelIndex & parent) const {
    if (parent.column() > 0) {
        return 0;
    }

    return cache_.size();
}

//--------------------------------------------------------------------------------------------------
Qt::ItemFlags AccountTransactionModel::flags(const QModelIndex & index) const {
    return QAbstractTableModel::flags(index);
}

} // ledger namespace
} // ub namespace
