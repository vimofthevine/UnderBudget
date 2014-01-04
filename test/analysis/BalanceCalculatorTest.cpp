/*
 * Copyright 2014 Kyle Treubig
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
#include "analysis/Actuals.hpp"
#include "analysis/BalanceCalculator.hpp"
#include "analysis/ProjectedBalance.hpp"
#include "analysis/SortedDifferences.hpp"
#include "budget/Estimate.hpp"
#include "BalanceCalculatorTest.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::BalanceCalculatorTest)

namespace QTest {

//------------------------------------------------------------------------------
template<>
char* toString(const ub::Money& money)
{
	QString str = QString("%1 (%2, %3)").arg(money.toString())
		.arg(QVariant(money.amount()).toString()).arg(money.currency().code());
	return toString(str);
}

}

namespace ub {

//------------------------------------------------------------------------------
static const uint EXPENSES = 111;
static const uint LIVING = 222;
static const uint RENT = 333;
static const uint UTILITIES = 444;
static const uint FOOD = 555;
static const uint SALARY = 666;
static const uint BONUS = 777;

//------------------------------------------------------------------------------
QSharedPointer<Estimate> createEstimates()
{
	QSharedPointer<Estimate> root = Estimate::createRoot();
	Estimate* expenses = Estimate::create(root.data(),
		EXPENSES, "Expenses", "", Estimate::Expense, Money(), QDate(), false);
	Estimate* living = Estimate::create(expenses,
		LIVING, "Living", "", Estimate::Expense, Money(), QDate(), false);
	Estimate::create(living, RENT, "Rent", "",
		Estimate::Expense, Money(10, "USD"), QDate(), false);
	Estimate::create(living, UTILITIES, "Utilities", "",
		Estimate::Expense, Money(5, "USD"), QDate(), false);
	Estimate::create(expenses, FOOD, "Food", "",
		Estimate::Expense, Money(50, "USD"), QDate(), false);
	Estimate::create(root.data(), SALARY, "Salary", "",
		Estimate::Income, Money(100, "USD"), QDate(), false);
	Estimate::create(root.data(), BONUS, "Bonus", "",
		Estimate::Income, Money(50, "USD"), QDate(), false);

	return root;
}

//------------------------------------------------------------------------------
QSharedPointer<Actuals> createActuals()
{
	QSharedPointer<Actuals> actuals(new Actuals);
	actuals->record(RENT, Money(16, "USD"));
	actuals->record(UTILITIES, Money(3, "USD"));
	actuals->record(FOOD, Money(51, "USD"));
	actuals->record(SALARY, Money(97, "USD"));
	actuals->record(BONUS, Money(55, "USD"));
	return actuals;
}

//------------------------------------------------------------------------------
void BalanceCalculatorTest::estimatedBalance()
{
	ProjectedBalance estimated, actual, expected;
	SortedDifferences over, under;
	QSharedPointer<Actuals> actuals = createActuals();
	BalanceCalculator calculator(createEstimates(), actuals.data(),
		&estimated, &actual, &expected, &over, &under);
	calculator.calculateBalances();

	QCOMPARE(estimated.value(), Money(85, "USD"));
}

//------------------------------------------------------------------------------
void BalanceCalculatorTest::actualBalance()
{
	ProjectedBalance estimated, actual, expected;
	SortedDifferences over, under;
	QSharedPointer<Actuals> actuals = createActuals();
	BalanceCalculator calculator(createEstimates(), actuals.data(),
		&estimated, &actual, &expected, &over, &under);
	calculator.calculateBalances();

	QCOMPARE(actual.value(), Money(82, "USD"));
}

//------------------------------------------------------------------------------
void BalanceCalculatorTest::expectedBalance()
{
	ProjectedBalance estimated, actual, expected;
	SortedDifferences over, under;
	QSharedPointer<Actuals> actuals = createActuals();
	BalanceCalculator calculator(createEstimates(), actuals.data(),
		&estimated, &actual, &expected, &over, &under);
	calculator.calculateBalances();

	QCOMPARE(expected.value(), Money(83, "USD"));
}

//------------------------------------------------------------------------------
void BalanceCalculatorTest::overBudgetEstimates()
{
	ProjectedBalance estimated, actual, expected;
	SortedDifferences over, under;
	QSharedPointer<Actuals> actuals = createActuals();
	BalanceCalculator calculator(createEstimates(), actuals.data(),
		&estimated, &actual, &expected, &over, &under);
	calculator.calculateBalances();

	QCOMPARE(over.size(), 3);

	QCOMPARE(over.estimate(0), RENT);
	QCOMPARE(over.difference(0), Money(6, "USD"));

	QCOMPARE(over.estimate(1), SALARY);
	QCOMPARE(over.difference(1), Money(3, "USD"));

	QCOMPARE(over.estimate(2), FOOD);
	QCOMPARE(over.difference(2), Money(1, "USD"));
}

//------------------------------------------------------------------------------
void BalanceCalculatorTest::underBudgetEstimates()
{
	ProjectedBalance estimated, actual, expected;
	SortedDifferences over, under;
	QSharedPointer<Actuals> actuals = createActuals();
	BalanceCalculator calculator(createEstimates(), actuals.data(),
		&estimated, &actual, &expected, &over, &under);
	calculator.calculateBalances();

	QCOMPARE(under.size(), 2);

	QCOMPARE(under.estimate(0), BONUS);
	QCOMPARE(under.difference(0), Money(5, "USD"));

	QCOMPARE(under.estimate(1), UTILITIES);
	QCOMPARE(under.difference(1), Money(2, "USD"));
}

}

