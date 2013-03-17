/*
 * Copyright 2013 Kyle Treubig
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

// UnderBudget include(s)
#include "accounting/Currency.hpp"
#include "CurrencyTest.hpp"
#include "StubConversionRates.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::CurrencyTest)

namespace ub {

//------------------------------------------------------------------------------
void CurrencyTest::emptyConstruction_data()
{
	QTest::addColumn<QLocale>("locale");
	QTest::addColumn<Currency>("currency");

	QTest::newRow("en-US") << QLocale(QLocale::English, QLocale::UnitedStates)
		<< Currency("USD");
	QTest::newRow("uk-UA") << QLocale(QLocale::Ukrainian, QLocale::Ukraine)
		<< Currency("UAH");
	QTest::newRow("de-DE") << QLocale(QLocale::German, QLocale::Germany)
		<< Currency("EUR");
}

//------------------------------------------------------------------------------
void CurrencyTest::emptyConstruction()
{
	QFETCH(QLocale, locale);
	QFETCH(Currency, currency);

	QLocale::setDefault(locale);
	Currency actual;
	QCOMPARE(actual, currency);
}

//------------------------------------------------------------------------------
void CurrencyTest::format_data()
{
	QTest::addColumn<QLocale>("locale");
	QTest::addColumn<Currency>("currency");
	QTest::addColumn<double>("value");
	QTest::addColumn<QString>("result");

	QTest::newRow("en-US_usd_short")
		<< QLocale(QLocale::English, QLocale::UnitedStates) << Currency("USD")
		<< 4.0 << "$4.00";
	QTest::newRow("en-US_usd_long")
		<< QLocale(QLocale::English, QLocale::UnitedStates) << Currency("USD")
		<< 1234.0 << "$1,234.00";
	QTest::newRow("en-US_usd_negative")
		<< QLocale(QLocale::English, QLocale::UnitedStates) << Currency("USD")
		<< -234.32 << "($234.32)";
	QTest::newRow("en-US_uah")
		<< QLocale(QLocale::English, QLocale::UnitedStates) << Currency("UAH")
		<< 1234.0 << "UAH1,234.00";
	QTest::newRow("uk-UA_uah_short")
		<< QLocale(QLocale::Ukrainian, QLocale::Ukraine) << Currency("UAH")
		<< 4.0 << QString::fromUtf8("4,00\u00A0\u20B4");
	QTest::newRow("uk-UA_uah_long")
		<< QLocale(QLocale::Ukrainian, QLocale::Ukraine) << Currency("UAH")
		<< 1234.0 << QString::fromUtf8("1\u00A0234,00\u00A0\u20B4");
	QTest::newRow("uk-UA_uah_negative")
		<< QLocale(QLocale::Ukrainian, QLocale::Ukraine) << Currency("UAH")
		<< -234.32 << QString::fromUtf8("-234,32\u00A0\u20B4");
	QTest::newRow("uk-UA_usd")
		<< QLocale(QLocale::Ukrainian, QLocale::Ukraine) << Currency("USD")
		<< 1234.0 << "1\u00A0234,00\u00A0USD";
}

//------------------------------------------------------------------------------
void CurrencyTest::format()
{
	QFETCH(QLocale, locale);
	QFETCH(Currency, currency);
	QFETCH(double, value);
	QFETCH(QString, result);

	QLocale::setDefault(locale);
	QCOMPARE(currency.format(value), result);
}

//------------------------------------------------------------------------------
void CurrencyTest::conversionRate_data()
{
	QTest::addColumn<Currency>("source");
	QTest::addColumn<Currency>("target");
	QTest::addColumn<QString>("code");
	QTest::addColumn<double>("rate");

	QTest::newRow("rateFound") << Currency("USD") << Currency("UAH")
		<< "UAH" << 8.14;
	QTest::newRow("rateNotFound") << Currency("UAH") << Currency("USD")
		<< "EUR" << 1.0;
}

//------------------------------------------------------------------------------
void CurrencyTest::conversionRate()
{
	QFETCH(Currency, source); // source currency
	QFETCH(Currency, target); // target currency
	QFETCH(QString, code); // target currency code in rates lookup
	QFETCH(double, rate); // expected conversion rate

	StubConversionRates rates(rate, source.code(), code);
	QCOMPARE(source.conversionRate(target, rates), rate);
}

}
