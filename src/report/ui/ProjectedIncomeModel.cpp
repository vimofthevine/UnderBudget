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
#include <vector>

// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountRepository.hpp>
#include <budget/model/Impact.hpp>
#include <ledger/model/LedgerRepository.hpp>
#include <ledger/ui/AccountModel.hpp>
#include "ProjectedIncomeModel.hpp"

namespace ub {
namespace report {

//--------------------------------------------------------------------------------------------------
ProjectedIncomeModel::ProjectedIncomeModel() {
    headers_ << tr("Name") << tr("Amount");
}

//--------------------------------------------------------------------------------------------------
ledger::Account ProjectedIncomeModel::account(const QModelIndex & index) const {
    if (not accounts_ or not index.isValid()) {
        return ledger::Account();
    }

    return accounts_->getAccount(index.internalId());
}


//--------------------------------------------------------------------------------------------------
void ProjectedIncomeModel::setImpacts(const std::vector<budget::Impact> & impacts) {
    beginResetModel();
    impacts_.clear();
    for (auto & impact : impacts) {
        if (impact.type() == budget::Impact::Income) {
            impacts_[impact.categoryID()] += impact.amount();
        }
    }
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
void ProjectedIncomeModel::setRepository(std::shared_ptr<ledger::LedgerRepository> repository) {
    beginResetModel();
    accounts_ = repository->accounts();
    AccountModel::setRepository(repository);
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
int ProjectedIncomeModel::columnCount(const QModelIndex & parent) const {
    return headers_.size();
}

//--------------------------------------------------------------------------------------------------
QVariant ProjectedIncomeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal and role == Qt::DisplayRole) {
        return headers_.at(section);
    }
    return QVariant();
}

//--------------------------------------------------------------------------------------------------
QVariant ProjectedIncomeModel::data(const QModelIndex & index, int role) const {
    if (not accounts_ or not index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    auto account = accounts_->getAccount(index.internalId());
    if (account.id() == -1) {
        emit error(accounts_->lastError());
        return QVariant();
    }

    switch (index.column()) {
    case NAME:
        return account.name();
    case IMPACT:
        if (account.children().size() == 0u) {
            auto iter = impacts_.find(account.id());
            if (iter != impacts_.end()) {
                return iter->second.toString();
            }
        }
        return QVariant();
    default:
        return QVariant();
    }
}

} // report namespace
} // ub namespace
