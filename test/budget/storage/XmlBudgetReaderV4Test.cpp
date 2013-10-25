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
#include "XmlBudgetReaderV4Test.hpp"
#include "budget/AssignmentRules.hpp"
#include "budget/Balance.hpp"
#include "budget/Budget.hpp"
#include "budget/storage/XmlBudgetReader.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::XmlBudgetReaderV4Test)

namespace ub {

//------------------------------------------------------------------------------
void XmlBudgetReaderV4Test::cleanup()
{
	delete fullBudget;
	delete incompleteBudget;
}

//------------------------------------------------------------------------------
void XmlBudgetReaderV4Test::init()
{
	fullBudget = new QBuffer;
	fullBudget->open(QBuffer::ReadWrite);

	incompleteBudget = new QBuffer;
	incompleteBudget->open(QBuffer::ReadWrite);

	fullBudget->write("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
		"<budget version=\"4.0\">"
		" <name>Version 4 Budget</name>"
		" <period type=\"custom\">"
		"  <start-date>"
		"   <day>1</day>"
		"   <month>6</month>" // July, starts at 0
		"   <year>2013</year>"
		"  </start-date>"
		"  <end-date>"
		"   <day>1</day>"
		"   <month>7</month>" // August, starts at 0
		"   <year>2013</year>"
		"  </end-date>"
		" </period>"
		" <initial-balance currency=\"USD\">14500</initial-balance>"
		" <estimate id=\"0\" version=\"1.1\" uid=\"12341234\">"
		"  <name>Root</name>"
		"  <amount currency=\"USD\">0.00</amount>"
		"  <type>EXPENSE</type>"
		"  <complete>false</complete>"
		"  <estimates>"
		"   <estimate id=\"1\" version=\"1.1\" uid=\"122523\">"
		"    <name>Income Estimates</name>"
		"    <amount currency=\"USD\">0.00</amount>"
		"    <type>CATEGORY</type>"
		"    <complete>false</complete>"
		"    <estimates>"
		"     <estimate id=\"2\" version=\"1.1\" uid=\"789234552\">"
		"      <name>Salary</name>"
		"      <description>paychecks</description>"
		"      <amount currency=\"USD\">2400</amount>"
		"      <type>INCOME</type>"
		"      <complete>false</complete>"
		"      <estimates/>"
		"     </estimate>" // salary
		"    </estimates>"
		"   </estimate>" // income
		"   <estimate id=\"3\" version=\"1.1\" uid=\"872231\">"
		"    <name>Expense Estimates</name>"
		"    <amount currency=\"USD\">0.00</amount>"
		"    <type>CATEGORY</type>"
		"    <complete>false</complete>"
		"    <estimates>"
		"     <estimate id=\"4\" version=\"1.1\" uid=\"87222211\">"
		"      <name>Utilities</name>"
		"      <amount currency=\"USD\">0.00</amount>"
		"      <type>CATEGORY</type>"
		"      <complete>false</complete>"
		"      <estimates>"
		"       <estimate id=\"5\" version=\"1.1\" uid=\"875211115\">"
		"        <name>Water</name>"
		"        <amount currency=\"USD\">45</amount>"
		"        <type>EXPENSE</type>"
		"        <due-date>"
		"         <day>18</day>"
		"         <month>6</month>" // July, starts at 0
		"         <year>2013</year>"
		"        </due-date>"
		"        <complete>false</complete>"
		"        <estimates/>"
		"       </estimate>" // water
		"       <estimate id=\"6\" version=\"1.1\" uid=\"87111123\">"
		"        <name>Gas</name>"
		"        <amount currency=\"USD\">114.23</amount>"
		"        <type>EXPENSE</type>"
		"        <complete>true</complete>"
		"        <estimates/>"
		"       </estimate>" // gas
		"      </estimates>"
		"     </estimate>" // utilities
		"    </estimates>"
		"   </estimate>" // expense
		"  </estimates>"
		" </estimate>" // root
		" <rules>"
		"  <rule>"
		"   <estimate reference=\"5\"/>"
		"   <conditions>"
		"    <condition>"
		"     <field>DEPOSIT</field>"
		"     <operator>ENDS_WITH</operator>"
		"     <value>expense_acct</value>"
		"    </condition>"
		"   </conditions>"
		"  </rule>"
		"  <rule>"
		"   <estimate reference=\"2\"/>"
		"   <conditions>"
		"    <condition>"
		"     <field>WITHDRAWAL</field>"
		"     <operator>EQUALS</operator>"
		"     <value>income_acct</value>"
		"    </condition>"
		"    <condition>"
		"     <field>MEMO</field>"
		"     <operator>EQUALS_CASE</operator>"
		"     <value>vAluE</value>"
		"    </condition>"
		"   </conditions>"
		"  </rule>"
		" </rules>"
		"</budget>");

	incompleteBudget->write("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
		"<budget version=\"4.0\">"
		"</budget>");

	fullBudget->seek(0);
	incompleteBudget->seek(0);
}

//------------------------------------------------------------------------------
void XmlBudgetReaderV4Test::readFullBudget()
{
	XmlBudgetReader reader;

	// Make sure budget was read successfully
	QCOMPARE(reader.read(fullBudget), true);
	QCOMPARE(reader.lastReadBudget().isNull(), false);

	// Make sure budget was read correctly
	QSharedPointer<Budget> budget = reader.lastReadBudget();
	QCOMPARE(budget->name(), QString("Version 4 Budget"));
	QCOMPARE(budget->initialBalance()->value(), Money(14500, "USD"));

	BudgetingPeriod::Parameters period = budget->budgetingPeriod()->parameters();
	QCOMPARE(period.type, BudgetingPeriod::CustomDateRange);
	QCOMPARE(period.param1.toDate(), QDate(2013,7,1));
	QCOMPARE(period.param2.toDate(), QDate(2013,8,1));

	// Make sure estimates were read correctly
	QSharedPointer<Estimate> rootEstimate = budget->estimates();
	COMPARE_ESTIMATE(rootEstimate.data(), (uint) 0, QString("Root"), QString(""),
		Estimate::Root, Money(), QDate(), false, 2);

	Estimate* income = rootEstimate->childAt(0);
	COMPARE_ESTIMATE(income, (uint) 1, QString("Income Estimates"), QString(""),
		Estimate::Expense, Money(), QDate(), false, 1);

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

	// Make sure rules were read correctly
	QSharedPointer<AssignmentRules> rules = budget->rules();
	QCOMPARE(rules->size(), 2);

	AssignmentRule* rule1 = rules->at(0);
	COMPARE_RULE(rule1, (uint) 5, 1);
	COMPARE_CONDITION(rule1->conditionAt(0), AssignmentRule::DepositAccount,
		AssignmentRule::EndsWith, false, QString("expense_acct"));

	AssignmentRule* rule2 = rules->at(1);
	COMPARE_RULE(rule2, (uint) 2, 2);
	COMPARE_CONDITION(rule2->conditionAt(0), AssignmentRule::WithdrawalAccount,
		AssignmentRule::StringEquals, false, QString("income_acct"));
	COMPARE_CONDITION(rule2->conditionAt(1), AssignmentRule::Memo,
		AssignmentRule::StringEquals, true, QString("vAluE"));
}

//------------------------------------------------------------------------------
void XmlBudgetReaderV4Test::readIncompleteBudget()
{
	XmlBudgetReader reader;

	// Make sure budget was read successfully
	QCOMPARE(reader.read(incompleteBudget), true);
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
void XmlBudgetReaderV4Test::readBudgetingPeriods_data()
{
	QTest::addColumn<QString>("data");
	QTest::addColumn<BudgetingPeriod::Type>("type");
	QTest::addColumn<QVariant>("param1");
	QTest::addColumn<QVariant>("param2");

	QString templateData = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
		"<budget version=\"4.0\">"
		"%1"
		"</budget>";

	QString custom = "<period type=\"custom\">"
		" <start-date>"
		"  <day>1</day>"
		"  <month>4</month>"
		"  <year>2013</year>"
		" </start-date>"
		" <end-date>"
		"  <day>5</day>"
		"  <month>4</month>"
		"  <year>2013</year>"
		" </end-date>"
		"</period>";

	QString calMon = "<period type=\"literal-month\">"
		" <month>4</month>"
		" <year>2013</year>"
		"</period>";

	QString payMon = "<period type=\"paydate-month\">"
		" <month>4</month>"
		" <year>2013</year>"
		"</period>";

	QTest::newRow("custom") << templateData.arg(custom)
		<< BudgetingPeriod::CustomDateRange << QVariant(QDate(2013,5,1))
		<< QVariant(QDate(2013,5,5));
	QTest::newRow("calendar-month") << templateData.arg(calMon)
		<< BudgetingPeriod::CalendarMonth << QVariant(2013)
		<< QVariant(5);
	QTest::newRow("paydate-month") << templateData.arg(payMon)
		<< BudgetingPeriod::PaydateMonth << QVariant(2013)
		<< QVariant(5);
}

//------------------------------------------------------------------------------
void XmlBudgetReaderV4Test::readBudgetingPeriods()
{
	QFETCH(QString, data);
	QFETCH(BudgetingPeriod::Type, type);
	QFETCH(QVariant, param1);
	QFETCH(QVariant, param2);

	QSharedPointer<QIODevice> buffer(new QBuffer);
	buffer->open(QBuffer::ReadWrite);
	buffer->write(qPrintable(data));
	buffer->seek(0);

	XmlBudgetReader reader;

	// Make sure budget was read successfully
	QCOMPARE(reader.read(buffer.data()), true);
	QCOMPARE(reader.lastReadBudget().isNull(), false);

	QSharedPointer<Budget> budget = reader.lastReadBudget();
	QSharedPointer<BudgetingPeriod> period = budget->budgetingPeriod();
	BudgetingPeriod::Parameters params = period->parameters();
	QCOMPARE(params.type, type);
	QCOMPARE(params.param1, param1);
	QCOMPARE(params.param2, param2);
}

//------------------------------------------------------------------------------
void XmlBudgetReaderV4Test::readRuleConditions_data()
{
	QTest::addColumn<QString>("data");
	QTest::addColumn<AssignmentRule::Field>("field");
	QTest::addColumn<AssignmentRule::Operator>("oper");
	QTest::addColumn<bool>("sensitive");
	QTest::addColumn<QString>("value");

	QString templateData = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
		"<budget version=\"4.0\">"
		" <rules>"
		"  <rule>"
		"   <conditions>"
		"    <condition>"
		"     <field>%1</field>"
		"     <operator>%2</operator>"
		"     <value>%3</value>"
		"    </condition>"
		"   </conditions>"
		"  </rule>"
		" </rules>"
		"</budget>";

	QTest::newRow("memo,begins") << templateData.arg("MEMO").arg("BEGINS_WITH").arg("j")
		<< AssignmentRule::Memo << AssignmentRule::BeginsWith << false << "j";
	QTest::newRow("payee,contains") << templateData.arg("PAYEE").arg("CONTAINS").arg("k")
		<< AssignmentRule::Payee << AssignmentRule::Contains << false << "k";
	QTest::newRow("withdrawal,ends") << templateData.arg("WITHDRAWAL").arg("ENDS_WITH").arg("l")
		<< AssignmentRule::WithdrawalAccount << AssignmentRule::EndsWith << false << "l";
	QTest::newRow("deposit,equals") << templateData.arg("DEPOSIT").arg("EQUALS").arg("m")
		<< AssignmentRule::DepositAccount << AssignmentRule::StringEquals << false << "m";
	QTest::newRow("memo,equals case") << templateData.arg("MEMO").arg("EQUALS_CASE").arg("n")
		<< AssignmentRule::Memo << AssignmentRule::StringEquals << true << "n";
}

//------------------------------------------------------------------------------
void XmlBudgetReaderV4Test::readRuleConditions()
{
	QFETCH(QString, data);
	QFETCH(AssignmentRule::Field, field);
	QFETCH(AssignmentRule::Operator, oper);
	QFETCH(bool, sensitive);
	QFETCH(QString, value);

	QSharedPointer<QIODevice> buffer(new QBuffer);
	buffer->open(QBuffer::ReadWrite);
	buffer->write(qPrintable(data));
	buffer->seek(0);

	XmlBudgetReader reader;

	// Make sure budget was read successfully
	QCOMPARE(reader.read(buffer.data()), true);
	QCOMPARE(reader.lastReadBudget().isNull(), false);

	QSharedPointer<Budget> budget = reader.lastReadBudget();
	QSharedPointer<AssignmentRules> rules = budget->rules();
	QCOMPARE(rules->size(), 1);
	AssignmentRule* rule = rules->at(0);
	QCOMPARE(rule->conditionCount(), 1);
	COMPARE_CONDITION(rule->conditionAt(0), field, oper, sensitive, value);
}

}

