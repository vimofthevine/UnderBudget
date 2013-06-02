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

// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "budget/BudgetingPeriod.hpp"

namespace ub {

//------------------------------------------------------------------------------
void BudgetingPeriod::calculateLiteralYearDates()
{
	int year = periodParameters.param1.toInt();
	periodStartDate = QDate(year, 1, 1); // Jan 1st
	periodEndDate = QDate(year, 12, 31); // Dec 31st
}

//------------------------------------------------------------------------------
void BudgetingPeriod::calculateLiteralMonthDates()
{
	int year = periodParameters.param1.toInt();
	int month = periodParameters.param2.toInt();
	periodStartDate = QDate(year, month, 1);
	periodEndDate = QDate(year, month, periodStartDate.daysInMonth());
}

//------------------------------------------------------------------------------
void BudgetingPeriod::calculatePaydateMonthDates()
{
	int year = periodParameters.param1.toInt();
	int month = periodParameters.param2.toInt();

	// Find first friday of the given month
	QDate firstFriday(year, month, 1);
	while (firstFriday.dayOfWeek() != Qt::Friday)
	{
		firstFriday = firstFriday.addDays(1);
	}
	periodStartDate = firstFriday;

	// Find first friday of the next month
	firstFriday = QDate(year, month, 1).addMonths(1);
	while (firstFriday.dayOfWeek() != Qt::Friday)
	{
		firstFriday = firstFriday.addDays(1);
	}
	periodEndDate = firstFriday.addDays(-1);
}

//------------------------------------------------------------------------------
void BudgetingPeriod::calculateCustomDates()
{
	periodStartDate = periodParameters.param1.toDate();
	periodEndDate = periodParameters.param2.toDate();
}

}

