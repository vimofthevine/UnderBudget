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
#include "ui/budget/RuleMoveProxyCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
RuleMoveProxyCommand::RuleMoveProxyCommand(AssignmentRulesModel* model, int from,
		int to, QUndoCommand* cmd)
	: model(model), oldIndex(from), newIndex(to), cmd(cmd)
{ }

//------------------------------------------------------------------------------
int RuleMoveProxyCommand::id() const
{
	return 7638758;
}

//------------------------------------------------------------------------------
bool RuleMoveProxyCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	return cmd->mergeWith(static_cast<const RuleMoveProxyCommand*>(command)->cmd);
}

//------------------------------------------------------------------------------
void RuleMoveProxyCommand::redo()
{
	int from = oldIndex;
	int to = newIndex;

	// If a downward move by one row, make sure it's not a noop
	if ((from+1) == to)
	{
		to += 1;
	}

	if (model->beginMoveRows(QModelIndex(), from, from, QModelIndex(), to))
	{
		cmd->redo();
		model->endMoveRows();
	}
	else
		qDebug() << "rule move rejected";
}

//------------------------------------------------------------------------------
void RuleMoveProxyCommand::undo()
{
	int from = newIndex;
	int to = oldIndex;

	// If originally a move to the end of the list, adjust the index appropriately
	if (from >= model->rowCount())
	{
		from = model->rowCount() - 1;
	}

	// If a downward move by one row, make sure it's not a noop
	if ((from+1) == to)
	{
		to += 1;
	}

	if (model->beginMoveRows(QModelIndex(), from, from, QModelIndex(), to))
	{
		cmd->undo();
		model->endMoveRows();
	}
	else
		qDebug() << "rule un-move rejected";
}

}

