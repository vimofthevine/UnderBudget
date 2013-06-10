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
#include "budget/ChangeEstimateFinishedCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int ChangeEstimateFinishedCommand::ID = 44232135;

//------------------------------------------------------------------------------
ChangeEstimateFinishedCommand::ChangeEstimateFinishedCommand(
		QHash<uint,QSharedPointer<Estimate> >* estimates,
		uint estimateId, bool oldState, bool newState,
		QUndoCommand* parent)
	: QUndoCommand(parent),
	  estimates(estimates), estimateId(estimateId),
	  oldState(oldState), newState(newState)
{ }

//------------------------------------------------------------------------------
int ChangeEstimateFinishedCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool ChangeEstimateFinishedCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	// Only merge if change is for the same estimate
	uint otherId =
		static_cast<const ChangeEstimateFinishedCommand*>(command)->estimateId;
	if (otherId != estimateId)
		return false;

	// Use new finished state from the merged command
	newState = static_cast<const ChangeEstimateFinishedCommand*>(command)->newState;
	return true;
}

//------------------------------------------------------------------------------
void ChangeEstimateFinishedCommand::redo()
{
	if (estimates->contains(estimateId))
	{
		estimates->value(estimateId)->setFinishedState(newState);
	}
}

//------------------------------------------------------------------------------
void ChangeEstimateFinishedCommand::undo()
{
	if (estimates->contains(estimateId))
	{
		estimates->value(estimateId)->setFinishedState(oldState);
	}
}

}


