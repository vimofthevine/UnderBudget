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
#include "XmlBudgetReaderTest.hpp"
#include "budget/AssignmentRules.hpp"
#include "budget/Budget.hpp"
#include "budget/storage/XmlBudgetReader.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::XmlBudgetReaderTest)

namespace ub {

//------------------------------------------------------------------------------
void XmlBudgetReaderTest::cleanup()
{
	delete version4;
	delete oldVersion;
	delete invalidXml;
	delete otherXml;
}

//------------------------------------------------------------------------------
void XmlBudgetReaderTest::init()
{
	invalidXml = new QBuffer;
	invalidXml->open(QBuffer::ReadWrite);

	otherXml = new QBuffer;
	otherXml->open(QBuffer::ReadWrite);

	oldVersion = new QBuffer;
	oldVersion->open(QBuffer::ReadWrite);

	version4 = new QBuffer;
	version4->open(QBuffer::ReadWrite);

	invalidXml->write("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
		"<budget version=\"4.0\">"
		" <name>Invalid Budget</name>"
		" <estimate id=\"0\" version=\"1.1\" uid=\"12341234\">"
		"  <name>Root" // missing closing tag
		"  <amount currency=\"USD\">0.00</amount>"
		"  <type>EXPENSE</type>"
		"  <complete>false</complete>"
		"  <estimates />"
		" </estimate>"
		" <rules>"
		"  <rule>"
		"   <estimate reference=\"3\"/>"
		"   <conditions>"
		"    <condition>"
		"     <field>DEPOSIT</field>"
		"     <operator>ENDS_WITH</operator>"
		"     <value>condition_text</value>"
		"    </condition>"
		"   </conditions>"
		"  </rule>"
		" </rules>"
		"</budget>");

	otherXml->write("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
		"<document>"
		"  <element/>"
		"  <element/>"
		"</document>");

	oldVersion->write("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
		"<budget version=\"3.0\">"
		" <name>Old Version Budget</name>"
		"</budget>");

	version4->write("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
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

	invalidXml->seek(0);
	otherXml->seek(0);
	oldVersion->seek(0);
	version4->seek(0);
}

//------------------------------------------------------------------------------
void XmlBudgetReaderTest::compare(Estimate* actual, uint id, const QString& name,
	const QString& descrip, Estimate::Type type, const Money& amount,
	const QDate& due, bool finished, int children)
{
	QCOMPARE(actual->estimateName(), name);
	QCOMPARE(actual->estimateDescription(), descrip);
	QCOMPARE(actual->estimateType(), type);
	QCOMPARE(actual->estimatedAmount(), amount);
	QCOMPARE(actual->activityDueDate(), due);
	QCOMPARE(actual->isActivityFinished(), finished);
	QCOMPARE(actual->childCount(), children);
}

//------------------------------------------------------------------------------
void XmlBudgetReaderTest::compare(AssignmentRule* actual,
	uint estimateId, int conditions)
{
	QCOMPARE(actual->estimateId(), estimateId);
	QCOMPARE(actual->conditionCount(), conditions);
}

//------------------------------------------------------------------------------
void XmlBudgetReaderTest::compare(AssignmentRule* actual, uint ruleId,
	uint estimateId, int conditions)
{
	QCOMPARE(actual->ruleId(), ruleId);
	QCOMPARE(actual->estimateId(), estimateId);
	QCOMPARE(actual->conditionCount(), conditions);
}

//------------------------------------------------------------------------------
void XmlBudgetReaderTest::compare(const AssignmentRule::Condition actual,
	AssignmentRule::Field field, AssignmentRule::Operator oper,
	bool sensitive, const QString& value)
{
	QCOMPARE(actual.field, field);
	QCOMPARE(actual.op, oper);
	QCOMPARE(actual.sensitive, sensitive);
	QCOMPARE(actual.value, value);
}

//------------------------------------------------------------------------------
void XmlBudgetReaderTest::readInvalidXml()
{
	XmlBudgetReader reader;

	// Make sure no budget was read
	QCOMPARE(reader.read(invalidXml), false);
	QCOMPARE(reader.lastReadBudget().isNull(), true);
}

//------------------------------------------------------------------------------
void XmlBudgetReaderTest::readNonBudgetXml()
{
	XmlBudgetReader reader;

	// Make sure no budget was read
	QCOMPARE(reader.read(otherXml), false);
	QCOMPARE(reader.lastReadBudget().isNull(), true);
}

//------------------------------------------------------------------------------
void XmlBudgetReaderTest::readOldVersion()
{
	XmlBudgetReader reader;

	// Make sure no budget was read
	QCOMPARE(reader.read(oldVersion), false);
	QCOMPARE(reader.lastReadBudget().isNull(), true);
}

//------------------------------------------------------------------------------
void XmlBudgetReaderTest::readVersion4Budget()
{
	XmlBudgetReader reader;

	// Make sure budget was read successfully
	QCOMPARE(reader.read(version4), true);
	QCOMPARE(reader.lastReadBudget().isNull(), false);

	// Make sure budget was read correctly
	QSharedPointer<Budget> budget = reader.lastReadBudget();
	QCOMPARE(budget->name(), QString("Version 4 Budget"));
	QCOMPARE(budget->initialBalance(), Money(14500, "USD"));

	BudgetingPeriod::Parameters period = budget->budgetingPeriod()->parameters();
	QCOMPARE(period.type, BudgetingPeriod::CustomDateRange);
	QCOMPARE(period.param1.toDate(), QDate(2013,7,1));
	QCOMPARE(period.param2.toDate(), QDate(2013,8,1));

	// Make sure estimates were read correctly
	QSharedPointer<Estimate> rootEstimate = budget->estimates();
	compare(rootEstimate.data(), 0, "Root", "",
		Estimate::Root, Money(), QDate(), false, 2);

	Estimate* income = rootEstimate->childAt(0);
	compare(income, 122523, "Income Estimates", "",
		Estimate::Expense, Money(), QDate(), false, 1);

	Estimate* salary = income->childAt(0);
	compare(salary, 789234552, "Salary", "paychecks",
		Estimate::Expense, Money(2400, "USD"), QDate(), false, 0);

	Estimate* expense = rootEstimate->childAt(1);
	compare(expense, 872231, "Expense Estimates", "",
		Estimate::Expense, Money(), QDate(), false, 1);

	Estimate* utilities = expense->childAt(0);
	compare(utilities, 87222211, "Utilities", "",
		Estimate::Expense, Money(), QDate(), false, 2);

	Estimate* water = utilities->childAt(0);
	compare(water, 875211115, "Water", "",
		Estimate::Expense, Money(45, "USD"), QDate(2013, 7, 18), false, 0);

	Estimate* gas = utilities->childAt(1);
	compare(gas, 87111123, "Gas", "",
		Estimate::Expense, Money(114.23, "USD"), QDate(), true, 0);

	// Make sure rules were read correctly
	QSharedPointer<AssignmentRules> rules = budget->rules();
	QCOMPARE(rules->size(), 2);

	AssignmentRule* rule1 = rules->at(0);
	compare(rule1, 875211115, 1);
	compare(rule1->conditionAt(0), AssignmentRule::DepositAccount,
		AssignmentRule::EndsWith, false, "expense_acct");

	AssignmentRule* rule2 = rules->at(1);
	compare(rule2, 780234552, 2);
	compare(rule2->conditionAt(0), AssignmentRule::WithdrawalAccount,
		AssignmentRule::StringEquals, false, "income_acct");
	compare(rule2->conditionAt(1), AssignmentRule::Memo,
		AssignmentRule::StringEquals, true, "vAluE");
}

}

