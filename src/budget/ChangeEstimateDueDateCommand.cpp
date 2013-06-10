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
#include "budget/ChangeEstimateDueDateCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int ChangeEstimateDueDateCommand::ID = 44523352;

//------------------------------------------------------------------------------
ChangeEstimateDueDateCommand::ChangeEstimateDueDateCommand(
		EstimatePointerMap estimates, uint estimateId,
		const QDate& oldDate, const QDate& newDate,
		QUndoCommand* parent)
	: QUndoCommand(parent),
	  estimates(estimates), estimateId(estimateId), oldDate(oldDate), newDate(newDate)
{ }

//------------------------------------------------------------------------------
int ChangeEstimateDueDateCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool ChangeEstimateDueDateCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	// Only merge if change is for the same estimate
	uint otherId =
		static_cast<const ChangeEstimateDueDateCommand*>(command)->estimateId;
	if (otherId != estimateId)
		return false;

	// Use new due date from the merged command
	newDate = static_cast<const ChangeEstimateDueDateCommand*>(command)->newDate;
	return true;
}

//------------------------------------------------------------------------------
void ChangeEstimateDueDateCommand::redo()
{
	if (estimates->contains(estimateId))
	{
		estimates->value(estimateId)->setDueDate(newDate);
	}
}

//------------------------------------------------------------------------------
void ChangeEstimateDueDateCommand::undo()
{
	if (estimates->contains(estimateId))
	{
		estimates->value(estimateId)->setDueDate(oldDate);
	}
}

}


