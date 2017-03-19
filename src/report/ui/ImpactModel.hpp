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
#include <vector>

// Qt include(s)
#include <QAbstractTableModel>
#include <QList>
#include <QModelIndex>
#include <QString>
#include <QVariant>

// UnderBudget include(s)
#include <budget/model/Impact.hpp>

namespace ub {
namespace report {

/**
 * Budgeted impacts view model.
 *
 * @ingroup report
 */
class ImpactModel : public QAbstractTableModel {
    Q_OBJECT

public:
    /** Impact date column */
    static const int DATE = 0;
    /** Impact type column */
    static const int TYPE = 1;
    /** Impact category column */
    static const int CATEGORY = 2;
    /** Impact description column */
    static const int DESCRIPTION = 3;
    /** Impact amount column */
    static const int AMOUNT = 4;

    /**
     * Initializes the impact view model.
     */
    ImpactModel();

    /**
     * Updates the impacts to be represented by the model.
     *
     * @param impacts Impacts list
     */
    void setImpacts(const std::vector<budget::Impact> & impacts);

    int columnCount(const QModelIndex & parent) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    int rowCount(const QModelIndex & parent) const override;

    QVariant data(const QModelIndex & index, int role) const override;

private:
    /** Header titles */
    QList<QString> headers_;
    /** Impacts */
    std::vector<budget::Impact> impacts_;
};

} // report namespace
} // ub namespace
