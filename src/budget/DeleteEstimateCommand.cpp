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
#include "budget/DeleteEstimateCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int DeleteEstimateCommand::ID = 451266234;

//------------------------------------------------------------------------------
DeleteEstimateCommand::DeleteEstimateCommand(
		Estimate* root, uint estimateId,
		QUndoCommand* parent)
	: QUndoCommand(parent),
	  root(root), estimateId(estimateId)
{
	index = -1;

	Estimate* estimate = root->find(estimateId);
	if (estimate)
	{
		parentId = estimate->parentEstimate()->estimateId();
		name = estimate->name;
		description = estimate->description;
		type = estimate->type;
		amount = estimate->amount;
		dueDate = estimate->dueDate;
		finished = estimate->finished;

		Estimate* parentEstimate = root->find(parentId);
		if (parentEstimate)
		{
			index = parentEstimate->indexOf(estimate);
		}
	}
}

//------------------------------------------------------------------------------
int DeleteEstimateCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool DeleteEstimateCommand::mergeWith(const QUndoCommand* command)
{
	// This command can never be merged
	return false;
}

//------------------------------------------------------------------------------
void DeleteEstimateCommand::redo()
{
	Estimate* estimate = root->find(estimateId);
	if (estimate)
	{
		estimate->deleteSelf();
	}
}

//------------------------------------------------------------------------------
void DeleteEstimateCommand::undo()
{
	Estimate* parent = root->find(parentId);
	if (parent)
	{
		parent->createChild(estimateId,
			name, description, type, amount, dueDate, finished, index);
	}
}

}


