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
#include <budget/model/Impact.hpp>
#include "ImpactModel.hpp"

namespace ub {
namespace report {

//--------------------------------------------------------------------------------------------------
ImpactModel::ImpactModel() {
    headers_ << tr("Date") << tr("Type") << tr("Category") << tr("Description") << tr("Amount");
}

//--------------------------------------------------------------------------------------------------
void ImpactModel::setImpacts(const std::vector<budget::Impact> & impacts) {
    beginResetModel();
    impacts_ = impacts;
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
int ImpactModel::columnCount(const QModelIndex & parent) const {
    return headers_.size();
}

//--------------------------------------------------------------------------------------------------
QVariant ImpactModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if ((orientation == Qt::Horizontal) and (role == Qt::DisplayRole)) {
        return headers_.at(section);
    }
    return QVariant();
}

//--------------------------------------------------------------------------------------------------
QVariant ImpactModel::data(const QModelIndex & index, int role) const {
    if (not index.isValid()) {
        return QVariant();
    }

    if ((role != Qt::DisplayRole) and (role != Qt::EditRole)) {
        return QVariant();
    }

    int row = index.row();
    if ((row < 0) or (row >= impacts_.size())) {
        return QVariant();
    }

    auto impact = impacts_.at(row);
    switch (index.column()) {
    case DATE:
        return impact.date();
    case TYPE:
        return (impact.type() == budget::Impact::Expense) ? tr("Expense") : tr("Income");
    case CATEGORY:
        return impact.category();
    case DESCRIPTION:
        return impact.description();
    case AMOUNT:
        return (impact.type() == budget::Impact::Expense)
                ? (-impact.amount()).toString()
                : impact.amount().toString();
    default:
        return QVariant();
    }
}

//--------------------------------------------------------------------------------------------------
int ImpactModel::rowCount(const QModelIndex & parent) const {
    if (parent.column() > 0) {
        return 0;
    }

    return impacts_.size();
}

} // report namespace
} // ub namespace
