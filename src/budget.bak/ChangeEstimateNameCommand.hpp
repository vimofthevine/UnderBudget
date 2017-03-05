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

#ifndef CHANGEESTIMATENAMECOMMAND_HPP
#define CHANGEESTIMATENAMECOMMAND_HPP

// Qt include(s)
#include <QUndoCommand>

// UnderBudget include(s)
#include "budget/Estimate.hpp"

namespace ub {

/**
 * An undoable command to change a estimate's name.
 *
 * @ingroup estimate_commands
 */
class ChangeEstimateNameCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new change estimate name command. It is critical to note that
	 * the estimate map pointer being given is not owned by this object, and it may
	 * be deleted externally at any time. Since the `QUndoStack` that will take
	 * ownership of this command is tied to the `Session` that also holds a
	 * `QSharedPointer` to the encompassing budget, it should be impossible for
	 * the estimate map to be deleted while this command is still relevant.
	 *
	 * @param[in] root       root estimate
	 * @param[in] estimateId ID of the estimate being modified
	 * @param[in] oldName    old name
	 * @param[in] newName    new name
	 * @param[in] parent     parent undoable command for grouping
	 */
	ChangeEstimateNameCommand(Estimate* root, uint estimateId,
		const QString& oldName, const QString& newName,
		QUndoCommand* parent = 0);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Change estimate name command ID */
	static const int ID;
	/** Root estimate */
	Estimate* root;
	/** The ID of the estimate being modified */
	uint estimateId;
	/** The previous name */
	QString oldName;
	/** The new name */
	QString newName;
};

}

#endif //CHANGEESTIMATENAMECOMMAND_HPP
