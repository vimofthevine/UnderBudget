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
#include <QtCore>

// UnderBudget include(s)
#include "budget/Balance.hpp"
#include "budget/RemoveContributorCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int RemoveContributorCommand::ID = 8723463;

//------------------------------------------------------------------------------
RemoveContributorCommand::RemoveContributorCommand(Balance* balance, int index,
		QUndoCommand* parent)
	: QUndoCommand(parent), balance(balance), index(index)
{
	contributor = balance->contributorAt(index);
}

//------------------------------------------------------------------------------
int RemoveContributorCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool RemoveContributorCommand::mergeWith(const QUndoCommand* command)
{
	// This command can never be merged
	return false;
}

//------------------------------------------------------------------------------
void RemoveContributorCommand::redo()
{
	balance->deleteContributor(index);
}

//------------------------------------------------------------------------------
void RemoveContributorCommand::undo()
{
	balance->addContributor(contributor, index);
}

}

