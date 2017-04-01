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
