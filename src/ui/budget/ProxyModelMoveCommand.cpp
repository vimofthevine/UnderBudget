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
	qDebug() << "applying move from" << oldParentId << "at index" << oldRow
		<< "to" << newParentId << "at index" << newRow;
	QModelIndex oldParentIndex = model->index(oldParentId);
	QModelIndex newParentIndex = model->index(newParentId);

	if (model->beginMoveRows(oldParentIndex, oldRow, oldRow, newParentIndex, newRow))
	{
		cmd->redo();
		model->endMoveRows();
		model->emitDataChanged(QModelIndex());
	}
	else
	{
		qDebug() << "move rows rejected";
	}
}

//------------------------------------------------------------------------------
void ProxyModelMoveCommand::undo()
{
	qDebug() << "undoing move from" << oldParentId << "at index" << oldRow
		<< "to" << newParentId << "at index" << newRow;
	QModelIndex oldParentIndex = model->index(oldParentId);
	QModelIndex newParentIndex = model->index(newParentId);
	qDebug() << "old parent index valid?" << oldParentIndex.isValid();
	qDebug() << "new parent index valid?" << newParentIndex.isValid();
	int originRow = newRow;

	// If the original move was a re-ordering under the same parent
	if (oldParentId == newParentId)
	{
		// If it was a move to the end of the parent's children
		int numChildren = model->rowCount(newParentIndex);
		if (numChildren == newRow)
		{
			qDebug() << "undoing reorder to end of list";
			originRow = numChildren - 1;
		}
	}

	if (model->beginMoveRows(newParentIndex, originRow, originRow, oldParentIndex, oldRow))
	{
		cmd->undo();
		model->endMoveRows();
		model->emitDataChanged(QModelIndex());
	}
	else
	{
		qDebug() << "move rows rejected";
	}
}

}


