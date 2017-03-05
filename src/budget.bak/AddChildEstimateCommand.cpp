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
#include "budget/AddChildEstimateCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int AddChildEstimateCommand::ID = 451234686;

//------------------------------------------------------------------------------
AddChildEstimateCommand::AddChildEstimateCommand(
		Estimate* root, uint parentId,
		QUndoCommand* parent)
	: QUndoCommand(parent),
	  root(root), parentId(parentId), childId(0)
{
	Estimate* parentEstimate = root->find(parentId);
	if (parentEstimate)
	{
		parentAmount = parentEstimate->amount;
		parentDueDateOffset = parentEstimate->dueDateOffset;
		parentFinished = parentEstimate->finished;
	}
}

//------------------------------------------------------------------------------
int AddChildEstimateCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool AddChildEstimateCommand::mergeWith(const QUndoCommand* command)
{
	// This command can never be merged
	return false;
}

//------------------------------------------------------------------------------
void AddChildEstimateCommand::redo()
{
	Estimate* parent = root->find(parentId);
	if (parent)
	{
		childId = parent->createChild(childId);
	}
}

//------------------------------------------------------------------------------
void AddChildEstimateCommand::undo()
{
	if (childId)
	{
		Estimate* child = root->find(childId);
		if (child)
		{
			child->deleteSelf();
		}
	}

	Estimate* parent = root->find(parentId);
	if (parent)
	{
		parent->setAmount(parentAmount);
		parent->setDueDateOffset(parentDueDateOffset);
		parent->setFinishedState(parentFinished);
	}
}

}


