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

#ifndef DELETEESTIMATECOMMAND_HPP
#define DELETEESTIMATECOMMAND_HPP

// Qt include(s)
#include <QUndoCommand>

// UnderBudget include(s)
#include "budget/Estimate.hpp"

namespace ub {

/**
 * An undoable command to delete an estimate.
 */
class DeleteEstimateCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new delete estimate command. It is critical to note that
	 * the estimate map pointer being given is not owned by this object, and it may
	 * be deleted externally at any time. Since the `QUndoStack` that will take
	 * ownership of this command is tied to the `Session` that also holds a
	 * `QSharedPointer` to the encompassing budget, it should be impossible for
	 * the estimate map to be deleted while this command is still relevant.
	 *
	 * @param[in] estimates  map of estimates
	 * @param[in] estimateId ID of the estimate to be deleted
	 * @param[in] parent     parent undoable command for grouping
	 */
	DeleteEstimateCommand(EstimatePointerMap estimates, uint estimateId,
		QUndoCommand* parent = 0);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Delete estimate command ID */
	static const int ID;
	/** Map of estimates */
	EstimatePointerMap estimates;
	/** The ID of the estimate to be deleted */
	uint estimateId;
	/** The ID of the parent estimate */
	uint parentId;
	/** Name of the deleted estimate */
	QString name;
	/** Description of the deleted estimate */
	QString description;
	/** Type of the deleted estimate */
	Estimate::Type type;
	/** Amount of the deleted estimate */
	Money amount;
	/** Due date of the deleted estimate */
	QDate dueDate;
	/** Finished state of the deleted estimate */
	bool finished;
};

}

#endif //DELETEESTIMATECOMMAND_HPP
