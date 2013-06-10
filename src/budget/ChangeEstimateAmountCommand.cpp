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
#include "budget/ChangeEstimateAmountCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int ChangeEstimateAmountCommand::ID = 442523535;

//------------------------------------------------------------------------------
ChangeEstimateAmountCommand::ChangeEstimateAmountCommand(
		EstimatePointerMap estimates, uint estimateId,
		const Money& oldAmount, const Money& newAmount,
		QUndoCommand* parent)
	: QUndoCommand(parent),
	  estimates(estimates), estimateId(estimateId),
	  oldAmount(oldAmount), newAmount(newAmount)
{ }

//------------------------------------------------------------------------------
int ChangeEstimateAmountCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool ChangeEstimateAmountCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	// Only merge if change is for the same estimate
	uint otherId =
		static_cast<const ChangeEstimateAmountCommand*>(command)->estimateId;
	if (otherId != estimateId)
		return false;

	// Use new amount from the merged command
	newAmount = static_cast<const ChangeEstimateAmountCommand*>(command)->newAmount;
	return true;
}

//------------------------------------------------------------------------------
void ChangeEstimateAmountCommand::redo()
{
	if (estimates->contains(estimateId))
	{
		estimates->value(estimateId)->setAmount(newAmount);
	}
}

//------------------------------------------------------------------------------
void ChangeEstimateAmountCommand::undo()
{
	if (estimates->contains(estimateId))
	{
		estimates->value(estimateId)->setAmount(oldAmount);
	}
}

}


