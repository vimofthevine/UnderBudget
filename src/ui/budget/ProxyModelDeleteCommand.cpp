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
#include "ui/budget/EstimateModel.hpp"
#include "ui/budget/ProxyModelDeleteCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
ProxyModelDeleteCommand::ProxyModelDeleteCommand(EstimateModel* model,
		uint parentId, uint childId, int row, QUndoCommand* cmd)
	: model(model), parentId(parentId), childId(childId), row(row), cmd(cmd)
{ }

//------------------------------------------------------------------------------
int ProxyModelDeleteCommand::id() const
{
	return 7633252;
}

//------------------------------------------------------------------------------
bool ProxyModelDeleteCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	return cmd->mergeWith(static_cast<const ProxyModelDeleteCommand*>(command)->cmd);
}

//------------------------------------------------------------------------------
void ProxyModelDeleteCommand::redo()
{
	QModelIndex index = model->index(parentId);
	model->beginRemoveRows(index, row, row);
	cmd->redo();
	model->endRemoveRows();
}

//------------------------------------------------------------------------------
void ProxyModelDeleteCommand::undo()
{
	QModelIndex index = model->index(parentId);
	model->beginInsertRows(index, row, row);
	cmd->undo();
	model->endInsertRows();
}

}


