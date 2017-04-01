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
#include <QMetaType>
#include <QString>

namespace ub {
namespace ledger {

/**
 * Model of a single monetary currency.
 *
 * @ingroup ledger
 */
class Currency {
public:
    /**
     * Initializes a currency for the current locale.
     */
    Currency();

    /**
     * Initializes a currency with the given ID and ISO 4217 code.
     *
     * @param[in] id   Currency ID
     * @param[in] code ISO 4217 currency code
     */
    Currency(int64_t id, const QString & code);

    /**
     * Initializes a currency with the given ISO 4217 code.
     *
     * This allows for literal strings to be used easily.
     *
     * @param[in] code ISO 4217 currency code
     */
    // Currency(const QString & code);

    // Copy constructor
    Currency(const Currency & orig) = default;

    // Assignment operator
    Currency & operator=(const Currency & orig) = default;

    /**
     * Determines if the given currency and this currency are equivalent.
     *
     * @param[in] that Currency to be checked for equality
     * @return @c true if the currencies are the same
     */
    bool operator==(const Currency & that) const;

    /**
     * Determines if the given currency and this currency are not equivalent.
     *
     * @param[in] that Currency to be checked for inequality
     * @return @c true if the currencies are different
     */
    bool operator!=(const Currency & that) const;

    /**
     * Returns the ISO 4217 code for this currency.
     *
     * @return ISO 4217 code
     */
    QString code() const;

    /**
     * Formats the given value according to this currency and the current locale.
     *
     * @param[in] value Decimal value to be formatted
     */
    QString format(double value) const;

    /**
     * Returns the ID of this currency.
     *
     * @return Currency ID
     */
    int64_t id() const;

    /**
     * Returns the UTF symbol for this currency.
     *
     * @return UTF symbol
     */
    QString symbol() const;

private:
    /** Currency ID */
    int64_t id_;
    /** ISO 4217 currency code */
    QString iso4217_;
};

} // ledger namespace
} // ub namespace

// Make Currency known to QMetaType
Q_DECLARE_METATYPE(ub::ledger::Currency)
