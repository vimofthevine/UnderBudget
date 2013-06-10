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
#include "budget/ChangeEstimateDescriptionCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int ChangeEstimateDescriptionCommand::ID = 446282623;

//------------------------------------------------------------------------------
ChangeEstimateDescriptionCommand::ChangeEstimateDescriptionCommand(
		QHash<uint,QSharedPointer<Estimate> >* estimates,
		uint estimateId, const QString& oldDescrip, const QString& newDescrip,
		QUndoCommand* parent)
	: QUndoCommand(parent),
	  estimates(estimates), estimateId(estimateId),
	  oldDescrip(oldDescrip), newDescrip(newDescrip)
{ }

//------------------------------------------------------------------------------
int ChangeEstimateDescriptionCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool ChangeEstimateDescriptionCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	// Only merge if change is for the same estimate
	uint otherId =
		static_cast<const ChangeEstimateDescriptionCommand*>(command)->estimateId;
	if (otherId != estimateId)
		return false;

	// Use new description from the merged command
	newDescrip = static_cast<const ChangeEstimateDescriptionCommand*>(command)->newDescrip;
	return true;
}

//------------------------------------------------------------------------------
void ChangeEstimateDescriptionCommand::redo()
{
	if (estimates->contains(estimateId))
	{
		estimates->value(estimateId)->setDescription(newDescrip);
	}
}

//------------------------------------------------------------------------------
void ChangeEstimateDescriptionCommand::undo()
{
	if (estimates->contains(estimateId))
	{
		estimates->value(estimateId)->setDescription(oldDescrip);
	}
}

}


