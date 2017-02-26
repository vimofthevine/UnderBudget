// Standard include(s)
#include <algorithm>
#include <memory>

// Qt include(s)
#include <QtCore>
#include <QtWidgets>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountRepository.hpp>
#include <ledger/model/LedgerRepository.hpp>
#include <ledger/model/TransactionRepository.hpp>
#include "AccountModel.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountModel::AccountModel() {
    headers_ << tr("Name") << tr("Balance");
}

//--------------------------------------------------------------------------------------------------
void AccountModel::setRepository(std::shared_ptr<LedgerRepository> repository) {
    beginResetModel();
    accounts_ = repository->accounts();
    transactions_ = repository->transactions();
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
Account AccountModel::account(const QModelIndex &index) const {
    if (not accounts_) {
        return Account();
    }

    return index.isValid() ?  accounts_->getAccount(index.internalId())
        : accounts_->getRoot();
}

//--------------------------------------------------------------------------------------------------
bool AccountModel::create(const Account &account, const QModelIndex &parent) {
    if (not accounts_) {
        return false;
    }

    int rows = rowCount(parent);
    beginInsertRows(parent, rows, rows);
    int id = accounts_->create(account, this->account(parent));
    endInsertRows();

    if (id <= 0) {
        emit error(accounts_->lastError());
    }

    return (id > 0);
}

//--------------------------------------------------------------------------------------------------
bool AccountModel::update(const Account &account, const QModelIndex &index) {
    if (not accounts_) {
        return false;
    }

    bool success = accounts_->update(account);
    if (success) {
        emit dataChanged(
            this->index(index.row(), 0, index.parent()),
            this->index(index.row(), columnCount(index) - 1, index.parent()));
    } else {
        emit error(accounts_->lastError());
    }

    return success;
}

//--------------------------------------------------------------------------------------------------
bool AccountModel::remove(const QModelIndex &index) {
    if (not accounts_) {
        return false;
    }

    beginRemoveRows(index.parent(), index.row(), index.row());
    bool success = accounts_->remove(this->account(index));
    endRemoveRows();

    if (not success) {
        emit error(accounts_->lastError());
    }

    return success;
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
    if (not accounts_ or not index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    Account account = accounts_->getAccount(index.internalId());
    if (account.id() == -1) {
        emit error(accounts_->lastError());
        return QVariant();
    }

    switch (index.column()) {
    case NAME:
        return account.name();
    case BALANCE:
        if (account.children().size() > 0u) {
            return QVariant();
        } else {
            return transactions_->getBalance(QDate::currentDate(), account).toString();
        }
    default:
        return QVariant();
    }
}

//--------------------------------------------------------------------------------------------------
int AccountModel::rowCount(const QModelIndex &parent) const {
    if (not accounts_ or parent.column() > 0) {
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
    if (not accounts_ or not hasIndex(row, column, parent)) {
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
    if (not accounts_ or not child.isValid()) {
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
