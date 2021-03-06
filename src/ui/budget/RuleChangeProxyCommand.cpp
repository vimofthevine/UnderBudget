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
#include <QtWidgets>

// UnderBudget include(s)
#include "ui/budget/AssignmentRulesModel.hpp"
#include "ui/budget/RuleChangeProxyCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
RuleChangeProxyCommand::RuleChangeProxyCommand(AssignmentRulesModel* model,
		uint ruleId, QUndoCommand* cmd, bool layout)
	: model(model), ruleId(ruleId), cmd(cmd), layoutChange(layout)
{ }

//------------------------------------------------------------------------------
int RuleChangeProxyCommand::id() const
{
	return 5633251;
}

//------------------------------------------------------------------------------
bool RuleChangeProxyCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	return cmd->mergeWith(static_cast<const RuleChangeProxyCommand*>(command)->cmd);
}

//------------------------------------------------------------------------------
void RuleChangeProxyCommand::redo()
{
	if (layoutChange)
	{
		model->emitLayoutAboutToBeChanged();
	}

	cmd->redo();
	model->emitDataChanged(model->indexFor(ruleId));

	if (layoutChange)
	{
		model->emitLayoutChanged();
	}
}

//------------------------------------------------------------------------------
void RuleChangeProxyCommand::undo()
{
	if (layoutChange)
	{
		model->emitLayoutAboutToBeChanged();
	}

	cmd->undo();
	model->emitDataChanged(model->indexFor(ruleId));

	if (layoutChange)
	{
		model->emitLayoutChanged();
	}
}

}

