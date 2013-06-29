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

#ifndef PROXYMODELMOVECOMMAND_HPP
#define PROXYMODELMOVECOMMAND_HPP

// Qt include(s)
#include <QPersistentModelIndex>
#include <QUndoCommand>

// Forward declaration(s)
class QModelIndex;

namespace ub {

// Forward declaration(s)
class EstimateModel;

/**
 * Undoable command proxy for a move command on the underlying model,
 * that will trigger the correct signals on the original view model.
 */
class ProxyModelMoveCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new model move proxy command.
	 *
	 * @param[in] model       estimate tree model
	 * @param[in] oldParentId ID of the old parent
	 * @param[in] oldRow      old row of the moved estimate
	 * @param[in] newParentId ID of the new parent
	 * @param[in] newRow      new row for the moved estimate
	 * @param[in] cmd         original move command
	 */
	ProxyModelMoveCommand(EstimateModel* model, uint oldParentId,
		int oldRow, uint newParentId, int newRow, QUndoCommand* cmd);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Estimate tree model */
	EstimateModel* model;
	/** ID of the old parent of the moved estimate */
	uint oldParentId;
	/** Old row of the moved estimate */
	int oldRow;
	/** ID of the new parent of the moved estimate */
	uint newParentId;
	/** New row of the moved estimate */
	int newRow;
	/** Original command */
	QUndoCommand* cmd;
};

}

#endif //PROXYMODELMOVECOMMAND_HPP

