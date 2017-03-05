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
#include "AssignmentRulesTest.hpp"
#include "budget/AssignmentRule.hpp"
#include "budget/AssignmentRules.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::AssignmentRulesTest)

namespace ub {

//------------------------------------------------------------------------------
static const uint RULE_1 = 1111;
static const uint RULE_2 = 2222;
static const uint RULE_3 = 3333;
static const uint ESTIMATE_1 = 1234;
static const uint ESTIMATE_2 = 2234;
static const uint ESTIMATE_3 = 3234;

//------------------------------------------------------------------------------
void AssignmentRulesTest::init()
{
	rules = AssignmentRules::create();
	QList<AssignmentRule::Condition> conditions;

	rule1 = rules->createRule(RULE_1, ESTIMATE_1, conditions);
	rule2 = rules->createRule(RULE_2, ESTIMATE_2, conditions);
	rule3 = rules->createRule(RULE_3, ESTIMATE_3, conditions);
}

//------------------------------------------------------------------------------
void AssignmentRulesTest::indexOf()
{
	QCOMPARE(rules->indexOf(RULE_1), 0);
	QCOMPARE(rules->indexOf(RULE_2), 1);
	QCOMPARE(rules->indexOf(RULE_3), 2);
	QCOMPARE(rules->indexOf(4444), -1);
}

//------------------------------------------------------------------------------
void AssignmentRulesTest::atIndexRetrieval()
{
	QCOMPARE(rules->at(-1) == 0, true);
	QCOMPARE(rules->at(0), rule1);
	QCOMPARE(rules->at(1), rule2);
	QCOMPARE(rules->at(2), rule3);
	QCOMPARE(rules->at(3) == 0, true);
}

//------------------------------------------------------------------------------
void AssignmentRulesTest::find()
{
	QCOMPARE(rules->find(RULE_1), rule1);
	QCOMPARE(rules->find(RULE_2), rule2);
	QCOMPARE(rules->find(RULE_3), rule3);
	QCOMPARE(rules->find(9) == 0, true);
}

//------------------------------------------------------------------------------
void AssignmentRulesTest::findFor()
{
	AssignmentRule* rule4 = rules->createRule(4444, ESTIMATE_2,
		QList<AssignmentRule::Condition>());

	QCOMPARE(rules->findFor(ESTIMATE_1).size(), 1);
	QCOMPARE(rules->findFor(ESTIMATE_2).size(), 2);
	QCOMPARE(rules->findFor(ESTIMATE_3).size(), 1);

	QList<AssignmentRule*> found = rules->findFor(ESTIMATE_3);
	QCOMPARE(found.at(0), rule3);

	found = rules->findFor(ESTIMATE_2);
	// Most recently to least recently inserted
	QCOMPARE(found.at(0), rule4);
	QCOMPARE(found.at(1), rule2);
}

//------------------------------------------------------------------------------
void AssignmentRulesTest::addRule()
{
	QUndoCommand* cmd = rules->addRule(ESTIMATE_1);

	// Assert pre-conditions
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->findFor(ESTIMATE_1).size(), 1);
	QCOMPARE(rules->at(2)->estimateId(), ESTIMATE_3);
	QCOMPARE(rules->at(3) == 0, true);

	cmd->redo();
	QCOMPARE(rules->size(), 4);
	QCOMPARE(rules->findFor(ESTIMATE_1).size(), 2);
	QCOMPARE(rules->at(2)->estimateId(), ESTIMATE_3);
	QCOMPARE(rules->at(3)->estimateId(), ESTIMATE_1);

	cmd->undo();
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->findFor(ESTIMATE_1).size(), 1);
	QCOMPARE(rules->at(2)->estimateId(), ESTIMATE_3);
	QCOMPARE(rules->at(3) == 0, true);
}

//------------------------------------------------------------------------------
void AssignmentRulesTest::removeFirstRule()
{
	QUndoCommand* cmd = rules->removeAt(0);

	// Assert pre-conditions
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);
	QCOMPARE(rules->at(2)->ruleId(), RULE_3);
	QCOMPARE(rules->at(0)->estimateId(), ESTIMATE_1);

	cmd->redo();
	QCOMPARE(rules->size(), 2);
	QCOMPARE(rules->at(0)->ruleId(), RULE_2);
	QCOMPARE(rules->at(1)->ruleId(), RULE_3);

	cmd->undo();
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);
	QCOMPARE(rules->at(2)->ruleId(), RULE_3);
	QCOMPARE(rules->at(0)->estimateId(), ESTIMATE_1);
}

//------------------------------------------------------------------------------
void AssignmentRulesTest::removeLastRule()
{
	QUndoCommand* cmd = rules->removeAt(2);

	// Assert pre-conditions
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);
	QCOMPARE(rules->at(2)->ruleId(), RULE_3);
	QCOMPARE(rules->at(2)->estimateId(), ESTIMATE_3);

	cmd->redo();
	QCOMPARE(rules->size(), 2);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);

	cmd->undo();
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);
	QCOMPARE(rules->at(2)->ruleId(), RULE_3);
	QCOMPARE(rules->at(2)->estimateId(), ESTIMATE_3);
}

//------------------------------------------------------------------------------
void AssignmentRulesTest::removeRule()
{
	QUndoCommand* cmd = rules->removeAt(1);

	// Assert pre-conditions
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);
	QCOMPARE(rules->at(2)->ruleId(), RULE_3);
	QCOMPARE(rules->at(1)->estimateId(), ESTIMATE_2);

	cmd->redo();
	QCOMPARE(rules->size(), 2);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_3);

	cmd->undo();
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);
	QCOMPARE(rules->at(2)->ruleId(), RULE_3);
	QCOMPARE(rules->at(1)->estimateId(), ESTIMATE_2);
}

//------------------------------------------------------------------------------
void AssignmentRulesTest::removeByRuleId()
{
	QUndoCommand* cmd = rules->removeRule(RULE_2);

	// Assert pre-conditions
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);
	QCOMPARE(rules->at(2)->ruleId(), RULE_3);
	QCOMPARE(rules->at(1)->estimateId(), ESTIMATE_2);

	cmd->redo();
	QCOMPARE(rules->size(), 2);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_3);

	cmd->undo();
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);
	QCOMPARE(rules->at(2)->ruleId(), RULE_3);
	QCOMPARE(rules->at(1)->estimateId(), ESTIMATE_2);
}

//------------------------------------------------------------------------------
void AssignmentRulesTest::removeByEstimate()
{
	AssignmentRule* rule4 = rules->createRule(4444, ESTIMATE_2,
		QList<AssignmentRule::Condition>());
	QUndoCommand* cmd = rules->removeAll(ESTIMATE_2);

	// Assert pre-conditions
	QCOMPARE(rules->size(), 4);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);
	QCOMPARE(rules->at(2)->ruleId(), RULE_3);
	QCOMPARE(rules->at(3)->ruleId(), (uint) 4444);
	QCOMPARE(rules->at(1)->estimateId(), ESTIMATE_2);
	QCOMPARE(rules->at(3)->estimateId(), ESTIMATE_2);

	cmd->redo();
	QCOMPARE(rules->size(), 2);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_3);

	cmd->undo();
	QCOMPARE(rules->size(), 4);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);
	QCOMPARE(rules->at(2)->ruleId(), RULE_3);
	QCOMPARE(rules->at(3)->ruleId(), (uint) 4444);
	QCOMPARE(rules->at(1)->estimateId(), ESTIMATE_2);
	QCOMPARE(rules->at(3)->estimateId(), ESTIMATE_2);
}

//------------------------------------------------------------------------------
void AssignmentRulesTest::moveToBeginning()
{
	QUndoCommand* cmd = rules->move(2, 0);

	// Assert pre-conditions
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);
	QCOMPARE(rules->at(2)->ruleId(), RULE_3);

	cmd->redo();
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_3);
	QCOMPARE(rules->at(1)->ruleId(), RULE_1);
	QCOMPARE(rules->at(2)->ruleId(), RULE_2);

	cmd->undo();
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);
	QCOMPARE(rules->at(2)->ruleId(), RULE_3);
}

//------------------------------------------------------------------------------
void AssignmentRulesTest::moveToEnd()
{
	QUndoCommand* cmd = rules->move(0, 3);

	// Assert pre-conditions
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);
	QCOMPARE(rules->at(2)->ruleId(), RULE_3);

	cmd->redo();
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_2);
	QCOMPARE(rules->at(1)->ruleId(), RULE_3);
	QCOMPARE(rules->at(2)->ruleId(), RULE_1);

	cmd->undo();
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);
	QCOMPARE(rules->at(2)->ruleId(), RULE_3);
}

//------------------------------------------------------------------------------
void AssignmentRulesTest::moveToMiddle()
{
	QUndoCommand* cmd = rules->move(2, 1);

	// Assert pre-conditions
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);
	QCOMPARE(rules->at(2)->ruleId(), RULE_3);

	cmd->redo();
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_3);
	QCOMPARE(rules->at(2)->ruleId(), RULE_2);

	cmd->undo();
	QCOMPARE(rules->size(), 3);
	QCOMPARE(rules->at(0)->ruleId(), RULE_1);
	QCOMPARE(rules->at(1)->ruleId(), RULE_2);
	QCOMPARE(rules->at(2)->ruleId(), RULE_3);
}

}

