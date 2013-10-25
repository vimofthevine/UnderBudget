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
#include "budget/Balance.hpp"
#include "budget/UpdateContributorCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int UpdateContributorCommand::ID = 8723464;

//------------------------------------------------------------------------------
UpdateContributorCommand::UpdateContributorCommand(Balance* balance, int index,
		const Balance::Contributor& contributor, QUndoCommand* parent)
	: QUndoCommand(parent), balance(balance),
	  index(index), newContributor(contributor)
{
	oldContributor = balance->contributorAt(index);
}

//------------------------------------------------------------------------------
int UpdateContributorCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool UpdateContributorCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	// Only merge if change is for the same contributor index
	int otherIndex =
		static_cast<const UpdateContributorCommand*>(command)->index;
	if (otherIndex != index)
		return false;

	// Use new contributor parameters from the merged command
	newContributor = static_cast<const UpdateContributorCommand*>(command)->newContributor;
	return true;
}

//------------------------------------------------------------------------------
void UpdateContributorCommand::redo()
{
	balance->updateContributor(newContributor, index);
}

//------------------------------------------------------------------------------
void UpdateContributorCommand::undo()
{
	balance->updateContributor(oldContributor, index);
}

}

