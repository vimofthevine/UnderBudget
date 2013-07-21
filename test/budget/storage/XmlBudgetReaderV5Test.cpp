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
#include "budget_comparisons.hpp"
#include "XmlBudgetReaderV5Test.hpp"
#include "budget/AssignmentRules.hpp"
#include "budget/Balance.hpp"
#include "budget/Budget.hpp"
#include "budget/storage/XmlBudgetReader.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::XmlBudgetReaderV5Test)

namespace ub {

//------------------------------------------------------------------------------
void XmlBudgetReaderV5Test::readFullBudget()
{
	QBuffer buffer;
	buffer.open(QBuffer::ReadWrite);
	buffer.write("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
		"<ub:budget xmlns:ub=\"http://underbudget.vimofthevine.com/ub\""
		"  xmlns:balance=\"http://underbudget.vimofthevine.com/balance\""
		"  xmlns:contributor=\"http://underbudget.vimofthevine.com/contributor\""
		"  xmlns:period=\"http://underbudget.vimofthevine.com/budgeting-period\""
		"  xmlns:estimate=\"http://underbudget.vimofthevine.com/estimate\""
		"  xmlns:rule=\"http://underbudget.vimofthevine.com/rule\""
		"  xmlns:condition=\"http://underbudget.vimofthevine.com/condition\""
		"  version=\"5.0\">\n"
		" <ub:name>Version 5 Budget</ub:name>\n"
		" <ub:initial-balance>\n"
		"  <balance:contributor>\n"
		"   <contributor:name>Asset</contributor:name>\n"
		"   <contributor:amount currency=\"USD\">16500</contributor:amount>\n"
		"   <contributor:increase>true</contributor:increase>\n"
		"  </balance:contributor>\n"
		"  <balance:contributor>\n"
		"   <contributor:name>Liability</contributor:name>\n"
		"   <contributor:amount currency=\"USD\">2000</contributor:amount>\n"
		"   <contributor:increase>false</contributor:increase>\n"
		"  </balance:contributor>\n"
		" </ub:initial-balance>\n"
		" <ub:period type=\"custom\">\n"
		"  <period:param1>2013-07-01</period:param1>\n"
		"  <period:param2>2013-08-01</period:param2>\n"
		" </ub:period>\n"
		" <ub:estimate id=\"0\">\n" // { root
		"  <ub:estimate id=\"1\">\n" // { incomes
		"   <estimate:name>Income Estimates</estimate:name>\n"
		"   <estimate:type>income</estimate:type>\n"
		"   <ub:estimate id=\"2\">\n" // { salary
		"    <estimate:name>Salary</estimate:name>\n"
		"    <estimate:description>paychecks</estimate:description>\n"
		"    <estimate:amount currency=\"USD\">2400</estimate:amount>\n"
		"   </ub:estimate>\n" // } salary
		"  </ub:estimate>\n" // } incomes
		"  <ub:estimate id=\"3\">\n" // { expenses
		"   <estimate:name>Expense Estimates</estimate:name>\n"
		"   <estimate:type>expense</estimate:type>\n"
		"   <ub:estimate id=\"4\">\n" // { utilities
		"    <estimate:name>Utilities</estimate:name>\n"
		"    <ub:estimate id=\"5\">\n" // { water
		"     <estimate:name>Water</estimate:name>\n"
		"     <estimate:amount currency=\"USD\">45</estimate:amount>\n"
		"     <estimate:due-date>2013-07-18</estimate:due-date>\n"
		"    </ub:estimate>\n" // } water
		"    <ub:estimate id=\"6\">\n" // { gas
		"     <estimate:name>Gas</estimate:name>\n"
		"     <estimate:amount currency=\"USD\">114.23</estimate:amount>\n"
		"     <estimate:finished/>\n"
		"    </ub:estimate>\n" // } gas
		"   </ub:estimate>\n" // } utilities
		"  </ub:estimate>\n" // } expenses
		"  <ub:estimate id=\"7\">\n" // { credit card
		"   <estimate:name>Credit Card</estimate:name>\n"
		"   <estimate:type>transfer</estimate:type>\n"
		"   <estimate:amount currency=\"USD\">146</estimate:amount>\n"
		"  </ub:estimate>\n" // } credit card
		" </ub:estimate>\n" // } root
		" <ub:rules>\n"
		"  <ub:rule id=\"20\">\n"
		"   <rule:estimate>5</rule:estimate>\n"
		"   <rule:condition>\n"
		"    <condition:field>deposit</condition:field>\n"
		"    <condition:operator>ends-with</condition:operator>\n"
		"    <condition:case-sensitive>false</condition:case-sensitive>\n"
		"    <condition:value>expense_acct</condition:value>\n"
		"   </rule:condition>\n"
		"  </ub:rule>\n"
		"  <ub:rule id=\"21\">\n"
		"   <rule:estimate>2</rule:estimate>\n"
		"   <rule:condition>\n"
		"    <condition:field>withdrawal</condition:field>\n"
		"    <condition:operator>string-equals</condition:operator>\n"
		"    <condition:case-sensitive>false</condition:case-sensitive>\n"
		"    <condition:value>income_acct</condition:value>\n"
		"   </rule:condition>\n"
		"   <rule:condition>\n"
		"    <condition:field>memo</condition:field>\n"
		"    <condition:operator>string-equals</condition:operator>\n"
		"    <condition:case-sensitive>true</condition:case-sensitive>\n"
		"    <condition:value>vAluE</condition:value>\n"
		"   </rule:condition>\n"
		"  </ub:rule>\n"
		" </ub:rules>\n"
		"</ub:budget>\n");
	buffer.seek(0);

	XmlBudgetReader reader;

	// Make sure budget was read successfully
	bool success = reader.read(&buffer);
	if ( ! success)
		qDebug() << reader.errorString();
	QCOMPARE(success, true);
	QCOMPARE(reader.lastReadBudget().isNull(), false);

	// Make sure budget was read correctly
	QSharedPointer<Budget> budget = reader.lastReadBudget();
	QCOMPARE(budget->name(), QString("Version 5 Budget"));

	QSharedPointer<Balance> balance = budget->initialBalance();
	QCOMPARE(balance->contributorCount(), 2);
	QCOMPARE(balance->contributorAt(0).name, QString("Asset"));
	QCOMPARE(balance->contributorAt(0).amount, Money(16500, "USD"));
	QCOMPARE(balance->contributorAt(0).increase, true);
	QCOMPARE(balance->contributorAt(1).name, QString("Liability"));
	QCOMPARE(balance->contributorAt(1).amount, Money(2000, "USD"));
	QCOMPARE(balance->contributorAt(1).increase, false);
	QCOMPARE(balance->value(), Money(14500, "USD"));

	BudgetingPeriod::Parameters period = budget->budgetingPeriod()->parameters();
	QCOMPARE(period.type, BudgetingPeriod::CustomDateRange);
	QCOMPARE(period.param1.toDate(), QDate(2013,7,1));
	QCOMPARE(period.param2.toDate(), QDate(2013,8,1));

	// Make sure estimates were read correctly
	QSharedPointer<Estimate> rootEstimate = budget->estimates();
	COMPARE_ESTIMATE(rootEstimate.data(), (uint) 0, QString("Root"), QString(""),
		Estimate::Root, Money(), QDate(), false, 3);

	Estimate* income = rootEstimate->childAt(0);
	COMPARE_ESTIMATE(income, (uint) 1, QString("Income Estimates"), QString(""),
		Estimate::Income, Money(), QDate(), false, 1);

	Estimate* salary = income->childAt(0);
	COMPARE_ESTIMATE(salary, (uint) 2, QString("Salary"), QString("paychecks"),
		Estimate::Income, Money(2400, "USD"), QDate(), false, 0);

	Estimate* expense = rootEstimate->childAt(1);
	COMPARE_ESTIMATE(expense, (uint) 3, QString("Expense Estimates"), QString(""),
		Estimate::Expense, Money(), QDate(), false, 1);

	Estimate* utilities = expense->childAt(0);
	COMPARE_ESTIMATE(utilities, (uint) 4, QString("Utilities"), QString(""),
		Estimate::Expense, Money(), QDate(), false, 2);

	Estimate* water = utilities->childAt(0);
	COMPARE_ESTIMATE(water, (uint) 5, QString("Water"), QString(""),
		Estimate::Expense, Money(45, "USD"), QDate(2013, 7, 18), false, 0);

	Estimate* gas = utilities->childAt(1);
	COMPARE_ESTIMATE(gas, (uint) 6, QString("Gas"), QString(""),
		Estimate::Expense, Money(114.23, "USD"), QDate(), true, 0);

	Estimate* credit = rootEstimate->childAt(2);
	COMPARE_ESTIMATE(credit, (uint) 7, QString("Credit Card"), QString(""),
		Estimate::Transfer, Money(146, "USD"), QDate(), false, 0);

	// Make sure rules were read correctly
	QSharedPointer<AssignmentRules> rules = budget->rules();
	QCOMPARE(rules->size(), 2);

	AssignmentRule* rule1 = rules->at(0);
	COMPARE_RULE_ID(rule1, (uint) 20, (uint) 5, 1);
	COMPARE_CONDITION(rule1->conditionAt(0), AssignmentRule::DepositAccount,
		AssignmentRule::EndsWith, false, QString("expense_acct"));

	AssignmentRule* rule2 = rules->at(1);
	COMPARE_RULE_ID(rule2, (uint) 21, (uint) 2, 2);
	COMPARE_CONDITION(rule2->conditionAt(0), AssignmentRule::WithdrawalAccount,
		AssignmentRule::StringEquals, false, QString("income_acct"));
	COMPARE_CONDITION(rule2->conditionAt(1), AssignmentRule::Memo,
		AssignmentRule::StringEquals, true, QString("vAluE"));
}

//------------------------------------------------------------------------------
void XmlBudgetReaderV5Test::readIncompleteBudget()
{
	QBuffer buffer;
	buffer.open(QBuffer::ReadWrite);
	buffer.write("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
		"<ub:budget xmlns:ub=\"http://underbudget/vimofthevine.com/ub\""
		"  xmlns:period=\"http://underbudget/vimofthevine.com/budgeting-period\""
		"  xmlns:estimate=\"http://underbudget/vimofthevine.com/estimate\""
		"  xmlns:rule=\"http://underbudget/vimofthevine.com/rule\""
		"  xmlns:condition=\"http://underbudget/vimofthevine.com/condition\""
		"  version=\"5.0\">\n"
		"</ub:budget>\n");
	buffer.seek(0);

	XmlBudgetReader reader;

	// Make sure budget was read successfully
	QCOMPARE(reader.read(&buffer), true);
	QCOMPARE(reader.lastReadBudget().isNull(), false);

	// Make sure budget was read
	QSharedPointer<Budget> budget = reader.lastReadBudget();
	QCOMPARE(budget->name(), QString("Budget"));
	QCOMPARE(budget->budgetingPeriod()->parameters().type,
		BudgetingPeriod::CalendarMonth);
	QCOMPARE(budget->initialBalance()->value(), Money());
	QCOMPARE(budget->estimates()->childCount(), 0);
	QCOMPARE(budget->rules()->size(), 0);
}

//------------------------------------------------------------------------------
void XmlBudgetReaderV5Test::readBudgetingPeriods_data()
{
	QTest::addColumn<QString>("data");
	QTest::addColumn<bool>("shouldRead");
	QTest::addColumn<BudgetingPeriod::Type>("type");
	QTest::addColumn<QVariant>("param1");
	QTest::addColumn<QVariant>("param2");

	QString templateData = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
		"<ub:budget xmlns:ub=\"http://underbudget/vimofthevine.com/ub\""
		"  xmlns:period=\"http://underbudget/vimofthevine.com/budgeting-period\""
		"  version=\"5.0\">"
		"%1"
		"</ub:budget>";

	QString custom = "<ub:period type=\"custom\">"
		" <period:param1>2013-05-01</period:param1>"
		" <period:param2>2013-05-05</period:param2>"
		"</ub:period>";

	QString calMon = "<ub:period type=\"calendar-month\">"
		" <period:param1>2013</period:param1>"
		" <period:param2>5</period:param2>"
		"</ub:period>";

	QString calYear = "<ub:period type=\"calendar-year\">"
		" <period:param1>2013</period:param1>"
		" <period:param2></period:param2>"
		"</ub:period>";

	QString payMon = "<ub:period type=\"paydate-month\">"
		" <period:param1>2013</period:param1>"
		" <period:param2>5</period:param2>"
		"</ub:period>";

	QString singleParam = "<ub:period type=\"calendar-year\">"
		" <period:param1>2013</period:param1>"
		"</ub:period>";

	QString unknownType = "<ub:period type=\"literal-year\">\n"
		" <period:param1>2013</period:param1>\n"
		" <period:param2>2</period:param2>\n"
		"</ub:period>\n";

	QString invalidDateParams = "<ub:period type=\"custom\">\n"
		" <period:param1>2013</period:param1>\n"
		" <period:param2>4</period:param2>\n"
		"</ub:period>\n";

	QString invalidIntParams = "<ub:period type=\"calendar-month\">\n"
		" <period:param1>2013-04-23</period:param1>\n"
		" <period:param2>2013-07-12</period:param2>\n"
		"</ub:period>\n";

	QString invalidDates = "<ub:period type=\"custom\">\n"
		" <period:param1>2013-14-23</period:param1>\n"
		" <period:param2>2013-07-12</period:param2>\n"
		"</ub:period>\n";

	QTest::newRow("custom") << templateData.arg(custom) << true
		<< BudgetingPeriod::CustomDateRange << QVariant(QDate(2013,5,1))
		<< QVariant(QDate(2013,5,5));
	QTest::newRow("calendar-month") << templateData.arg(calMon) << true
		<< BudgetingPeriod::CalendarMonth << QVariant(2013)
		<< QVariant(5);
	QTest::newRow("calendar-year") << templateData.arg(calYear) << true
		<< BudgetingPeriod::CalendarYear << QVariant(2013)
		<< QVariant("");
	QTest::newRow("paydate-month") << templateData.arg(payMon) << true
		<< BudgetingPeriod::PaydateMonth << QVariant(2013)
		<< QVariant(5);
	QTest::newRow("single-param") << templateData.arg(singleParam) << true
		<< BudgetingPeriod::CalendarYear << QVariant(2013) << QVariant();
	QTest::newRow("unknown-type") << templateData.arg(unknownType)
		<< false << BudgetingPeriod::Undefined << QVariant() << QVariant();
	QTest::newRow("invalid-date-params") << templateData.arg(invalidDateParams)
		<< false << BudgetingPeriod::CustomDateRange << QVariant() << QVariant();
	QTest::newRow("invalid-int-params") << templateData.arg(invalidIntParams)
		<< false << BudgetingPeriod::CalendarMonth << QVariant() << QVariant();
	QTest::newRow("invalid-dates") << templateData.arg(invalidDates)
		<< false << BudgetingPeriod::CustomDateRange << QVariant() << QVariant();
}

//------------------------------------------------------------------------------
void XmlBudgetReaderV5Test::readBudgetingPeriods()
{
	QFETCH(QString, data);
	QFETCH(bool, shouldRead);
	QFETCH(BudgetingPeriod::Type, type);
	QFETCH(QVariant, param1);
	QFETCH(QVariant, param2);

	QBuffer buffer;
	buffer.open(QBuffer::ReadWrite);
	buffer.write(qPrintable(data));
	buffer.seek(0);

	XmlBudgetReader reader;

	bool success = reader.read(&buffer);
	if ( ! success && shouldRead)
		qDebug() << reader.errorString();
	QCOMPARE(success, shouldRead);
	if (shouldRead)
	{
		QCOMPARE(reader.lastReadBudget().isNull(), false);

		QSharedPointer<Budget> budget = reader.lastReadBudget();
		QSharedPointer<BudgetingPeriod> period = budget->budgetingPeriod();
		BudgetingPeriod::Parameters params = period->parameters();
		QCOMPARE(params.type, type);
		QCOMPARE(params.param1, param1);
		QCOMPARE(params.param2, param2);
	}
}

//------------------------------------------------------------------------------
void XmlBudgetReaderV5Test::readRuleConditions_data()
{
	QTest::addColumn<QString>("data");
	QTest::addColumn<bool>("shouldRead");
	QTest::addColumn<AssignmentRule::Field>("field");
	QTest::addColumn<AssignmentRule::Operator>("oper");
	QTest::addColumn<bool>("sensitive");
	QTest::addColumn<QString>("value");

	QString templateData = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
		"<ub:budget xmlns:ub=\"http://underbudget/vimofthevine.com/ub\""
		"  xmlns:rule=\"http://underbudget/vimofthevine.com/rule\""
		"  xmlns:condition=\"http://underbudget/vimofthevine.com/condition\""
		"  version=\"5.0\">"
		" <ub:rules>\n"
		"  <ub:rule>\n"
		"   <rule:condition>\n"
		"    <condition:field>%1</condition:field>\n"
		"    <condition:operator>%2</condition:operator>\n"
		"    <condition:case-sensitive>%3</condition:case-sensitive>\n"
		"    <condition:value>%4</condition:value>\n"
		"   </rule:condition>\n"
		"  </ub:rule>\n"
		" </ub:rules>\n"
		"</ub:budget>\n";

	// Dates
	QTest::newRow("date,before")
		<< templateData.arg("date").arg("before").arg("false").arg("2013-06-02")
		<< true << AssignmentRule::Date << AssignmentRule::Before
		<< false << "2013-06-02";
	QTest::newRow("date,after")
		<< templateData.arg("date").arg("after").arg("false").arg("2013-07-04")
		<< true << AssignmentRule::Date << AssignmentRule::After
		<< false << "2013-07-04";
	QTest::newRow("date,equals")
		<< templateData.arg("date").arg("date-equals").arg("false").arg("2013-12-25")
		<< true << AssignmentRule::Date << AssignmentRule::DateEquals
		<< false << "2013-12-25";
	QTest::newRow("date,invalid")
		<< templateData.arg("date").arg("date-equals").arg("false").arg("text")
		<< false << AssignmentRule::Date << AssignmentRule::DateEquals
		<< false << "text";
	QTest::newRow("date,ends-with")
		<< templateData.arg("date").arg("ends-with").arg("true").arg("2013-01-04")
		<< false << AssignmentRule::Date << AssignmentRule::EndsWith
		<< true << "2013-01-04";

	// Amounts
	QTest::newRow("amount,less-than")
		<< templateData.arg("amount").arg("less-than").arg("false").arg("23.5")
		<< true << AssignmentRule::Amount << AssignmentRule::LessThan
		<< false << "23.5";
	QTest::newRow("amount,less-than-or-equal")
		<< templateData.arg("amount").arg("less-than-or-equal").arg("false").arg("225")
		<< true << AssignmentRule::Amount << AssignmentRule::LessThanOrEqual
		<< false << "225";
	QTest::newRow("amount,greater-than")
		<< templateData.arg("amount").arg("greater-than").arg("false").arg("13.75")
		<< true << AssignmentRule::Amount << AssignmentRule::GreaterThan
		<< false << "13.75";
	QTest::newRow("amount,greater-than-or-equal")
		<< templateData.arg("amount").arg("greater-than-or-equal").arg("false").arg("2.0")
		<< true << AssignmentRule::Amount << AssignmentRule::GreaterThanOrEqual
		<< false << "2.0";
	QTest::newRow("amount,equals")
		<< templateData.arg("amount").arg("amount-equals").arg("false").arg("204.99")
		<< true << AssignmentRule::Amount << AssignmentRule::AmountEquals
		<< false << "204.99";
	QTest::newRow("amount,invalid")
		<< templateData.arg("amount").arg("amount-equals").arg("false").arg("text")
		<< false << AssignmentRule::Amount << AssignmentRule::AmountEquals
		<< false << "text";
	QTest::newRow("amount,after")
		<< templateData.arg("amount").arg("after").arg("true").arg("12345")
		<< false << AssignmentRule::Amount << AssignmentRule::After
		<< true << "12345";

	// Strings
	QTest::newRow("payee,begins")
		<< templateData.arg("payee").arg("begins-with").arg("false").arg("payee begins with")
		<< true << AssignmentRule::Payee << AssignmentRule::BeginsWith
		<< false << "payee begins with";
	QTest::newRow("memo,ends,sensitive")
		<< templateData.arg("memo").arg("ends-with").arg("true").arg("memo Ends With")
		<< true << AssignmentRule::Memo << AssignmentRule::EndsWith
		<< true << "memo Ends With";
	QTest::newRow("deposit,contains")
		<< templateData.arg("deposit").arg("contains").arg("false").arg("deposit contains")
		<< true << AssignmentRule::DepositAccount << AssignmentRule::Contains
		<< false << "deposit contains";
	QTest::newRow("withdrawal,equals,sensitive")
		<< templateData.arg("withdrawal").arg("string-equals").arg("true").arg("Withdrawal Equals")
		<< true << AssignmentRule::WithdrawalAccount << AssignmentRule::StringEquals
		<< true << "Withdrawal Equals";
	QTest::newRow("memo,before")
		<< templateData.arg("memo").arg("before").arg("false").arg("2013-04-03")
		<< false << AssignmentRule::Memo << AssignmentRule::Before
		<< false << "2013-04-03";
}

//------------------------------------------------------------------------------
void XmlBudgetReaderV5Test::readRuleConditions()
{
	QFETCH(QString, data);
	QFETCH(bool, shouldRead);
	QFETCH(AssignmentRule::Field, field);
	QFETCH(AssignmentRule::Operator, oper);
	QFETCH(bool, sensitive);
	QFETCH(QString, value);

	QBuffer buffer;
	buffer.open(QBuffer::ReadWrite);
	buffer.write(qPrintable(data));
	buffer.seek(0);

	XmlBudgetReader reader;
	bool success = reader.read(&buffer);
	if ( ! success && shouldRead)
	{
		qDebug() << reader.errorString();
	}
	QCOMPARE(success, shouldRead);
	if (shouldRead)
	{
		QCOMPARE(reader.lastReadBudget().isNull(), false);

		QSharedPointer<Budget> budget = reader.lastReadBudget();
		QSharedPointer<AssignmentRules> rules = budget->rules();
		QCOMPARE(rules->size(), 1);
		AssignmentRule* rule = rules->at(0);
		QCOMPARE(rule->conditionCount(), 1);
		COMPARE_CONDITION(rule->conditionAt(0), field, oper, sensitive, value);
	}
}

}

