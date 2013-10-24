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
#include "ui/budget/BalanceContributorsModel.hpp"
#include "ui/budget/ContributorRemoveProxyCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
ContributorRemoveProxyCommand::ContributorRemoveProxyCommand(
		BalanceContributorsModel* model, int index,
		QUndoCommand* cmd)
	: model(model), index(index), cmd(cmd)
{ }

//------------------------------------------------------------------------------
int ContributorRemoveProxyCommand::id() const
{
	return 7639258;
}

//------------------------------------------------------------------------------
bool ContributorRemoveProxyCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	return cmd->mergeWith(static_cast<const ContributorRemoveProxyCommand*>(command)->cmd);
}

//------------------------------------------------------------------------------
void ContributorRemoveProxyCommand::redo()
{
	model->beginRemoveRows(QModelIndex(), index, index);
	cmd->redo();
	model->endRemoveRows();
}

//------------------------------------------------------------------------------
void ContributorRemoveProxyCommand::undo()
{
	model->beginInsertRows(QModelIndex(), index, index);
	cmd->undo();
	model->endInsertRows();
}

}

