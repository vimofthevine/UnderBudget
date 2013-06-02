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
#include "budget/ChangePeriodParamsCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int ChangePeriodParamsCommand::ID = 1262353522;

//------------------------------------------------------------------------------
ChangePeriodParamsCommand::ChangePeriodParamsCommand(BudgetingPeriod* period,
		const BudgetingPeriod::Parameters& oldParams,
		const BudgetingPeriod::Parameters& newParams,
		QUndoCommand* parent)
	: QUndoCommand(parent), period(period),
	oldParams(oldParams), newParams(newParams)
{ }

//------------------------------------------------------------------------------
int ChangePeriodParamsCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool ChangePeriodParamsCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	// Use new params from the merged command
	newParams = static_cast<const ChangePeriodParamsCommand*>(command)->newParams;
	return true;
}

//------------------------------------------------------------------------------
void ChangePeriodParamsCommand::redo()
{
	period->setParams(newParams);
}

//------------------------------------------------------------------------------
void ChangePeriodParamsCommand::undo()
{
	period->setParams(oldParams);
}

}

