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

#pragma once

// Standard include(s)
#include <cstdint>
#include <map>
#include <memory>
#include <vector>

// Qt include(s)
#include <QAbstractItemModel>

// UnderBudget include(s)
#include <budget/model/Impact.hpp>
#include <ledger/model/Money.hpp>
#include <ledger/ui/EnvelopeModel.hpp>

namespace ub {

namespace ledger {
class EnvelopeRepository;
class LedgerRepository;
}

namespace report {

class CondensedImpactModel : public ledger::EnvelopeModel {
    Q_OBJECT

public:
    /** Envelope budgeted impact column */
    static const int IMPACT = 1;

    /**
     * Initializes the condensed impact view model.
     */
    CondensedImpactModel();

    /**
     * Updates the impacts to be represented by the model.
     *
     * @param impacts Impacts list
     */
    void setImpacts(const std::vector<budget::Impact> & impacts);

    /**
     * Updates the repositories used by the model.
     *
     * @param repository Ledger repository
     */
    void setRepository(std::shared_ptr<ledger::LedgerRepository> repository);

    int columnCount(const QModelIndex & parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QVariant data(const QModelIndex & index, int role) const override;

private:
    /** Header titles */
    QList<QString> headers_;
    /** Envelope repository */
    std::shared_ptr<ledger::EnvelopeRepository> envelopes_;
    /** Impacts per envelope */
    std::map<int64_t, ledger::Money> impacts_;
};

} // report namespace
} // ub namespace
