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

// Qt include(s)
#include <QtCore>
#include <QtTest>

// UnderBudget include(s)
#include <ledger/model/Currency.hpp>
#include "CurrencyTest.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::ledger::CurrencyTest)

namespace ub {
namespace ledger {

//------------------------------------------------------------------------------
void CurrencyTest::defaultConstructorShouldUseCurrentLocale_data() {
    QTest::addColumn<QLocale>("locale");
    QTest::addColumn<Currency>("currency");

    QTest::newRow("en-US") << QLocale(QLocale::English, QLocale::UnitedStates)
        << Currency(0, "USD");
    QTest::newRow("uk-UA") << QLocale(QLocale::Ukrainian, QLocale::Ukraine)
        << Currency(0, "UAH");
    QTest::newRow("de-DE") << QLocale(QLocale::German, QLocale::Germany)
        << Currency(0, "EUR");
}

//------------------------------------------------------------------------------
void CurrencyTest::defaultConstructorShouldUseCurrentLocale() {
    QFETCH(QLocale, locale);
    QFETCH(Currency, currency);

    QLocale::setDefault(locale);
    Currency actual;
    QCOMPARE(actual, currency);
}

//------------------------------------------------------------------------------
void CurrencyTest::shouldFormatValuesWithAppropriateCurrencySymbol_data() {
    QTest::addColumn<QLocale>("locale");
    QTest::addColumn<Currency>("currency");
    QTest::addColumn<double>("value");
    QTest::addColumn<QString>("result");

    QTest::newRow("en-US_usd_short")
        << QLocale(QLocale::English, QLocale::UnitedStates) << Currency(0, "USD")
        << 4.0 << "$4.00";
    QTest::newRow("en-US_usd_long")
        << QLocale(QLocale::English, QLocale::UnitedStates) << Currency(0, "USD")
        << 1234.0 << "$1,234.00";
    QTest::newRow("en-US_usd_negative")
        << QLocale(QLocale::English, QLocale::UnitedStates) << Currency(0, "USD")
        << -234.32 << "$-234.32";
    QTest::newRow("en-US_uah")
        << QLocale(QLocale::English, QLocale::UnitedStates) << Currency(0, "UAH")
        << 1234.0 << QChar(8372) + QString("1,234.00");
    QTest::newRow("uk-UA_uah_short")
        << QLocale(QLocale::Ukrainian, QLocale::Ukraine) << Currency(0, "UAH")
        << 4.0 << QString::fromUtf8("4,00\u00A0\u20B4");
    QTest::newRow("uk-UA_uah_long")
        << QLocale(QLocale::Ukrainian, QLocale::Ukraine) << Currency(0, "UAH")
        << 1234.0 << QString::fromUtf8("1\u00A0234,00\u00A0\u20B4");
    QTest::newRow("uk-UA_uah_negative")
        << QLocale(QLocale::Ukrainian, QLocale::Ukraine) << Currency(0, "UAH")
        << -234.32 << QString::fromUtf8("-234,32\u00A0\u20B4");
    QTest::newRow("uk-UA_usd")
        << QLocale(QLocale::Ukrainian, QLocale::Ukraine) << Currency(0, "USD")
        << 1234.0 << "1\u00A0234,00\u00A0$";
}

//------------------------------------------------------------------------------
void CurrencyTest::shouldFormatValuesWithAppropriateCurrencySymbol() {
    QFETCH(QLocale, locale);
    QFETCH(Currency, currency);
    QFETCH(double, value);
    QFETCH(QString, result);

    QLocale::setDefault(locale);
    QCOMPARE(currency.format(value), result);
}

} // ledger namespace
} // ub namespace
