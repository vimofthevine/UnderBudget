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

// Qt include(s)
#include <QDate>
#include <QString>

// UnderBudget include(s)
#include <ledger/model/Money.hpp>

namespace ub {
namespace budget {

/**
 * Model of a single impact of a budgeted expense or income on the cash balance.
 *
 * For a recurring expense or income, each occurrence would equate to an impact instance.
 *
 * @ingroup budget
 */
class Impact {
public:
    /** Type of the impact */
    enum ImpactType {
        /** Budgeted expense type */
        Expense,
        /** Budgeted income type */
        Income
    };

    /**
     * Initializes an impact with default values.
     */
    Impact() = default;

    /**
     * Returns the amount this impact has on the cash balance.
     *
     * @return Amount of impact on cash balance
     */
    ledger::Money amount() const;

    /**
     * Returns the category name to which this impact applies.
     *
     * @return Category to which this impact applies
     */
    QString category() const;

    /**
     * Returns the category ID to which this impact applies.
     *
     * This is the ID of either the account or envelope to which this
     * impact applies.
     *
     * @return Category ID to which this impact applies
     */
    int64_t categoryID() const;

    /**
     * Returns the date on which this impact applies.
     *
     * @return Date on which this impact applies
     */
    QDate date() const;

    /**
     * Returns the description of this impact.
     *
     * @return Description of this impact
     */
    QString description() const;

    /**
     * Updates the amount this impact has on the cash balance.
     *
     * @param amount New amount
     */
    void setAmount(const ledger::Money & amount);

    /**
     * Updates the category name to which this impact applies.
     *
     * @param category New category
     */
    void setCategory(const QString & category);

    /**
     * Updates the category ID to which this impact applies.
     *
     * @param id New category ID
     */
    void setCategoryID(int64_t id);

    /**
     * Updates the date on which this impact applies.
     *
     * @param date New date
     */
    void setDate(const QDate & date);

    /**
     * Updates the description of this impact.
     *
     * @param description New description
     */
    void setDescription(const QString & description);

    /**
     * Updates the type of this impact.
     *
     * @param type New impact type
     */
    void setType(ImpactType type);

    /**
     * Returns the type of this impact.
     *
     * @return Impact type
     */
    ImpactType type() const;

private:
    /** Impact amount (negative amounts are debits) */
    ledger::Money amount_;
    /** Impact category */
    QString category_;
    /** Associated account or envelope ID */
    int64_t category_id_;
    /** Impact date */
    QDate date_;
    /** Impact description */
    QString description_;
    /** Impact type */
    ImpactType type_;
};

} // budget namespace
} // ub namespace
