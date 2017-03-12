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
#include <budget/model/BudgetRepository.hpp>
#include <budget/model/Income.hpp>
#include <budget/model/IncomeRepository.hpp>
#include <ledger/model/Account.hpp>
#include "IncomeModel.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
IncomeModel::IncomeModel() {
    headers_ << tr("Description") << tr("Starts") << tr("Stops") << tr("Recurs") << tr("Amount");
}

//--------------------------------------------------------------------------------------------------
void IncomeModel::setRepository(std::shared_ptr<BudgetRepository> repository) {
    beginResetModel();
    account_ = ledger::Account();
    incomes_ = repository->incomes();
    cache_.clear();
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
Income IncomeModel::income(const QModelIndex & index) {
    if (not incomes_) {
        return Income();
    }

    int row = index.row();
    if ((row < 0) or (row >= cache_.size())) {
        return Income();
    }

    return cache_.at(row);
}

//--------------------------------------------------------------------------------------------------
bool IncomeModel::create(const Income & income) {
    if (not incomes_) {
        return false;
    }

    beginResetModel();
    auto id = incomes_->create(income);
    cache_ = incomes_->incomes(account_);
    endResetModel();

    if (id <= 0) {
        emit error(incomes_->lastError());
    }
}

//--------------------------------------------------------------------------------------------------
bool IncomeModel::update(const Income & income, const QModelIndex & index) {
    if (not incomes_ or not index.isValid()) {
        return false;
    }

    beginResetModel();
    bool success = incomes_->update(income);
    cache_ = incomes_->incomes(account_);
    endResetModel();

    if (not success) {
        emit error(incomes_->lastError());
    }

    return success;
}

//--------------------------------------------------------------------------------------------------
bool IncomeModel::remove(const QModelIndex & index) {
    if (not incomes_ or not index.isValid()) {
        return false;
    }

    beginResetModel();
    bool success = incomes_->remove(this->income(index));
    cache_ = incomes_->incomes(account_);
    endResetModel();

    if (not success) {
        emit error(incomes_->lastError());
    }

    return success;
}

//--------------------------------------------------------------------------------------------------
void IncomeModel::filterForAccount(const ledger::Account & account) {
    beginResetModel();
    account_ = account;
    if (incomes_) {
        cache_ = incomes_->incomes(account);
    }
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
void IncomeModel::refresh() {
    filterForAccount(account_);
}

//--------------------------------------------------------------------------------------------------
int IncomeModel::columnCount(const QModelIndex & parent) const {
    return headers_.size();
}

//--------------------------------------------------------------------------------------------------
QVariant IncomeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if ((orientation == Qt::Horizontal) and (role == Qt::DisplayRole)) {
        return headers_.at(section);
    }
    return QVariant();
}

//--------------------------------------------------------------------------------------------------
QVariant IncomeModel::data(const QModelIndex & index, int role) const {
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

    auto income = cache_.at(row);
    switch (index.column()) {
    case DESCRIPTION:
        return income.description();
    case BEGINNING_DATE:
        return income.beginningDate().toString("M/d/yy");
    case ENDING_DATE:
        return (income.endingDate().isValid() ? income.endingDate().toString("M/dd/yy")
                                               : tr("Never"));
    case RECURRENCE:
        return income.recurrence().toString();
    case AMOUNT:
        return (role == Qt::DisplayRole) ? income.amount().toString()
                                         : QVariant::fromValue(income.amount());
    default:
        return QVariant();
    }
}

//--------------------------------------------------------------------------------------------------
int IncomeModel::rowCount(const QModelIndex & parent) const {
    if (parent.column() > 0) {
        return 0;
    }

    return cache_.size();
}

} // budget namespace
} // ub namespace
