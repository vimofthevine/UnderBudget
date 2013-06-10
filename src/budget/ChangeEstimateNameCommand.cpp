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
#include "budget/ChangeEstimateNameCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int ChangeEstimateNameCommand::ID = 445233218;

//------------------------------------------------------------------------------
ChangeEstimateNameCommand::ChangeEstimateNameCommand(
		QHash<uint,QSharedPointer<Estimate> >* estimates,
		uint estimateId, const QString& oldName, const QString& newName,
		QUndoCommand* parent)
	: QUndoCommand(parent),
	  estimates(estimates), estimateId(estimateId),
	  oldName(oldName), newName(newName)
{ }

//------------------------------------------------------------------------------
int ChangeEstimateNameCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool ChangeEstimateNameCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	// Only merge if change is for the same estimate
	uint otherId =
		static_cast<const ChangeEstimateNameCommand*>(command)->estimateId;
	if (otherId != estimateId)
		return false;

	// Use new name from the merged command
	newName = static_cast<const ChangeEstimateNameCommand*>(command)->newName;
	return true;
}

//------------------------------------------------------------------------------
void ChangeEstimateNameCommand::redo()
{
	if (estimates->contains(estimateId))
	{
		estimates->value(estimateId)->setName(newName);
	}
}

//------------------------------------------------------------------------------
void ChangeEstimateNameCommand::undo()
{
	if (estimates->contains(estimateId))
	{
		estimates->value(estimateId)->setName(oldName);
	}
}

}


