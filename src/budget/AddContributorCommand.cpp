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
#include "budget/AddContributorCommand.hpp"
#include "budget/Balance.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int AddContributorCommand::ID = 8723462;

//------------------------------------------------------------------------------
AddContributorCommand::AddContributorCommand(Balance* balance,
		QUndoCommand* parent)
	: QUndoCommand(parent), balance(balance), index(balance->contributorCount())
{ }

//------------------------------------------------------------------------------
int AddContributorCommand::id() const
{
	return ID;
}

//------------------------------------------------------------------------------
bool AddContributorCommand::mergeWith(const QUndoCommand* command)
{
	// This command can never be merged
	return false;
}

//------------------------------------------------------------------------------
void AddContributorCommand::redo()
{
	balance->addContributor(Balance::Contributor(), index);
}

//------------------------------------------------------------------------------
void AddContributorCommand::undo()
{
	balance->deleteContributor(index);
}

}

