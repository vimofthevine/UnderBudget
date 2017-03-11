/*
 * Copyright 2016 Kyle Treubig
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
