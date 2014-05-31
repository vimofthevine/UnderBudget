/*
 * Copyright 2014 Kyle Treubig
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
#include "budget/ChangeEstimateDueDateOffsetCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int ChangeEstimateDueDateOffsetCommand::ID = 44523752;

//------------------------------------------------------------------------------
ChangeEstimateDueDateOffsetCommand::ChangeEstimateDueDateOffsetCommand(
		Estimate* root, uint estimateId,
		int oldDateOffset, int newDateOffset,
		QUndoCommand* parent)
	: QUndoCommand(parent),
	  root(root), estimateId(estimateId),
	  oldDateOffset(oldDateOffset), newDateOffset(newDateOffset)
{ }

//------------------------------------------------------------------------------
int ChangeEstimateDueDateOffsetCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool ChangeEstimateDueDateOffsetCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	// Only merge if change is for the same estimate
	uint otherId =
		static_cast<const ChangeEstimateDueDateOffsetCommand*>(command)->estimateId;
	if (otherId != estimateId)
		return false;

	// Use new due date from the merged command
	newDateOffset = static_cast<const ChangeEstimateDueDateOffsetCommand*>(command)
		->newDateOffset;
	return true;
}

//------------------------------------------------------------------------------
void ChangeEstimateDueDateOffsetCommand::redo()
{
	Estimate* estimate = root->find(estimateId);
	if (estimate)
	{
		estimate->setDueDateOffset(newDateOffset);
	}
}

//------------------------------------------------------------------------------
void ChangeEstimateDueDateOffsetCommand::undo()
{
	Estimate* estimate = root->find(estimateId);
	if (estimate)
	{
		estimate->setDueDateOffset(oldDateOffset);
	}
}

}


