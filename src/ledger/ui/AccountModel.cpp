// Standard include(s)
#include <algorithm>
#include <memory>

// Qt include(s)
#include <QtCore>
#include <QtWidgets>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountRepository.hpp>
#include <ledger/model/TransactionRepository.hpp>
#include "AccountModel.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountModel::AccountModel(std::shared_ptr<AccountRepository> accounts,
                           std::shared_ptr<TransactionRepository> transactions)
        : accounts_(accounts), transactions_(transactions) {
    headers_ << tr("Name") << tr("Balance");
}

//--------------------------------------------------------------------------------------------------
int AccountModel::columnCount(const QModelIndex &parent) const {
    return headers_.size();
}

//--------------------------------------------------------------------------------------------------
QVariant AccountModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal and role == Qt::DisplayRole) {
        return headers_.at(section);
    }
    return QVariant();
}

//--------------------------------------------------------------------------------------------------
QVariant AccountModel::data(const QModelIndex &index, int role) const {
    if (not index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    Account account = accounts_->getAccount(index.internalId());
    if (account.id() == -1) {
        return QVariant();
    }

    switch (index.column()) {
    case 0:
        return account.name();
    case 1:
        if (account.children().size() > 0u) {
            return QVariant();
        } else {
            return transactions_->getBalance(QDate(), account).toString();
        }
    default:
        return QVariant();
    }
}

//--------------------------------------------------------------------------------------------------
int AccountModel::rowCount(const QModelIndex &parent) const {
    if (parent.column() > 0) {
        return 0;
    }

    if (not parent.isValid()) {
        return accounts_->getRoot().children().size();
    } else {
        return accounts_->getAccount(parent.internalId()).children().size();
    }
}

//--------------------------------------------------------------------------------------------------
QModelIndex AccountModel::index(int row, int column, const QModelIndex &parent) const {
    if (not hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    auto children = parent.isValid() ? accounts_->getAccount(parent.internalId()).children()
                                     : accounts_->getRoot().children();
    if (row < 0 or row >= children.size()) {
        return QModelIndex();
    }
    auto child = children.at(row);

    if (child == -1) {
        return QModelIndex();
    } else {
        return createIndex(row, column, child);
    }
}

//--------------------------------------------------------------------------------------------------
QModelIndex AccountModel::parent(const QModelIndex &child) const {
    if (not child.isValid()) {
        return QModelIndex();
    }

    Account c_acct = accounts_->getAccount(child.internalId());
    if (c_acct.id() == -1) {
        return QModelIndex();
    }

    Account p_acct = accounts_->getAccount(c_acct.parent());
    if (p_acct.id() == -1) {
        return QModelIndex();
    }

    Account g_acct = accounts_->getAccount(p_acct.parent());
    if (g_acct.id() == -1) {
        return QModelIndex();
    }

    auto siblings = g_acct.children();
    auto iter = std::find(siblings.begin(), siblings.end(), p_acct.id());
    return createIndex(std::distance(siblings.begin(), iter), 0, p_acct.id());
}

} // ledger namespace
} // ub namespace