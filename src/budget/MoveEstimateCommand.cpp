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
		Estimate* root, uint estimateId,
		uint newParentId, int newIndex,
		QUndoCommand* parent)
	: QUndoCommand(parent),
	  root(root), estimateId(estimateId),
	  newParentId(newParentId), newIndex(newIndex)
{
	Estimate* estimate = root->find(estimateId);
	if (estimate)
	{
		Estimate* oldParent = estimate->parentEstimate();
		oldParentId = oldParent->estimateId();
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
	Estimate* estimate = root->find(estimateId);
	if (estimate)
	{
		estimate->moveTo(newParentId, newIndex);
	}
}

//------------------------------------------------------------------------------
void MoveEstimateCommand::undo()
{
	Estimate* estimate = root->find(estimateId);
	if (estimate)
	{
		estimate->moveTo(oldParentId, oldIndex);
	}
}

}


