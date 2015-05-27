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
#include "ui/budget/RuleAddProxyCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
RuleAddProxyCommand::RuleAddProxyCommand(AssignmentRulesModel* model, int index,
		QUndoCommand* cmd)
	: model(model), index(index), cmd(cmd)
{ }

//------------------------------------------------------------------------------
int RuleAddProxyCommand::id() const
{
	return 7638253;
}

//------------------------------------------------------------------------------
bool RuleAddProxyCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	return cmd->mergeWith(static_cast<const RuleAddProxyCommand*>(command)->cmd);
}

//------------------------------------------------------------------------------
void RuleAddProxyCommand::redo()
{
	model->beginInsertRows(QModelIndex(), index, index);
	cmd->redo();
	model->endInsertRows();
}

//------------------------------------------------------------------------------
void RuleAddProxyCommand::undo()
{
	model->beginRemoveRows(QModelIndex(), index, index);
	cmd->undo();
	model->endRemoveRows();
}

}

