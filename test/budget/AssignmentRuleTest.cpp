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

}

