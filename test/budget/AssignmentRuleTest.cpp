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
#include <QUndoCommand>

// UnderBudget include(s)
#include "AssignmentRuleTest.hpp"
#include "budget/AssignmentRule.hpp"
#include "budget/AssignmentRules.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::AssignmentRuleTest)

namespace QTest {

//------------------------------------------------------------------------------
template<>
char* toString(const ub::AssignmentRule::Field& field)
{
	switch (field)
	{
	case ub::AssignmentRule::Date:
		return toString("Date");
	case ub::AssignmentRule::Amount:
		return toString("Amount");
	case ub::AssignmentRule::Payee:
		return toString("Payee");
	case ub::AssignmentRule::Memo:
		return toString("Memo");
	case ub::AssignmentRule::DepositAccount:
		return toString("DepositAccount");
	case ub::AssignmentRule::WithdrawalAccount:
		return toString("WithdrawalAccount");
	default:
		return toString("Unknown field");
	}
}

//------------------------------------------------------------------------------
template<>
char* toString(const ub::AssignmentRule::Operator& oper)
{
	switch (oper)
	{
	case ub::AssignmentRule::BeginsWith:
		return toString("BeginsWith");
	case ub::AssignmentRule::EndsWith:
		return toString("EndsWith");
	case ub::AssignmentRule::StringEquals:
		return toString("StringEquals");
	case ub::AssignmentRule::Contains:
		return toString("Contains");
	case ub::AssignmentRule::Before:
		return toString("Before");
	case ub::AssignmentRule::After:
		return toString("After");
	case ub::AssignmentRule::DateEquals:
		return toString("DateEquals");
	case ub::AssignmentRule::LessThan:
		return toString("LessThan");
	case ub::AssignmentRule::LessThanOrEqual:
		return toString("LessThanOrEqual");
	case ub::AssignmentRule::GreaterThan:
		return toString("GreaterThan");
	case ub::AssignmentRule::GreaterThanOrEqual:
		return toString("GreaterThanOrEqual");
	case ub::AssignmentRule::AmountEquals:
		return toString("AmountEquals");
	default:
		return toString("Unknown operator");
	}
}

}

namespace ub {

//------------------------------------------------------------------------------
static const uint NO_COND = 1111;
static const uint ONE_COND = 2222;
static const uint MULTI_COND = 3333;
static const uint ESTIMATE_1 = 1234;
static const uint ESTIMATE_2 = 2234;
static const uint ESTIMATE_3 = 3234;

//------------------------------------------------------------------------------
void AssignmentRuleTest::init()
{
	rules = AssignmentRules::create();
	QList<AssignmentRule::Condition> conditions;

	ruleWithNoConditions = rules->createRule(NO_COND, ESTIMATE_1, conditions);

	conditions << AssignmentRule::Condition(AssignmentRule::Payee, AssignmentRule::StringEquals, true, "Condition1");
	ruleWithOneCondition = rules->createRule(ONE_COND, ESTIMATE_2, conditions);

	conditions << AssignmentRule::Condition(AssignmentRule::DepositAccount, AssignmentRule::BeginsWith, false, "Condition2");
	ruleWithMultipleConditions = rules->createRule(MULTI_COND, ESTIMATE_2, conditions);
}

//------------------------------------------------------------------------------
void AssignmentRuleTest::conditionCount()
{
	QCOMPARE(ruleWithNoConditions->conditionCount(), 0);
	QCOMPARE(ruleWithOneCondition->conditionCount(), 1);
	QCOMPARE(ruleWithMultipleConditions->conditionCount(), 2);
}

//------------------------------------------------------------------------------
void AssignmentRuleTest::conditionAt()
{
	// Test out-of-bound attempts
	QCOMPARE(ruleWithNoConditions->conditionAt(0).field, AssignmentRule::FieldNotDefined);
	QCOMPARE(ruleWithOneCondition->conditionAt(-1).field, AssignmentRule::FieldNotDefined);
	QCOMPARE(ruleWithOneCondition->conditionAt(1).field, AssignmentRule::FieldNotDefined);

	QCOMPARE(ruleWithOneCondition->conditionAt(0).field, AssignmentRule::Payee);
	QCOMPARE(ruleWithMultipleConditions->conditionAt(0).field, AssignmentRule::Payee);
	QCOMPARE(ruleWithMultipleConditions->conditionAt(1).field, AssignmentRule::DepositAccount);
}

//------------------------------------------------------------------------------
void AssignmentRuleTest::addCondition_data()
{
	QTest::addColumn<uint>("ruleId");
	QTest::addColumn<int>("oldCount");
	QTest::addColumn<int>("newCount");
	QTest::addColumn<int>("newIndex");
	QTest::addColumn<int>("existingIndex");

	QTest::newRow("no-conditions") << NO_COND << 0 << 1 << 0 << -1;
	QTest::newRow("one-condition") << ONE_COND << 1 << 2 << 1 << 0;
	QTest::newRow("many-conditions") << MULTI_COND << 2 << 3 << 2 << 1;
}

//------------------------------------------------------------------------------
void AssignmentRuleTest::addCondition()
{
	QFETCH(uint, ruleId);
	QFETCH(int, oldCount);
	QFETCH(int, newCount);
	QFETCH(int, newIndex);
	QFETCH(int, existingIndex);

	AssignmentRule* rule = rules->find(ruleId);
	QUndoCommand* cmd = rule->addCondition();

	QCOMPARE(rule->conditionCount(), oldCount);
	AssignmentRule::Field field;
	if (existingIndex >= 0)
	{
		field = rule->conditionAt(existingIndex).field;
	}

	cmd->redo();
	QCOMPARE(rule->conditionCount(), newCount);
	QCOMPARE(rule->conditionAt(newIndex).field, AssignmentRule::FieldNotDefined);
	if (existingIndex >= 0)
	{
		QCOMPARE(rule->conditionAt(existingIndex).field, field);
	}

	cmd->undo();
	QCOMPARE(rule->conditionCount(), oldCount);
	if (existingIndex >= 0)
	{
		QCOMPARE(rule->conditionAt(existingIndex).field, field);
	}
}

//------------------------------------------------------------------------------
void AssignmentRuleTest::removeConditionFromRuleWithNone()
{
	AssignmentRule* rule(ruleWithNoConditions);
	QUndoCommand* cmd = rule->removeCondition(0);

	QCOMPARE(rule->conditionCount(), 0);
	cmd->redo();
	QCOMPARE(rule->conditionCount(), 0);
	cmd->undo();
	QCOMPARE(rule->conditionCount(), 0);
}

//------------------------------------------------------------------------------
void AssignmentRuleTest::removeConditionFromRuleWithOne()
{
	AssignmentRule* rule(ruleWithOneCondition);
	QUndoCommand* cmd = rule->removeCondition(0);

	QCOMPARE(rule->conditionCount(), 1);
	QCOMPARE(rule->conditionAt(0).field, AssignmentRule::Payee);
	cmd->redo();
	QCOMPARE(rule->conditionCount(), 0);
	QCOMPARE(rule->conditionAt(0).field, AssignmentRule::FieldNotDefined);
	cmd->undo();
	QCOMPARE(rule->conditionCount(), 1);
	QCOMPARE(rule->conditionAt(0).field, AssignmentRule::Payee);
}

//------------------------------------------------------------------------------
void AssignmentRuleTest::removeConditionFromRuleWithMany()
{
	AssignmentRule* rule(ruleWithMultipleConditions);
	QUndoCommand* cmd = rule->removeCondition(0);

	QCOMPARE(rule->conditionCount(), 2);
	QCOMPARE(rule->conditionAt(0).field, AssignmentRule::Payee);
	QCOMPARE(rule->conditionAt(1).field, AssignmentRule::DepositAccount);
	cmd->redo();
	QCOMPARE(rule->conditionCount(), 1);
	QCOMPARE(rule->conditionAt(0).field, AssignmentRule::DepositAccount);
	cmd->undo();
	QCOMPARE(rule->conditionCount(), 2);
	QCOMPARE(rule->conditionAt(0).field, AssignmentRule::Payee);
	QCOMPARE(rule->conditionAt(1).field, AssignmentRule::DepositAccount);
}

//------------------------------------------------------------------------------
void AssignmentRuleTest::updateCondition()
{
	AssignmentRule* rule(ruleWithMultipleConditions);
	QUndoCommand* cmd = rule->updateCondition(1,
		AssignmentRule::Condition(AssignmentRule::Date, AssignmentRule::Before, false, "5/5/13"));

	// Assert pre-conditions
	QCOMPARE(rule->conditionCount(), 2);
	QCOMPARE(rule->conditionAt(0).field, AssignmentRule::Payee);
	QCOMPARE(rule->conditionAt(1).field, AssignmentRule::DepositAccount);
	QCOMPARE(rule->conditionAt(1).op, AssignmentRule::BeginsWith);
	QCOMPARE(rule->conditionAt(1).value, QString("Condition2"));

	cmd->redo();
	QCOMPARE(rule->conditionCount(), 2);
	QCOMPARE(rule->conditionAt(0).field, AssignmentRule::Payee);
	QCOMPARE(rule->conditionAt(1).field, AssignmentRule::Date);
	QCOMPARE(rule->conditionAt(1).op, AssignmentRule::Before);
	QCOMPARE(rule->conditionAt(1).value, QString("5/5/13"));

	cmd->undo();
	QCOMPARE(rule->conditionCount(), 2);
	QCOMPARE(rule->conditionAt(0).field, AssignmentRule::Payee);
	QCOMPARE(rule->conditionAt(1).field, AssignmentRule::DepositAccount);
	QCOMPARE(rule->conditionAt(1).op, AssignmentRule::BeginsWith);
	QCOMPARE(rule->conditionAt(1).value, QString("Condition2"));
}

}

