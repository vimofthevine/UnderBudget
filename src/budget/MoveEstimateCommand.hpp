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

#ifndef MOVEESTIMATECOMMAND_HPP
#define MOVEESTIMATECOMMAND_HPP

// Qt include(s)
#include <QUndoCommand>

// UnderBudget include(s)
#include "budget/Estimate.hpp"

namespace ub {

/**
 * An undoable command to move an estimate.
 *
 * @ingroup estimate_commands
 */
class MoveEstimateCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new move estimate command. It is critical to note that
	 * the estimate map pointer being given is not owned by this object, and it may
	 * be deleted externally at any time. Since the `QUndoStack` that will take
	 * ownership of this command is tied to the `Session` that also holds a
	 * `QSharedPointer` to the encompassing budget, it should be impossible for
	 * the estimate map to be deleted while this command is still relevant.
	 *
	 * @param[in] root        root estimate
	 * @param[in] estimateId  ID of the estimate to be moved
	 * @param[in] newParentId ID of the new parent to which to move the estimate
	 * @param[in] newIndex    new index under the parent to which to move the estimate
	 * @param[in] parent      parent undoable command for grouping
	 */
	MoveEstimateCommand(Estimate* root, uint estimateId,
		uint newParentId, int newIndex,
		QUndoCommand* parent = 0);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Move estimate command ID */
	static const int ID;
	/** Root estimate */
	Estimate* root;
	/** The ID of the estimate to be moved */
	uint estimateId;
	/** The ID of the old parent estimate */
	uint oldParentId;
	/** The ID of the new parent estimate */
	uint newParentId;
	/** The index of the estimate within the old parent's children */
	int oldIndex;
	/** The index of the estimate within the new parent's children */
	int newIndex;
};

}

#endif //MOVEESTIMATECOMMAND_HPP
