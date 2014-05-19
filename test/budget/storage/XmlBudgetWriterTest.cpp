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
#include "XmlBudgetWriterTest.hpp"
#include "budget/AssignmentRule.hpp"
#include "budget/AssignmentRules.hpp"
#include "budget/Balance.hpp"
#include "budget/Budget.hpp"
#include "budget/Estimate.hpp"
#include "budget/storage/XmlBudgetWriter.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::XmlBudgetWriterTest)

namespace ub {

//------------------------------------------------------------------------------
void XmlBudgetWriterTest::writeCompleteBudget()
{
	// Create estimate tree
	QSharedPointer<Estimate> root = Estimate::createRoot();
	Estimate* income = Estimate::create(root.data(), 10000, "Income Estimates",
		"estimates for incomes", Estimate::Income, Money(), QDate(), false);
	Estimate* salary = Estimate::create(income, 11000, "Someone's Salary",
		"", Estimate::Income, Money(5623, "USD"), QDate(), false);
	Estimate* expense = Estimate::create(root.data(), 20000, "Expense Estimates",
		"", Estimate::Expense, Money(), QDate(), false);
	Estimate* bills = Estimate::create(expense, 21000, "Bills",
		"recurring expenses", Estimate::Expense, Money(), QDate(), false);
	Estimate::create(bills, 21100, "Rent",
		"for the apt.", Estimate::Expense, Money(452.23, "USD"), QDate(2013,6,28), false);
	Estimate::create(bills, 21200, "Utilities",
		"elec, gas, etc.", Estimate::Expense, Money(500, "USD"), QDate(), true);
	Estimate::create(expense, 22000, "Foreign Expenses",
		"while in Europe", Estimate::Expense, Money(2000, "EUR"), QDate(), false);
	Estimate::create(root.data(), 30000, "Credit Card",
		"", Estimate::Transfer, Money(1400, "USD"), QDate(2013,6,21), true);

	// Create assignment rules
	QSharedPointer<AssignmentRules> rules = AssignmentRules::create();
	QList<AssignmentRule::Condition> conditions;

	// payee, begins with
	conditions << AssignmentRule::Condition(AssignmentRule::Payee,
		AssignmentRule::BeginsWith, false, "payee begins with");
	rules->createRule(100, 30000, conditions);
	// memo, ends with (case-sensitive)
	conditions.replace(0, AssignmentRule::Condition(AssignmentRule::Memo,
		AssignmentRule::EndsWith, true, "memo Ends With"));
	rules->createRule(101, 21200, conditions);
	// empty list
	conditions.clear();
	rules->createRule(102, 21100, conditions);
	// date, before and after (multiple conditions
	conditions << AssignmentRule::Condition(AssignmentRule::Date,
		AssignmentRule::After, false, "2013-05-06");
	conditions << AssignmentRule::Condition(AssignmentRule::Date,
		AssignmentRule::Before, false, "2013-05-10");
	rules->createRule(103, 22000, conditions);
	// date, equals
	conditions.clear();
	conditions << AssignmentRule::Condition(AssignmentRule::Date,
		AssignmentRule::DateEquals, false, "2013-05-11");
	rules->createRule(104, 30000, conditions);
	// amount, less than
	conditions.replace(0, AssignmentRule::Condition(AssignmentRule::Amount,
		AssignmentRule::LessThan, false, "3400"));
	rules->createRule(105, 21100, conditions);
	// amount, less than or equal
	conditions.replace(0, AssignmentRule::Condition(AssignmentRule::Amount,
		AssignmentRule::LessThanOrEqual, false, "3500.50"));
	rules->createRule(106, 211000, conditions);
	// amount, greater than
	conditions.replace(0, AssignmentRule::Condition(AssignmentRule::Amount,
		AssignmentRule::GreaterThan, false, "10000"));
	rules->createRule(107, 11000, conditions);
	// amount, greater than or equal
	conditions.replace(0, AssignmentRule::Condition(AssignmentRule::Amount,
		AssignmentRule::GreaterThanOrEqual, false, "12030"));
	rules->createRule(109, 21200, conditions);
	// amount, equals
	conditions.replace(0, AssignmentRule::Condition(AssignmentRule::Amount,
		AssignmentRule::AmountEquals, false, "502.99"));
	rules->createRule(110, 22000, conditions);
	// deposit, contains
	conditions.replace(0, AssignmentRule::Condition(AssignmentRule::DepositAccount,
		AssignmentRule::Contains, false, "deposit contains"));
	rules->createRule(111, 30000, conditions);
	// withdrawal, equals (case-sensitive)
	conditions.replace(0, AssignmentRule::Condition(AssignmentRule::WithdrawalAccount,
		AssignmentRule::StringEquals, true, "Withdrawal Equals"));
	rules->createRule(113, 21200, conditions);
	// withdrawal, equals
	conditions.replace(0, AssignmentRule::Condition(AssignmentRule::WithdrawalAccount,
		AssignmentRule::StringEquals, false, "withdrawal equals"));
	rules->createRule(112, 11000, conditions);

	// Create initial balance
	QList<Balance::Contributor> contributors;
	contributors << Balance::Contributor("Have", Money(10000, "USD"), true);
	contributors << Balance::Contributor("Owe", Money(200, "USD"), false);
	QSharedPointer<Balance> initial = Balance::create(contributors);

	// Create budgeting period
	BudgetingPeriod::Parameters params;
	params.type = BudgetingPeriod::CalendarMonth;
	params.param1 = 2013;
	params.param2 = 8;
	QSharedPointer<BudgetingPeriod> period(new BudgetingPeriod(params));

	// Create budget
	QSharedPointer<Budget> budget(new Budget("Serialized Budget", period,
		initial, root, rules));

	// Serialize budget
	QBuffer buffer;
	buffer.open(QIODevice::ReadWrite);
	QCOMPARE(XmlBudgetWriter::write(&buffer, budget), true);

	// Get the XML as an array of strings (lines)
	QString serialized(buffer.data());
	QStringList lines = serialized.split("\n");

	// Verify generated XML
	QCOMPARE(lines.at(0), QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"));
	QCOMPARE(lines.at(1).contains("ub:budget"), true);
	QCOMPARE(lines.at(1).contains("version=\"5.0\""), true);

	int i = 1;
	QCOMPARE(lines.at(++i).trimmed(), QString("<ub:name>Serialized Budget</ub:name>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<ub:initial-balance>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<balance:contributor>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<contributor:name>Have</contributor:name>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<contributor:amount currency=\"USD\">10000</contributor:amount>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<contributor:increase>true</contributor:increase>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</balance:contributor>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<balance:contributor>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<contributor:name>Owe</contributor:name>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<contributor:amount currency=\"USD\">200</contributor:amount>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<contributor:increase>false</contributor:increase>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</balance:contributor>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</ub:initial-balance>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<ub:period type=\"calendar-month\">"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<period:param1>2013</period:param1>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<period:param2>8</period:param2>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</ub:period>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:estimate id=\"0\">")); // root
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:estimate id=\"10000\">")); // incomes
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:name>Income Estimates</estimate:name>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:description>estimates for incomes</estimate:description>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:type>income</estimate:type>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:estimate id=\"11000\">")); // salary
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:name>Someone's Salary</estimate:name>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:amount currency=\"USD\">5623</estimate:amount>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</estimate:estimate>")); // salary
	QCOMPARE(lines.at(++i).trimmed(), QString("</estimate:estimate>")); // incomes
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:estimate id=\"20000\">")); // expenses
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:name>Expense Estimates</estimate:name>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:type>expense</estimate:type>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:estimate id=\"21000\">")); // bills
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:name>Bills</estimate:name>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:description>recurring expenses</estimate:description>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:estimate id=\"21100\">")); // rent
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:name>Rent</estimate:name>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:description>for the apt.</estimate:description>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:amount currency=\"USD\">452.23</estimate:amount>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:due-date>2013-06-28</estimate:due-date>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</estimate:estimate>")); // rent
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:estimate id=\"21200\">")); // utilities
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:name>Utilities</estimate:name>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:description>elec, gas, etc.</estimate:description>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:amount currency=\"USD\">500</estimate:amount>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:finished/>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</estimate:estimate>")); // utilities
	QCOMPARE(lines.at(++i).trimmed(), QString("</estimate:estimate>")); // bills
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:estimate id=\"22000\">")); // foreign expenses
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:name>Foreign Expenses</estimate:name>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:description>while in Europe</estimate:description>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:amount currency=\"EUR\">2000</estimate:amount>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</estimate:estimate>")); // foreign expenses
	QCOMPARE(lines.at(++i).trimmed(), QString("</estimate:estimate>")); // expenses
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:estimate id=\"30000\">")); // credit card
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:name>Credit Card</estimate:name>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:type>transfer</estimate:type>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:amount currency=\"USD\">1400</estimate:amount>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:due-date>2013-06-21</estimate:due-date>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<estimate:finished/>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</estimate:estimate>")); // credit card
	QCOMPARE(lines.at(++i).trimmed(), QString("</estimate:estimate>")); // root
	QCOMPARE(lines.at(++i).trimmed(), QString("<ub:rules>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:rule id=\"100\">"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:estimate>30000</rule:estimate>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:field>payee</condition:field>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:operator>begins-with</condition:operator>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:case-sensitive>false</condition:case-sensitive>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:value>payee begins with</condition:value>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:rule>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:rule id=\"101\">"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:estimate>21200</rule:estimate>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:field>memo</condition:field>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:operator>ends-with</condition:operator>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:case-sensitive>true</condition:case-sensitive>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:value>memo Ends With</condition:value>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:rule>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:rule id=\"102\">"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:estimate>21100</rule:estimate>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:rule>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:rule id=\"103\">"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:estimate>22000</rule:estimate>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:field>date</condition:field>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:operator>after</condition:operator>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:case-sensitive>false</condition:case-sensitive>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:value>2013-05-06</condition:value>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:field>date</condition:field>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:operator>before</condition:operator>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:case-sensitive>false</condition:case-sensitive>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:value>2013-05-10</condition:value>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:rule>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:rule id=\"104\">"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:estimate>30000</rule:estimate>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:field>date</condition:field>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:operator>date-equals</condition:operator>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:case-sensitive>false</condition:case-sensitive>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:value>2013-05-11</condition:value>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:rule>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:rule id=\"105\">"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:estimate>21100</rule:estimate>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:field>amount</condition:field>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:operator>less-than</condition:operator>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:case-sensitive>false</condition:case-sensitive>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:value>3400</condition:value>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:rule>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:rule id=\"106\">"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:estimate>211000</rule:estimate>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:field>amount</condition:field>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:operator>less-than-or-equal</condition:operator>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:case-sensitive>false</condition:case-sensitive>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:value>3500.50</condition:value>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:rule>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:rule id=\"107\">"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:estimate>11000</rule:estimate>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:field>amount</condition:field>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:operator>greater-than</condition:operator>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:case-sensitive>false</condition:case-sensitive>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:value>10000</condition:value>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:rule>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:rule id=\"109\">"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:estimate>21200</rule:estimate>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:field>amount</condition:field>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:operator>greater-than-or-equal</condition:operator>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:case-sensitive>false</condition:case-sensitive>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:value>12030</condition:value>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:rule>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:rule id=\"110\">"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:estimate>22000</rule:estimate>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:field>amount</condition:field>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:operator>amount-equals</condition:operator>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:case-sensitive>false</condition:case-sensitive>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:value>502.99</condition:value>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:rule>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:rule id=\"111\">"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:estimate>30000</rule:estimate>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:field>deposit</condition:field>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:operator>contains</condition:operator>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:case-sensitive>false</condition:case-sensitive>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:value>deposit contains</condition:value>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:rule>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:rule id=\"113\">"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:estimate>21200</rule:estimate>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:field>withdrawal</condition:field>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:operator>string-equals</condition:operator>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:case-sensitive>true</condition:case-sensitive>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:value>Withdrawal Equals</condition:value>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:rule>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:rule id=\"112\">"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:estimate>11000</rule:estimate>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:field>withdrawal</condition:field>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:operator>string-equals</condition:operator>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:case-sensitive>false</condition:case-sensitive>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("<condition:value>withdrawal equals</condition:value>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:condition>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</rule:rule>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</ub:rules>"));
	QCOMPARE(lines.at(++i).trimmed(), QString("</ub:budget>"));
	QCOMPARE(lines.at(++i).trimmed(), QString(""));
}

//------------------------------------------------------------------------------
void XmlBudgetWriterTest::writeBudgetingPeriods_data()
{
	QTest::addColumn<BudgetingPeriod::Type>("type");
	QTest::addColumn<QVariant>("param1");
	QTest::addColumn<QVariant>("param2");
	QTest::addColumn<QString>("period_line");
	QTest::addColumn<QString>("param1_line");
	QTest::addColumn<QString>("param2_line");

	QTest::newRow("calendar-year") << BudgetingPeriod::CalendarYear
		<< QVariant(2013) << QVariant()
		<< QString("<ub:period type=\"calendar-year\">")
		<< QString("<period:param1>2013</period:param1>")
		<< QString("<period:param2></period:param2>");
	QTest::newRow("calendar-month") << BudgetingPeriod::CalendarMonth
		<< QVariant(2013) << QVariant(11)
		<< QString("<ub:period type=\"calendar-month\">")
		<< QString("<period:param1>2013</period:param1>")
		<< QString("<period:param2>11</period:param2>");
	QTest::newRow("paydate-month") << BudgetingPeriod::PaydateMonth
		<< QVariant(2013) << QVariant(2)
		<< QString("<ub:period type=\"paydate-month\">")
		<< QString("<period:param1>2013</period:param1>")
		<< QString("<period:param2>2</period:param2>");
	QTest::newRow("custom-range") << BudgetingPeriod::CustomDateRange
		<< QVariant(QDate(2013,6,24)) << QVariant(QDate(2013,7,24))
		<< QString("<ub:period type=\"custom\">")
		<< QString("<period:param1>2013-06-24</period:param1>")
		<< QString("<period:param2>2013-07-24</period:param2>");
}

//------------------------------------------------------------------------------
void XmlBudgetWriterTest::writeBudgetingPeriods()
{
	QFETCH(BudgetingPeriod::Type, type);
	QFETCH(QVariant, param1);
	QFETCH(QVariant, param2);
	QFETCH(QString, period_line);
	QFETCH(QString, param1_line);
	QFETCH(QString, param2_line);

	// Create budgeting period
	BudgetingPeriod::Parameters params;
	params.type = type;
	params.param1 = param1;
	params.param2 = param2;
	QSharedPointer<BudgetingPeriod> period(new BudgetingPeriod(params));

	// Create budget
	QSharedPointer<Budget> budget(new Budget("Periods", period,
		Balance::create(), Estimate::createRoot(), AssignmentRules::create()));

	// Serialize budget
	QBuffer buffer;
	buffer.open(QIODevice::ReadWrite);
	QCOMPARE(XmlBudgetWriter::write(&buffer, budget), true);

	// Get the XML as an array of strings (lines)
	QString serialized(buffer.data());
	QStringList lines = serialized.split("\n");

	// Verify generated XML
	QCOMPARE(lines.at(10).trimmed(), period_line);
	QCOMPARE(lines.at(11).trimmed(), param1_line);
	QCOMPARE(lines.at(12).trimmed(), param2_line);
}

}

