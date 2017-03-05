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
#include "accounting/Money.hpp"
#include "budget/conditions.hpp"
#include "ConditionsTest.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::ConditionsTest);

namespace ub {

//------------------------------------------------------------------------------
void ConditionsTest::dateOperators_data()
{
	QTest::addColumn<QDate>("date");
	QTest::addColumn<AssignmentRule::Operator>("oper");
	QTest::addColumn<QString>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("invalid-value") << QDate(2013, 12, 16)
		<< AssignmentRule::DateEquals << QString("text") << false;
	QTest::newRow("invalid-oper") << QDate(2013, 12, 16)
		<< AssignmentRule::LessThan << QString("2013-12-17") << false;

	QTest::newRow("before/qualifies") << QDate(2013, 12, 16)
		<< AssignmentRule::Before << QString("2013-12-31") << true;
	QTest::newRow("before/fails-exact") << QDate(2013, 12, 16)
		<< AssignmentRule::Before << QString("2013-12-16") << false;
	QTest::newRow("before/fails-after") << QDate(2013, 12, 31)
		<< AssignmentRule::Before << QString("2013-12-16") << false;
	QTest::newRow("before/invalid-date") << QDate()
		<< AssignmentRule::Before << QString("2013-12-16") << false;

	QTest::newRow("after/qualifies") << QDate(2013, 12, 16)
		<< AssignmentRule::After << QString("2013-12-01") << true;
	QTest::newRow("after/fails-exact") << QDate(2013, 12, 16)
		<< AssignmentRule::After << QString("2013-12-16") << false;
	QTest::newRow("after/fails-before") << QDate(2013, 12, 16)
		<< AssignmentRule::After << QString("2013-12-31") << false;
	QTest::newRow("after/invalid-date") << QDate()
		<< AssignmentRule::After << QString("2013-12-16") << false;

	QTest::newRow("equals/qualifies") << QDate(2013, 12, 16)
		<< AssignmentRule::DateEquals << QString("2013-12-16") << true;
	QTest::newRow("equals/fails-before") << QDate(2013, 12, 13)
		<< AssignmentRule::DateEquals << QString("2013-12-16") << false;
	QTest::newRow("equals/fails-after") << QDate(2013, 12, 18)
		<< AssignmentRule::DateEquals << QString("2013-12-16") << false;
	QTest::newRow("equals/invalid-date") << QDate()
		<< AssignmentRule::DateEquals << QString("2013-12-16") << false;
}

//------------------------------------------------------------------------------
void ConditionsTest::dateOperators()
{
	QFETCH(QDate, date);
	QFETCH(AssignmentRule::Operator, oper);
	QFETCH(QString, value);
	QFETCH(bool, result);

	AssignmentRule::Condition condition;
	condition.op = oper;
	condition.value = value;

	QCOMPARE(qualifies(date, condition), result);
}

//------------------------------------------------------------------------------
void ConditionsTest::moneyOperators_data()
{
	QTest::addColumn<Money>("amount");
	QTest::addColumn<AssignmentRule::Operator>("oper");
	QTest::addColumn<QString>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("invalid/no-iso4217") << Money(12, "USD")
		<< AssignmentRule::AmountEquals << QString("12") << false;
	QTest::newRow("invalid/not-number") << Money(12, "USD")
		<< AssignmentRule::AmountEquals << QString("$12,USD") << false;
	QTest::newRow("invalid/opertor") << Money(12, "USD")
		<< AssignmentRule::Before << QString("12,USD") << false;

	QTest::newRow("lt/less") << Money(12, "USD")
		<< AssignmentRule::LessThan << QString("12.10,USD") << true;
	QTest::newRow("lt/exact") << Money(12.1, "USD")
		<< AssignmentRule::LessThan << QString("12.10,USD") << false;
	QTest::newRow("lt/greater") << Money(12.3, "USD")
		<< AssignmentRule::LessThan << QString("12.10,USD") << false;

	QTest::newRow("lte/less") << Money(12, "USD")
		<< AssignmentRule::LessThanOrEqual << QString("12.10,USD") << true;
	QTest::newRow("lte/exact") << Money(12.1, "USD")
		<< AssignmentRule::LessThanOrEqual << QString("12.10,USD") << true;
	QTest::newRow("lte/greater") << Money(12.2, "USD")
		<< AssignmentRule::LessThanOrEqual << QString("12.10,USD") << false;

	QTest::newRow("gt/less") << Money(12, "USD")
		<< AssignmentRule::GreaterThan << QString("12.10,USD") << false;
	QTest::newRow("gt/exact") << Money(12.1, "USD")
		<< AssignmentRule::GreaterThan << QString("12.10,USD") << false;
	QTest::newRow("gt/greater") << Money(12.2, "USD")
		<< AssignmentRule::GreaterThan << QString("12.10,USD") << true;

	QTest::newRow("gte/less") << Money(12, "USD")
		<< AssignmentRule::GreaterThanOrEqual << QString("12.10,USD") << false;
	QTest::newRow("gte/exact") << Money(12.1, "USD")
		<< AssignmentRule::GreaterThanOrEqual << QString("12.10,USD") << true;
	QTest::newRow("gte/greater") << Money(12.2, "USD")
		<< AssignmentRule::GreaterThanOrEqual << QString("12.10,USD") << true;

	QTest::newRow("equal/less") << Money(12, "USD")
		<< AssignmentRule::AmountEquals << QString("12.10,USD") << false;
	QTest::newRow("equal/exact") << Money(12.1, "USD")
		<< AssignmentRule::AmountEquals << QString("12.10,USD") << true;
	QTest::newRow("equal/greater") << Money(12.2, "USD")
		<< AssignmentRule::AmountEquals << QString("12.10,USD") << false;
}

//------------------------------------------------------------------------------
void ConditionsTest::moneyOperators()
{
	QFETCH(Money, amount);
	QFETCH(AssignmentRule::Operator, oper);
	QFETCH(QString, value);
	QFETCH(bool, result);

	AssignmentRule::Condition condition;
	condition.op = oper;
	condition.value = value;

	QCOMPARE(qualifies(amount, condition), result);
}

//------------------------------------------------------------------------------
void ConditionsTest::stringOperators_data()
{
	QTest::addColumn<QString>("string");
	QTest::addColumn<AssignmentRule::Operator>("oper");
	QTest::addColumn<bool>("sensitive");
	QTest::addColumn<QString>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("invalid-oper") << QString("Text")
		<< AssignmentRule::DateEquals << false << QString("Text") << false;

	QTest::newRow("begins/diff-text") << QString("My Text String")
		<< AssignmentRule::BeginsWith << false << QString("tex") << false;
	QTest::newRow("begins/wrong-case") << QString("Text String")
		<< AssignmentRule::BeginsWith << true << QString("tex") << false;
	QTest::newRow("begins/same-case") << QString("Text String")
		<< AssignmentRule::BeginsWith << true << QString("Tex") << true;
	QTest::newRow("begins/case-insensitive") << QString("Text String")
		<< AssignmentRule::BeginsWith << false << QString("tex") << true;

	QTest::newRow("ends/diff-text") << QString("Text String Value")
		<< AssignmentRule::EndsWith << false << QString("ring") << false;
	QTest::newRow("ends/wrong-case") << QString("Text String")
		<< AssignmentRule::EndsWith << true << QString("rinG") << false;
	QTest::newRow("ends/same-case") << QString("Text StrinG")
		<< AssignmentRule::EndsWith << true << QString("rinG") << true;
	QTest::newRow("ends/case-insensitive") << QString("Text String")
		<< AssignmentRule::EndsWith << false << QString("rinG") << true;

	QTest::newRow("contains/diff-text") << QString("Text String")
		<< AssignmentRule::Contains << false << QString("Test") << false;
	QTest::newRow("contains/wrong-case") << QString("Text String")
		<< AssignmentRule::Contains << true << QString("t s") << false;
	QTest::newRow("contains/same-case") << QString("Text String")
		<< AssignmentRule::Contains << true << QString("t S") << true;
	QTest::newRow("contains/case-insensitive") << QString("Text String")
		<< AssignmentRule::Contains << false << QString("t s") << true;

	QTest::newRow("equals/diff-text") << QString("Text String")
		<< AssignmentRule::StringEquals << false << QString("Test String") << false;
	QTest::newRow("equals/wrong-case") << QString("Text String")
		<< AssignmentRule::StringEquals << true << QString("text string") << false;
	QTest::newRow("equals/same-case") << QString("Text String")
		<< AssignmentRule::StringEquals << true << QString("Text String") << true;
	QTest::newRow("equals/case-insensitive") << QString("Text String")
		<< AssignmentRule::StringEquals << false << QString("text string") << true;
}

//------------------------------------------------------------------------------
void ConditionsTest::stringOperators()
{
	QFETCH(QString, string);
	QFETCH(AssignmentRule::Operator, oper);
	QFETCH(bool, sensitive);
	QFETCH(QString, value);
	QFETCH(bool, result);

	AssignmentRule::Condition condition;
	condition.op = oper;
	condition.sensitive = sensitive;
	condition.value = value;

	QCOMPARE(qualifies(string, condition), result);
}

}

