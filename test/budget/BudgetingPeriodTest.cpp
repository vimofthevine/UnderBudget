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
#include "BudgetingPeriodTest.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::BudgetingPeriodTest)

namespace ub {

//------------------------------------------------------------------------------
void BudgetingPeriodTest::dateWithinRange_data()
{
	QTest::addColumn<QDate>("start");
	QTest::addColumn<QDate>("end");
	QTest::addColumn<QDate>("date");
	QTest::addColumn<bool>("result");

	QTest::newRow("too-early") << QDate(2013, 5, 1) << QDate(2013, 5, 31)
		<< QDate(2013, 4, 30) << false;
	QTest::newRow("start-date") << QDate(2013, 5, 1) << QDate(2013, 5, 31)
		<< QDate(2013, 5, 1) << true;
	QTest::newRow("mid-range") << QDate(2013, 5, 1) << QDate(2013, 5, 31)
		<< QDate(2013, 5, 15) << true;
	QTest::newRow("end-date") << QDate(2013, 5, 1) << QDate(2013, 5, 31)
		<< QDate(2013, 5, 31) << true;
	QTest::newRow("too-late") << QDate(2013, 5, 1) << QDate(2013, 5, 31)
		<< QDate(2013, 6, 1) << false;
}

//------------------------------------------------------------------------------
void BudgetingPeriodTest::dateWithinRange()
{
	QFETCH(QDate, start);
	QFETCH(QDate, end);
	QFETCH(QDate, date);
	QFETCH(bool, result);

	BudgetingPeriod::Parameters params;
	params.type = BudgetingPeriod::CustomDateRange;
	params.param1 = start;
	params.param2 = end;
	BudgetingPeriod period(params);

	QCOMPARE(period.contains(date), result);
}

//------------------------------------------------------------------------------
void BudgetingPeriodTest::calendarYear()
{
	BudgetingPeriod::Parameters params;
	params.type = BudgetingPeriod::CalendarYear;
	params.param1 = 2013;
	BudgetingPeriod period(params);

	QCOMPARE(period.startDate(), QDate(2013, 1, 1));
	QCOMPARE(period.endDate(), QDate(2013, 12, 31));
}

//------------------------------------------------------------------------------
void BudgetingPeriodTest::calendarMonth()
{
	BudgetingPeriod::Parameters params;
	params.type = BudgetingPeriod::CalendarMonth;
	params.param1 = 2013;
	params.param2 = 5;
	BudgetingPeriod period(params);

	QCOMPARE(period.startDate(), QDate(2013, 5, 1));
	QCOMPARE(period.endDate(), QDate(2013, 5, 31));
}

//------------------------------------------------------------------------------
void BudgetingPeriodTest::customRange()
{
	BudgetingPeriod::Parameters params;
	params.type = BudgetingPeriod::CustomDateRange;
	params.param1 = QDate(2013, 5, 4);
	params.param2 = QDate(2013, 6, 4);
	BudgetingPeriod period(params);

	QCOMPARE(period.startDate(), QDate(2013, 5, 4));
	QCOMPARE(period.endDate(), QDate(2013, 6, 4));
}

//------------------------------------------------------------------------------
void BudgetingPeriodTest::paydateMonth_data()
{
	QTest::addColumn<int>("year");
	QTest::addColumn<int>("month");
	QTest::addColumn<QDate>("start");
	QTest::addColumn<QDate>("end");

	QTest::newRow("jan-2013") << 2013 << 1
		<< QDate(2013, 1, 4) << QDate(2013, 1, 31);
	QTest::newRow("feb-2013") << 2013 << 2
		<< QDate(2013, 2, 1) << QDate(2013, 2, 28);
	QTest::newRow("mar-2013") << 2013 << 3
		<< QDate(2013, 3, 1) << QDate(2013, 4, 4);
	QTest::newRow("apr-2013") << 2013 << 4
		<< QDate(2013, 4, 5) << QDate(2013, 5, 2);
	QTest::newRow("may-2013") << 2013 << 5
		<< QDate(2013, 5, 3) << QDate(2013, 6, 6);
	QTest::newRow("jun-2013") << 2013 << 6
		<< QDate(2013, 6, 7) << QDate(2013, 7, 4);
	QTest::newRow("jul-2013") << 2013 << 7
		<< QDate(2013, 7, 5) << QDate(2013, 8, 1);
	QTest::newRow("aug-2013") << 2013 << 8
		<< QDate(2013, 8, 2) << QDate(2013, 9, 5);
	QTest::newRow("sep-2013") << 2013 << 9
		<< QDate(2013, 9, 6) << QDate(2013, 10, 3);
	QTest::newRow("oct-2013") << 2013 << 10
		<< QDate(2013, 10, 4) << QDate(2013, 10, 31);
	QTest::newRow("nov-2013") << 2013 << 11
		<< QDate(2013, 11, 1) << QDate(2013, 12, 5);
	QTest::newRow("dec-2013") << 2013 << 12
		<< QDate(2013, 12, 6) << QDate(2014, 1, 2);
}

//------------------------------------------------------------------------------
void BudgetingPeriodTest::paydateMonth()
{
	QFETCH(int, year);
	QFETCH(int, month);
	QFETCH(QDate, start);
	QFETCH(QDate, end);

	BudgetingPeriod::Parameters params;
	params.type = BudgetingPeriod::PaydateMonth;
	params.param1 = year;
	params.param2 = month;
	BudgetingPeriod period(params);

	QCOMPARE(period.startDate(), start);
	QCOMPARE(period.endDate(), end);
}

}

