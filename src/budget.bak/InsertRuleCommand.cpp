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
#include "budget/InsertRuleCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int InsertRuleCommand::ID = 82132432;

//------------------------------------------------------------------------------
InsertRuleCommand::InsertRuleCommand(AssignmentRules* rules, int index,
		uint ruleId, uint estimateId,
		const QList<AssignmentRule::Condition>& conditions, QUndoCommand* parent)
	: QUndoCommand(parent), rules(rules), index(index), ruleId(ruleId),
	  estimateId(estimateId), conditions(conditions)
{ }

//------------------------------------------------------------------------------
int InsertRuleCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool InsertRuleCommand::mergeWith(const QUndoCommand* command)
{
	// This command can never be merged
	return false;
}

//------------------------------------------------------------------------------
void InsertRuleCommand::redo()
{
	rules->insert(index, ruleId, estimateId, conditions);
}

//------------------------------------------------------------------------------
void InsertRuleCommand::undo()
{
	rules->remove(index);
}

}

