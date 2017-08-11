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
#include <ledger/model/Envelope.hpp>
#include <ledger/model/EnvelopeTransaction.hpp>
#include <ledger/model/Money.hpp>
#include <ledger/ui/EnvelopeModel.hpp>

namespace ub {

namespace ledger {
class EnvelopeRepository;
class LedgerRepository;
}

namespace report {

class ProjectedExpenseModel : public ledger::EnvelopeModel {
    Q_OBJECT

public:
    /** Projected impact column */
    static const int PROJECTED = 1;
    /** Actual expense column */
    static const int ACTUAL = 2;
    /** Difference column */
    static const int DIFFERENCE = 3;

    /**
     * Initializes the projected expense view model.
     */
    ProjectedExpenseModel();

    /**
     * Retrieves the envelope represented by the given model index.
     *
     * @param index Model index
     * @return Envelope located at the given index
     */
    ledger::Envelope envelope(const QModelIndex & index) const;

    /**
     * Resets the model for new data.
     */
    void reset();

    /**
     * Updates the projected impacts to be represented by the model.
     *
     * @param impacts Projected impacts list
     */
    void setProjectedImpacts(const std::vector<budget::Impact> & impacts);

    /**
     * Updates the actual expenses to be represented by the model.
     *
     * @param expenses Actual expenses list
     */
    void setActualExpenses(const std::vector<ledger::EnvelopeTransaction> & expenses);

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
    /** Projected impact per envelope */
    std::map<int64_t, ledger::Money> projected_;
    /** Actual impact per envelope */
    std::map<int64_t, ledger::Money> actual_;
    /** Differences per envelope */
    std::map<int64_t, ledger::Money> difference_;
};

} // report namespace
} // ub namespace
