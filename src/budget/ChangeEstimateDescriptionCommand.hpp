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

#ifndef CHANGEESTIMATEDESCRIPTIONCOMMAND_HPP
#define CHANGEESTIMATEDESCRIPTIONCOMMAND_HPP

// Qt include(s)
#include <QUndoCommand>

// UnderBudget include(s)
#include "budget/Estimate.hpp"

namespace ub {

/**
 * An undoable command to change a estimate's description.
 */
class ChangeEstimateDescriptionCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new change estimate description command. It is critical to note that
	 * the estimate map pointer being given is not owned by this object, and it may
	 * be deleted externally at any time. Since the `QUndoStack` that will take
	 * ownership of this command is tied to the `Session` that also holds a
	 * `QSharedPointer` to the encompassing budget, it should be impossible for
	 * the estimate map to be deleted while this command is still relevant.
	 *
	 * @param[in] estimates  map of estimates
	 * @param[in] estimateId ID of the estimate being modified
	 * @param[in] oldDescrip old description
	 * @param[in] newDescrip new description
	 * @param[in] parent     parent undoable command for grouping
	 */
	ChangeEstimateDescriptionCommand(EstimatePointerMap estimates, uint estimateId,
		const QString& oldDescrip, const QString& newDescrip,
		QUndoCommand* parent = 0);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Change estimate description command ID */
	static const int ID;
	/** Map of estimates */
	EstimatePointerMap estimates;
	/** The ID of the estimate being modified */
	uint estimateId;
	/** The previous description */
	QString oldDescrip;
	/** The new description */
	QString newDescrip;
};

}

#endif //CHANGEESTIMATEDESCRIPTIONCOMMAND_HPP
