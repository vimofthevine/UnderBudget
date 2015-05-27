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

#ifndef GNUCASHREADERTEST_HPP
#define GNUCASHREADERTEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

namespace ub {

/**
 * Unit test for the GnuCashReader class.
 */
class GnuCashReaderTest : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Tests successful transaction import.
	 */
	void success();

	/**
	 * Test data for successful transaction import.
	 */
	void success_data();

	/**
	 * Tests importing with no date filters.
	 */
	void noDateFilters();

	/**
	 * Tests importing with date filters.
	 */
	void withDateFilters();

	/**
	 * Tests reading with errors in the XML.
	 */
	void errors();

	/**
	 * Test data for reading with errors.
	 */
	void errors_data();
};

}

#endif //GNUCASHREADERTEST_HPP
