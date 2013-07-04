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
#include "budget/MoveRuleCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int MoveRuleCommand::ID = 82132434;

//------------------------------------------------------------------------------
MoveRuleCommand::MoveRuleCommand(AssignmentRules* rules, int from, int to,
		QUndoCommand* parent)
	: QUndoCommand(parent), rules(rules), oldIndex(from), newIndex(to)
{ }

//------------------------------------------------------------------------------
int MoveRuleCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool MoveRuleCommand::mergeWith(const QUndoCommand* command)
{
	// This command can never be merged
	return false;
}

//------------------------------------------------------------------------------
void MoveRuleCommand::redo()
{
	rules->moveRule(oldIndex, newIndex);
}

//------------------------------------------------------------------------------
void MoveRuleCommand::undo()
{
	// In case original move was to end of the list
	int from = (newIndex >= rules->size()) ? rules->size() - 1 : newIndex;
	rules->moveRule(from, oldIndex);
}

}

