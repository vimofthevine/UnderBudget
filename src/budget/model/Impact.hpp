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

#pragma once

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

private:
    /** Impact amount (negative amounts are debits) */
    ledger::Money amount_;
    /** Impact category */
    QString category_;
    /** Impact date */
    QDate date_;
    /** Impact description */
    QString description_;
};

} // budget namespace
} // ub namespace
