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
#include "ui/budget/ProxyModelAddCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
ProxyModelAddCommand::ProxyModelAddCommand(EstimateModel* model, uint id,
		QUndoCommand* cmd)
	: model(model), estimateId(id), cmd(cmd)
{
	row = model->rowCount(model->index(estimateId));
}

//------------------------------------------------------------------------------
int ProxyModelAddCommand::id() const
{
	return 7633253;
}

//------------------------------------------------------------------------------
bool ProxyModelAddCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	return cmd->mergeWith(static_cast<const ProxyModelAddCommand*>(command)->cmd);
}

//------------------------------------------------------------------------------
void ProxyModelAddCommand::redo()
{
	QModelIndex index = model->index(estimateId);
	model->beginInsertRows(index, row, row);
	cmd->redo();
	model->endInsertRows();
}

//------------------------------------------------------------------------------
void ProxyModelAddCommand::undo()
{
	QModelIndex index = model->index(estimateId);
	model->beginRemoveRows(index, row, row);
	cmd->undo();
	model->endRemoveRows();
}

}



