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

#ifndef CONTRIBUTORADDPROXYCOMMAND_HPP
#define CONTRIBUTORADDPROXYCOMMAND_HPP

// Qt include(s)
#include <QUndoCommand>

namespace ub {

// Forward declaration(s)
class BalanceContributorsModel;

/**
 * Undoable command proxy for a add-contributor command on the underlying model,
 * that will trigger the correct signals on the original view model.
 */
class ContributorAddProxyCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new model add-contributor proxy command.
	 *
	 * @param[in] model balance contributors list model
	 * @param[in] index index at which the new contributor will be inserted
	 * @param[in] cmd   original add-child command
	 */
	ContributorAddProxyCommand(BalanceContributorsModel* model, int index, QUndoCommand* cmd);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Balance contributors list model */
	BalanceContributorsModel* model;
	/** Index at which the contributor is being inserted */
	const int index;
	/** Original command */
	QUndoCommand* cmd;
};

}

#endif //CONTRIBUTORADDPROXYCOMMAND_HPP
