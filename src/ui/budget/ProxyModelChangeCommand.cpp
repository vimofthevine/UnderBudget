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
#include "ui/budget/ProxyModelChangeCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
ProxyModelChangeCommand::ProxyModelChangeCommand(EstimateModel* model,
		const QModelIndex& index, QUndoCommand* cmd)
	: model(model), index(index), cmd(cmd)
{ }

//------------------------------------------------------------------------------
int ProxyModelChangeCommand::id() const
{
	return 7633251;
}

//------------------------------------------------------------------------------
bool ProxyModelChangeCommand::mergeWith(const QUndoCommand* command)
{
	if (command->id() != id())
		return false;

	return cmd->mergeWith(static_cast<const ProxyModelChangeCommand*>(command)->cmd);
}

//------------------------------------------------------------------------------
void ProxyModelChangeCommand::redo()
{
	cmd->redo();
	model->emitDataChanged(index);
}

//------------------------------------------------------------------------------
void ProxyModelChangeCommand::undo()
{
	cmd->undo();
	model->emitDataChanged(index);
}

}

