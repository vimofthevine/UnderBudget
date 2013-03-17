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
#include "accounting/UserConversionRates.hpp"
#include "UserConversionRatesTest.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::UserConversionRatesTest)

namespace ub {

//------------------------------------------------------------------------------
void UserConversionRatesTest::init()
{
	UserConversionRates::open(":memory:");
	rates = new UserConversionRates;

	// these values were correct as of 3/21/2013 (from google.com)
	rates->add("USD", "UAH", 8.14);
	rates->add("UAH", "USD", 0.12);
	rates->add("USD", "EUR", 0.77);
	rates->add("EUR", "USD", 1.29);
	rates->add("UAH", "EUR", 0.10);
	rates->add("EUR", "UAH", 10.51);
}

//------------------------------------------------------------------------------
void UserConversionRatesTest::cleanup()
{
	delete rates;
}

//------------------------------------------------------------------------------
void UserConversionRatesTest::lookup_data()
{
	QTest::addColumn<QString>("source");
	QTest::addColumn<QString>("target");
	QTest::addColumn<double>("rate");

	QTest::newRow("eur-to-usd") << "EUR" << "USD" << 1.29;
	QTest::newRow("usd-to-uah") << "USD" << "UAH" << 8.14;
	QTest::newRow("usd-to-eur") << "USD" << "EUR" << 0.77;
	QTest::newRow("eur-to-uah") << "EUR" << "UAH" << 10.51;
	QTest::newRow("uah-to-eur") << "UAH" << "EUR" << 0.10;
	QTest::newRow("usd-to-gbp") << "USD" << "GBP" << 1.0; // no entry, default
	QTest::newRow("uah-to-usd") << "UAH" << "USD" << 0.12;
}

//------------------------------------------------------------------------------
void UserConversionRatesTest::lookup()
{
	QFETCH(QString, source);
	QFETCH(QString, target);
	QFETCH(double, rate);

	QCOMPARE(rates->get(source, target), rate);
}

//------------------------------------------------------------------------------
void UserConversionRatesTest::addRates()
{
	QCOMPARE(rates->get("USD", "GBP"), 1.0);
	rates->add("USD", "GBP", 0.66);
	QCOMPARE(rates->get("USD", "GBP"), 0.66);
	rates->add("USD", "GBP", 1.52);
	QCOMPARE(rates->get("USD", "GBP"), 1.52);
}

//------------------------------------------------------------------------------
void UserConversionRatesTest::removeRates()
{
	QCOMPARE(rates->get("EUR", "USD"), 1.29);
	rates->remove("EUR", "USD");
	QCOMPARE(rates->get("EUR", "USD"), 1.0);
}

//------------------------------------------------------------------------------
void UserConversionRatesTest::open()
{
	QCOMPARE(rates->get("EUR", "USD"), 1.29);
	rates->open(":memory:");
	// although this pair used to exist, new database doesn't have it
	QCOMPARE(rates->get("EUR", "USD"), 1.0);
}

}

