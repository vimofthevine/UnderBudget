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
		EstimatePointerMap estimates, uint parentId,
		QUndoCommand* parent)
	: QUndoCommand(parent),
	  estimates(estimates), parentId(parentId), childId(0)
{
	if (estimates->contains(parentId))
	{
		QSharedPointer<Estimate> parent = estimates->value(parentId);
		parentAmount = parent->amount;
		parentDueDate = parent->dueDate;
		parentFinished = parent->finished;
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
	if ( ! childId && estimates->contains(parentId))
	{
		childId = estimates->value(parentId)->createChild();
	}
}

//------------------------------------------------------------------------------
void AddChildEstimateCommand::undo()
{
	if (childId && estimates->contains(childId))
	{
		estimates->value(childId)->deleteSelf();
	}

	if (estimates->contains(parentId))
	{
		QSharedPointer<Estimate> parent = estimates->value(parentId);
		parent->setAmount(parentAmount);
		parent->setDueDate(parentDueDate);
		parent->setFinishedState(parentFinished);
	}
}

}


