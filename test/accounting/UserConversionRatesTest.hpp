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

#ifndef USERCONVERSIONRATESTEST_HPP
#define USERCONVERSIONRATESTEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

// Forward declaration(s)
class UserConversionRates;

namespace ub {

/**
 * Unit tests for the UserConversionRates class.
 */
class UserConversionRatesTest : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Creates an in-memory conversion rates instance,
	 * populating it with test data.
	 */
	void init();

	/**
	 * Destroys the in-memory conversion rates instance.
	 */
	void cleanup();

	/**
	 * Tests the lookup of conversion rates from the database.
	 */
	void lookup();

	/**
	 * Test data for conversion rate lookup.
	 */
	void lookup_data();

	/**
	 * Tests the addition of new conversion rates to the database.
	 */
	void addRates();

	/**
	 * Tests the removal of conversion rates from the database.
	 */
	void removeRates();

	/**
	 * Tests the opening of a different conversion rates source file.
	 */
	void open();

private:
	UserConversionRates* rates;
};

}

#endif //USERCONVERSIONRATESTEST_HPP
