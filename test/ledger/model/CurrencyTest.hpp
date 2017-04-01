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

// Qt include(s)
#include <QtTest/QtTest>

namespace ub {
namespace ledger {

/** Unit test for the Currency class. */
class CurrencyTest : public QObject {
    Q_OBJECT

private slots:
    /** Verifies that the default constructor creates a currency for the current locale */
    void defaultConstructorShouldUseCurrentLocale();

    /** Sets up test data for the default constructor test */
    void defaultConstructorShouldUseCurrentLocale_data();

    /** Verifies that values are formatted correctly according to the currency type */
    void shouldFormatValuesWithAppropriateCurrencySymbol();

    /** Sets up test data for the format test */
    void shouldFormatValuesWithAppropriateCurrencySymbol_data();
};

} // ledger namespace
} // ub namespace
