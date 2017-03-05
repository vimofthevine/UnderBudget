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

#ifndef BUDGETINGPERIODTEST_HPP
#define BUDGETINGPERIODTEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

namespace ub {

/**
 * Unit tests for the BudgetingPeriod class.
 */
class BudgetingPeriodTest : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Tests date-within-range checking
	 */
	void dateWithinRange();

	/**
	 * Test data for date-within-range checking
	 */
	void dateWithinRange_data();

	/**
	 * Tests calendar year periods
	 */
	void calendarYear();

	/**
	 * Tests calendar month periods
	 */
	void calendarMonth();

	/**
	 * Tests custom date range periods
	 */
	void customRange();

	/**
	 * Tests paydate month periods
	 */
	void paydateMonth();

	/**
	 * Test data for paydate month periods
	 */
	void paydateMonth_data();
};

}

#endif //BUDGETINGPERIODTEST_HPP
