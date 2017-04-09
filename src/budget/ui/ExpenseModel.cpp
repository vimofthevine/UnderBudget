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
#include <budget/model/BudgetRepository.hpp>
#include <budget/model/Expense.hpp>
#include <budget/model/ExpenseRepository.hpp>
#include <ledger/model/Envelope.hpp>
#include "ExpenseModel.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
ExpenseModel::ExpenseModel() {
    headers_ << tr("Description") << tr("Starts") << tr("Stops") << tr("Recurs") << tr("Amount");
}

//--------------------------------------------------------------------------------------------------
void ExpenseModel::setRepository(std::shared_ptr<BudgetRepository> repository) {
    beginResetModel();
    envelope_ = ledger::Envelope();
    expenses_ = repository->expenses();
    cache_.clear();
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
Expense ExpenseModel::expense(const QModelIndex & index) {
    if (not expenses_) {
        return Expense();
    }

    int row = index.row();
    if ((row < 0) or (row >= cache_.size())) {
        return Expense();
    }

    return cache_.at(row);
}

//--------------------------------------------------------------------------------------------------
bool ExpenseModel::create(const Expense & expense) {
    if (not expenses_) {
        return false;
    }

    beginResetModel();
    auto id = expenses_->create(expense);
    cache_ = expenses_->expenses(envelope_);
    endResetModel();

    if (id <= 0) {
        emit error(expenses_->lastError());
    }
}

//--------------------------------------------------------------------------------------------------
bool ExpenseModel::update(const Expense & expense, const QModelIndex & index) {
    if (not expenses_ or not index.isValid()) {
        return false;
    }

    beginResetModel();
    bool success = expenses_->update(expense);
    cache_ = expenses_->expenses(envelope_);
    endResetModel();

    if (not success) {
        emit error(expenses_->lastError());
    }

    return success;
}

//--------------------------------------------------------------------------------------------------
bool ExpenseModel::remove(const QModelIndex & index) {
    if (not expenses_ or not index.isValid()) {
        return false;
    }

    beginResetModel();
    bool success = expenses_->remove(this->expense(index));
    cache_ = expenses_->expenses(envelope_);
    endResetModel();

    if (not success) {
        emit error(expenses_->lastError());
    }

    return success;
}

//--------------------------------------------------------------------------------------------------
void ExpenseModel::filterForEnvelope(const ledger::Envelope & envelope) {
    beginResetModel();
    envelope_ = envelope;
    if (expenses_) {
        cache_ = expenses_->expenses(envelope);
    }
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
void ExpenseModel::refresh() {
    filterForEnvelope(envelope_);
}

//--------------------------------------------------------------------------------------------------
int ExpenseModel::columnCount(const QModelIndex & parent) const {
    return headers_.size();
}

//--------------------------------------------------------------------------------------------------
QVariant ExpenseModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if ((orientation == Qt::Horizontal) and (role == Qt::DisplayRole)) {
        return headers_.at(section);
    }
    return QVariant();
}

//--------------------------------------------------------------------------------------------------
QVariant ExpenseModel::data(const QModelIndex & index, int role) const {
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

    auto expense = cache_.at(row);
    switch (index.column()) {
    case DESCRIPTION:
        return expense.description();
    case BEGINNING_DATE:
        return expense.beginningDate();
    case ENDING_DATE:
        return expense.endingDate();
    case RECURRENCE:
        return expense.recurrence().toString();
    case AMOUNT:
        return (role == Qt::DisplayRole) ? expense.amount().toString()
                                         : QVariant::fromValue(expense.amount());
    default:
        return QVariant();
    }
}

//--------------------------------------------------------------------------------------------------
int ExpenseModel::rowCount(const QModelIndex & parent) const {
    if (parent.column() > 0) {
        return 0;
    }

    return cache_.size();
}

} // budget namespace
} // ub namespace
