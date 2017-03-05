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

#ifndef CHANGEPERIODPARAMSCOMMAND_HPP
#define CHANGEPERIODPARAMSCOMMAND_HPP

// Qt include(s)
#include <QUndoCommand>

// UnderBudget include(s)
#include "budget/BudgetingPeriod.hpp"

namespace ub {

/**
 * And undoable command to change a budgeting period's definition.
 *
 * @ingroup period_commands
 */
class ChangePeriodParamsCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new change period parameters command. It is critical to note that
	 * the budgeting period pointer being given is not owned by this object, and it may
	 * be deleted externally at any time. Since the `QUndoStack` that will take
	 * ownership of this command is tied to the `Session` that also holds a
	 * `QSharedPointer` to the encompassing budget, it should be impossible for the budget
	 * to be deleted while this command is still relevant.
	 *
	 * @param[in] period    budgeting period being modified
	 * @param[in] oldParams old period definition parameters
	 * @param[in] newParams new period definition parameters
	 * @param[in] parent    parent undoable command for grouping
	 */
	ChangePeriodParamsCommand(BudgetingPeriod* period,
		const BudgetingPeriod::Parameters& oldParams,
		const BudgetingPeriod::Parameters& newParams,
		QUndoCommand* parent = 0);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Change period parameters command ID */
	static const int ID;
	/** The budgeting period being modified */
	BudgetingPeriod* period;
	/** The previous parameters */
	BudgetingPeriod::Parameters oldParams;
	/** The new parameters */
	BudgetingPeriod::Parameters newParams;
};

}

#endif //CHANGEPERIODPARAMSCOMMAND_HPP
