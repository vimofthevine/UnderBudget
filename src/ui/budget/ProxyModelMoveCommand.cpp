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
#include "ui/budget/ProxyModelMoveCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
ProxyModelMoveCommand::ProxyModelMoveCommand(EstimateModel* model,
		uint oldParentId, int oldRow, uint newParentId, int newRow,
		QUndoCommand* cmd)
	: model(model), oldParentId(oldParentId), oldRow(oldRow),
	  newParentId(newParentId), newRow(newRow), cmd(cmd)
{ }

//------------------------------------------------------------------------------
int ProxyModelMoveCommand::id() const
{
	return 7633254;
}

//------------------------------------------------------------------------------
bool ProxyModelMoveCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	return cmd->mergeWith(static_cast<const ProxyModelMoveCommand*>(command)->cmd);
}

//------------------------------------------------------------------------------
void ProxyModelMoveCommand::redo()
{
	QModelIndex oldParentIndex = model->index(oldParentId);
	QModelIndex newParentIndex = model->index(newParentId);

	model->beginMoveRows(oldParentIndex, oldRow, oldRow, newParentIndex, newRow);
	cmd->redo();
	model->endMoveRows();
}

//------------------------------------------------------------------------------
void ProxyModelMoveCommand::undo()
{
	QModelIndex oldParentIndex = model->index(oldParentId);
	QModelIndex newParentIndex = model->index(newParentId);

	model->beginMoveRows(newParentIndex, newRow, newRow, oldParentIndex, oldRow);
	cmd->undo();
	model->endMoveRows();
}

}


