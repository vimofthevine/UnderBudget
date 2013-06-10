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
#include "budget/ChangeEstimateTypeCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int ChangeEstimateTypeCommand::ID = 448722313;

//------------------------------------------------------------------------------
ChangeEstimateTypeCommand::ChangeEstimateTypeCommand(
		QHash<uint,QSharedPointer<Estimate> >* estimates,
		uint estimateId, Estimate::Type oldType, Estimate::Type newType,
		QUndoCommand* parent)
	: QUndoCommand(parent),
	  estimates(estimates), estimateId(estimateId),
	  oldType(oldType), newType(newType)
{ }

//------------------------------------------------------------------------------
int ChangeEstimateTypeCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool ChangeEstimateTypeCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	// Only merge if change is for the same estimate
	uint otherId =
		static_cast<const ChangeEstimateTypeCommand*>(command)->estimateId;
	if (otherId != estimateId)
		return false;

	// Use new name from the merged command
	newType = static_cast<const ChangeEstimateTypeCommand*>(command)->newType;
	return true;
}

//------------------------------------------------------------------------------
void ChangeEstimateTypeCommand::redo()
{
	if (estimates->contains(estimateId))
	{
		estimates->value(estimateId)->setType(newType);
	}
}

//------------------------------------------------------------------------------
void ChangeEstimateTypeCommand::undo()
{
	if (estimates->contains(estimateId))
	{
		estimates->value(estimateId)->setType(oldType);
	}
}

}


