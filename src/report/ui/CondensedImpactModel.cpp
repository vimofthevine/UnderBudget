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
#include <ledger/model/EnvelopeRepository.hpp>
#include <ledger/model/LedgerRepository.hpp>
#include <ledger/ui/EnvelopeModel.hpp>
#include "CondensedImpactModel.hpp"

namespace ub {
namespace report {

//--------------------------------------------------------------------------------------------------
CondensedImpactModel::CondensedImpactModel() {
    headers_ << tr("Name") << tr("Amount");
}

//--------------------------------------------------------------------------------------------------
void CondensedImpactModel::setImpacts(const std::vector<budget::Impact> & impacts) {
    beginResetModel();
    impacts_.clear();
    for (auto & impact : impacts) {
        if (impact.type() == budget::Impact::Expense) {
            impacts_[impact.categoryID()] += impact.amount();
        }
    }
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
void CondensedImpactModel::setRepository(std::shared_ptr<ledger::LedgerRepository> repository) {
    beginResetModel();
    envelopes_ = repository->envelopes();
    EnvelopeModel::setRepository(repository);
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
int CondensedImpactModel::columnCount(const QModelIndex & parent) const {
    return headers_.size();
}

//--------------------------------------------------------------------------------------------------
QVariant CondensedImpactModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal and role == Qt::DisplayRole) {
        return headers_.at(section);
    }
    return QVariant();
}

//--------------------------------------------------------------------------------------------------
QVariant CondensedImpactModel::data(const QModelIndex & index, int role) const {
    if (not envelopes_ or not index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    auto envelope = envelopes_->getEnvelope(index.internalId());
    if (envelope.id() == -1) {
        emit error(envelopes_->lastError());
        return QVariant();
    }

    switch (index.column()) {
    case NAME:
        return envelope.name();
    case IMPACT:
        if (envelope.children().size() == 0u) {
            auto iter = impacts_.find(envelope.id());
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
