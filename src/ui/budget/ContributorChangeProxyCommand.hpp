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

#ifndef CONTRIBUTORCHANGEPROXYCOMMAND_HPP
#define CONTRIBUTORCHANGEPROXYCOMMAND_HPP

// Qt include(s)
#include <QPersistentModelIndex>
#include <QUndoCommand>

// Forward declaration(s)
class QModelIndex;

namespace ub {

// Forward declaration(s)
class BalanceContributorsModel;

/**
 * Undoable command proxy for a change command on the underlying model,
 * that will trigger the correct signals on the original view model.
 */
class ContributorChangeProxyCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new model change proxy command.
	 *
	 * @param[in] model balance contributors list model
	 * @param[in] index row index of the modified contributor
	 * @param[in] cmd   original change command
	 */
	ContributorChangeProxyCommand(BalanceContributorsModel* model,
		int index, QUndoCommand* cmd);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Balance contributors list model */
	BalanceContributorsModel* model;
	/** Row index of the modified contributor */
	const int index;
	/** Original command */
	QUndoCommand* cmd;
};

}

#endif //CONTRIBUTORCHANGEPROXYCOMMAND_HPP
