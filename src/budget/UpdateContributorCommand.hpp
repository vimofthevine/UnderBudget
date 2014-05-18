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

#ifndef UPDATECONTRIBUTORCOMMAND_HPP
#define UPDATECONTRIBUTORCOMMAND_HPP

// Qt include(s)
#include <QUndoCommand>

// UnderBudget include(s)
#include "accounting/Money.hpp"
#include "budget/Balance.hpp"

namespace ub {

/**
 * An undoable command to update an existing contributor for a balance.
 *
 * @ingroup balance_commands
 */
class UpdateContributorCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new update-contributor command. It is critical to note that
	 * the balance pointer being given is not owned by this object, and it may
	 * be deleted externally at any time. Since the `QUndoStack` that will take
	 * ownership of this command is tied to the `Session` that also holds a
	 * `QSharedPointer` to the budget, it should be impossible for the balance
	 * to be deleted while this command is still relevant.
	 *
	 * @param[in] balance     balance being modified
	 * @param[in] index       index of the contributor to be updated
	 * @param[in] contributor new contributor parameters
	 * @param[in] parent      parent undoable command for grouping
	 */
	UpdateContributorCommand(Balance* balance, int index,
		const Balance::Contributor& contributor, QUndoCommand* parent = 0);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Update contributor command ID */
	static const int ID;
	/** The balance being modified */
	Balance* balance;
	/** Index of the contributor to be updated */
	int index;
	/** Old contributor parameters */
	Balance::Contributor oldContributor;
	/** New contributor parameters */
	Balance::Contributor newContributor;
};

}

#endif //UPDATECONTRIBUTORCOMMAND_HPP
