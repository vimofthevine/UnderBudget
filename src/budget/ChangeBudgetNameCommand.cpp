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
#include "budget/ChangeBudgetNameCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int ChangeBudgetNameCommand::ID = 1262135353;

//------------------------------------------------------------------------------
ChangeBudgetNameCommand::ChangeBudgetNameCommand(Budget* budget,
		const QString& oldName, const QString& newName)
	: budget(budget), oldName(oldName), newName(newName)
{ }

//------------------------------------------------------------------------------
int ChangeBudgetNameCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool ChangeBudgetNameCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	// Use new name from the merged command
	newName = static_cast<const ChangeBudgetNameCommand*>(command)->newName;
	return true;
}

//------------------------------------------------------------------------------
void ChangeBudgetNameCommand::redo()
{
	budget->setName(newName);
}

//------------------------------------------------------------------------------
void ChangeBudgetNameCommand::undo()
{
	budget->setName(oldName);
}

}

