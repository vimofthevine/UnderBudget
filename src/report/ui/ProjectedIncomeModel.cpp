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
#include <budget/model/Impact.hpp>
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountRepository.hpp>
#include <ledger/model/AccountTransaction.hpp>
#include <ledger/model/LedgerRepository.hpp>
#include <ledger/ui/AccountModel.hpp>
#include "ProjectedIncomeModel.hpp"

namespace ub {
namespace report {

//--------------------------------------------------------------------------------------------------
ProjectedIncomeModel::ProjectedIncomeModel() {
    headers_ << tr("Account") << tr("Projected") << tr("Actual") << tr("Difference");
}

//--------------------------------------------------------------------------------------------------
ledger::Account ProjectedIncomeModel::account(const QModelIndex & index) const {
    if (not accounts_ or not index.isValid()) {
        return ledger::Account();
    }

    return accounts_->getAccount(index.internalId());
}

//--------------------------------------------------------------------------------------------------
void ProjectedIncomeModel::reset() {
    difference_.clear();
}

//--------------------------------------------------------------------------------------------------
void ProjectedIncomeModel::setActualIncomes(
    const std::vector<ledger::AccountTransaction> & incomes) {
    beginResetModel();
    actual_.clear();
    for (auto & income : incomes) {
        if (not income.amount().isNegative()) {
            auto id = income.account().id();
            actual_[id] = income.amount();
            difference_[id] = projected_[id] - actual_[id];
        }
    }
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
void ProjectedIncomeModel::setProjectedImpacts(const std::vector<budget::Impact> & impacts) {
    beginResetModel();
    projected_.clear();
    for (auto & impact : impacts) {
        if (impact.type() == budget::Impact::Income) {
            auto id = impact.categoryID();
            projected_[id] += impact.amount();
            difference_[id] = projected_[id] - actual_[id];
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
QVariant ProjectedIncomeModel::headerData(int section, Qt::Orientation orientation,
                                          int role) const {
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

    auto account = accounts_->getAccount(index.internalId());
    if (account.id() == -1) {
        emit error(accounts_->lastError());
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case NAME:
            return account.name();
        case PROJECTED:
            if (account.children().empty()) {
                auto iter = projected_.find(account.id());
                if (iter != projected_.end()) {
                    return iter->second.toString();
                }
            }
            return QVariant();
        case ACTUAL:
            if (account.children().empty()) {
                auto iter = actual_.find(account.id());
                if (iter != actual_.end()) {
                    return iter->second.toString();
                }
            }
            return QVariant();
        case DIFFERENCE:
            if (account.children().empty()) {
                auto iter = difference_.find(account.id());
                if (iter != difference_.end()) {
                    return iter->second.toString();
                }
            }
            return QVariant();
        default:
            return QVariant();
        }
    }

    if ((role == Qt::ForegroundRole) and (index.column() == DIFFERENCE)) {
        if (account.children().empty()) {
            auto iter = difference_.find(account.id());
            if ((iter != difference_.end()) and (not iter->second.isNegative())) {
                return QColor(Qt::red);
            }
        }
    }

    return QVariant();
}

} // report namespace
} // ub namespace
