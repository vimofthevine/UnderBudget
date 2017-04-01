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

#ifndef ACCOUNTTEST_HPP
#define ACCOUNTTEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

namespace ub {

/**
 * Unit tests for the Account class.
 */
class AccountTest : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Tests the creation of a full name for an account
	 */
	void fullName();

	/**
	 * Test data for full name test
	 */
	void fullName_data();

	/**
	 * Tests equality comparison of accounts
	 */
	void equality();

	/**
	 * Test data for equality comparison
	 */
	void equality_data();
};

}

#endif //ACCOUNTTEST_HPP
