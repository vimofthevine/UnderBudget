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
		EstimatePointerMap estimates, uint estimateId,
		QUndoCommand* parent)
	: QUndoCommand(parent),
	  estimates(estimates), estimateId(estimateId)
{
	if (estimates->contains(estimateId))
	{
		QSharedPointer<Estimate> estimate = estimates->value(estimateId);
		parentId = estimate->parent;
		name = estimate->name;
		description = estimate->description;
		type = estimate->type;
		amount = estimate->amount;
		dueDate = estimate->dueDate;
		finished = estimate->finished;
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
	if (estimates->contains(estimateId))
	{
		estimates->value(estimateId)->deleteSelf();
	}
}

//------------------------------------------------------------------------------
void DeleteEstimateCommand::undo()
{
	if (estimates->contains(parentId))
	{
		estimates->value(parentId)->createChild(estimateId,
			name, description, type, amount, dueDate, finished);
	}
}

}


