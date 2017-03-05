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
#include "budget/AddConditionCommand.hpp"
#include "budget/AssignmentRule.hpp"
#include "budget/AssignmentRules.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int AddConditionCommand::ID = 723432;

//------------------------------------------------------------------------------
AddConditionCommand::AddConditionCommand(AssignmentRules* rules, uint ruleId,
		QUndoCommand* parent)
	: QUndoCommand(parent), rules(rules), ruleId(ruleId), index(-1)
{
	AssignmentRule* rule = rules->find(ruleId);
	if (rule)
	{
		index = rule->conditionCount();
	}
}

//------------------------------------------------------------------------------
int AddConditionCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool AddConditionCommand::mergeWith(const QUndoCommand* command)
{
	// This command can never be merged
	return false;
}

//------------------------------------------------------------------------------
void AddConditionCommand::redo()
{
	AssignmentRule* rule = rules->find(ruleId);
	if (rule)
	{
		rule->addCondition(AssignmentRule::Condition(), index);
	}
}

//------------------------------------------------------------------------------
void AddConditionCommand::undo()
{
	AssignmentRule* rule = rules->find(ruleId);
	if (rule)
	{
		rule->deleteCondition(index);
	}
}

}

