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
#include "budget/RemoveRuleCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int RemoveRuleCommand::ID = 82132433;

//------------------------------------------------------------------------------
RemoveRuleCommand::RemoveRuleCommand(AssignmentRules* rules, int index,
		uint ruleId, uint estimateId,
		const QList<AssignmentRule::Condition>& conditions, QUndoCommand* parent)
	: QUndoCommand(parent), rules(rules), index(index), ruleId(ruleId),
	  estimateId(estimateId), conditions(conditions)
{ }

//------------------------------------------------------------------------------
int RemoveRuleCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool RemoveRuleCommand::mergeWith(const QUndoCommand* command)
{
	// This command can never be merged
	return false;
}

//------------------------------------------------------------------------------
void RemoveRuleCommand::redo()
{
	rules->remove(index);
}

//------------------------------------------------------------------------------
void RemoveRuleCommand::undo()
{
	rules->insert(index, ruleId, estimateId, conditions);
}

}

