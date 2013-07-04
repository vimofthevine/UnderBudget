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

#ifndef CHANGEINITIALBALANCECOMMAND_HPP
#define CHANGEINITIALBALANCECOMMAND_HPP

// Qt include(s)
#include <QUndoCommand>

// UnderBudget include(s)
#include "accounting/Money.hpp"
#include "budget/Budget.hpp"

namespace ub {

/**
 * An undoable command to change a budget's initial balance.
 */
class ChangeInitialBalanceCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new change initial balance command. It is critical to note that
	 * the budget pointer being given is not owned by this object, and it may
	 * be deleted externally at any time. Since the `QUndoStack` that will take
	 * ownership of this command is tied to the `Session` that also holds a
	 * `QSharedPointer` to the budget, it should be impossible for the budget
	 * to be deleted while this command is still relevant.
	 *
	 * @param[in] budget     budget being modified
	 * @param[in] oldBalance old initial balance
	 * @param[in] newBalance new initial balance
	 * @param[in] parent     parent undoable command for grouping
	 */
	ChangeInitialBalanceCommand(Budget* budget, const Money& oldBalance,
		const Money& newBalance, QUndoCommand* parent = 0);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Change budget initial balance command ID */
	static const int ID;
	/** The budget being modified */
	Budget* budget;
	/** The previous initial balance */
	Money oldBalance;
	/** The new initial balance */
	Money newBalance;
};

}

#endif //CHANGEINITIALBALANCECOMMAND_HPP
