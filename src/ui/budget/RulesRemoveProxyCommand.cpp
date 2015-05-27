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
#include "ui/budget/RulesRemoveProxyCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
RulesRemoveProxyCommand::RulesRemoveProxyCommand(AssignmentRulesModel* model,
		QUndoCommand* cmd)
	: model(model), cmd(cmd)
{ }

//------------------------------------------------------------------------------
int RulesRemoveProxyCommand::id() const
{
	return 7638259;
}

//------------------------------------------------------------------------------
bool RulesRemoveProxyCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	return cmd->mergeWith(static_cast<const RulesRemoveProxyCommand*>(command)->cmd);
}

//------------------------------------------------------------------------------
void RulesRemoveProxyCommand::redo()
{
	// Remove all rows
	model->beginRemoveRows(QModelIndex(), 0, model->rowCount()-1);
	model->endRemoveRows();
	cmd->redo();
	// Re-insert all rows
	model->beginInsertRows(QModelIndex(), 0, model->rowCount()-1);
	model->endInsertRows();
}

//------------------------------------------------------------------------------
void RulesRemoveProxyCommand::undo()
{
	// Remove all rows
	model->beginRemoveRows(QModelIndex(), 0, model->rowCount()-1);
	model->endRemoveRows();
	cmd->undo();
	// Re-insert all rows
	model->beginInsertRows(QModelIndex(), 0, model->rowCount()-1);
	model->endInsertRows();
}

}

