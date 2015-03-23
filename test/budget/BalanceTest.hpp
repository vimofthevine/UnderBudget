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

#ifndef BALANCETEST_HPP
#define BALANCETEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

// UnderBudget include(s)
#include "budget/Balance.hpp"

namespace ub {

/**
 * Unit tests for the Balance class.
 */
class BalanceTest : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Tests contributor counts of balances.
	 */
	void contributorCount();

	/**
	 * Tests contributor retrieval of balances.
	 */
	void contributorAt();

	/**
	 * Tests values of balances.
	 */
	void value();

	/**
	 * Tests creating of balances from a single money value
	 */
	void createFromMoney();

	/**
	 * Tests adding of contributors to a balance with no contributors.
	 */
	void addContributorToBalanceWithNone();

	/**
	 * Tests adding of contributors to a balance with one contributor.
	 */
	void addContributorToBalanceWithOne();

	/**
	 * Tests adding of contributors to a balance with many contributors.
	 */
	void addContributorToBalanceWithMany();

	/**
	 * Tests removing of contributors from a balance with no contributors.
	 */
	void removeContributorFromBalanceWithNone();

	/**
	 * Tests removing of contributors from a balance with one contributor.
	 */
	void removeContributorFromBalanceWithOne();

	/**
	 * Tests removing of contributors from a balance with many contributors.
	 */
	void removeContributorFromBalanceWithMany();

	/**
	 * Tests updating of a contributor for a balance.
	 */
	void updateContributor();
};

}

#endif //BALANCETEST_HPP
