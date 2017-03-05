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
#include "budget/AssignmentRule.hpp"
#include "budget/AssignmentRules.hpp"
#include "budget/UpdateConditionCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int UpdateConditionCommand::ID = 723434;

//------------------------------------------------------------------------------
UpdateConditionCommand::UpdateConditionCommand(AssignmentRules* rules,
		uint ruleId, int index, const AssignmentRule::Condition& condition,
		QUndoCommand* parent)
	: QUndoCommand(parent), rules(rules), ruleId(ruleId), index(index),
	  newCondition(condition)
{
	AssignmentRule* rule = rules->find(ruleId);
	if (rule)
	{
		oldCondition = rule->conditionAt(index);
	}
}

//------------------------------------------------------------------------------
int UpdateConditionCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool UpdateConditionCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	// Only merge if change is for the same rule and condition index
	uint otherId =
		static_cast<const UpdateConditionCommand*>(command)->ruleId;
	int otherIndex =
		static_cast<const UpdateConditionCommand*>(command)->index;
	if (otherId != ruleId)
		return false;
	if (otherIndex != index)
		return false;

	// Use new condition parameters from the merged command
	newCondition = static_cast<const UpdateConditionCommand*>(command)->newCondition;
	return true;
}

//------------------------------------------------------------------------------
void UpdateConditionCommand::redo()
{
	AssignmentRule* rule = rules->find(ruleId);
	if (rule)
	{
		rule->updateCondition(newCondition, index);
	}
}

//------------------------------------------------------------------------------
void UpdateConditionCommand::undo()
{
	AssignmentRule* rule = rules->find(ruleId);
	if (rule)
	{
		rule->updateCondition(oldCondition, index);
	}
}

}


