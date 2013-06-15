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
#include "budget/MoveEstimateCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int MoveEstimateCommand::ID = 451423256;

//------------------------------------------------------------------------------
MoveEstimateCommand::MoveEstimateCommand(
		EstimatePointerMap estimates, uint estimateId,
		QSharedPointer<Estimate> newParent, int newIndex,
		QUndoCommand* parent)
	: QUndoCommand(parent),
	  estimates(estimates), estimateId(estimateId), newIndex(newIndex)
{
	if (estimates->contains(estimateId))
	{
		QSharedPointer<Estimate> estimate = estimates->value(estimateId);
		QSharedPointer<Estimate> oldParent = estimate->parentEstimate();

		oldParentId = oldParent->estimateId();
		newParentId = newParent->estimateId();

		oldIndex = oldParent->indexOf(estimate);
	}
}

//------------------------------------------------------------------------------
int MoveEstimateCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool MoveEstimateCommand::mergeWith(const QUndoCommand* command)
{
	// This command can never be merged
	return false;
}

//------------------------------------------------------------------------------
void MoveEstimateCommand::redo()
{
	if (estimates->contains(estimateId))
	{
		estimates->value(estimateId)->moveTo(newParentId, newIndex);
	}
}

//------------------------------------------------------------------------------
void MoveEstimateCommand::undo()
{
	if (estimates->contains(estimateId))
	{
		estimates->value(estimateId)->moveTo(oldParentId, oldIndex);
	}
}

}


