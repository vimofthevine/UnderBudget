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

#ifndef PROXYMODELDELETECOMMAND_HPP
#define PROXYMODELDELETECOMMAND_HPP

// Qt include(s)
#include <QPersistentModelIndex>
#include <QUndoCommand>

// Forward declaration(s)
class QModelIndex;

namespace ub {

// Forward declaration(s)
class EstimateModel;

/**
 * Undoable command proxy for a delete command on the underlying model,
 * that will trigger the correct signals on the original view model.
 */
class ProxyModelDeleteCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new model delete proxy command.
	 *
	 * @param[in] model estimate tree model
	 * @param[in] index index of the estimate to be deleted
	 * @param[in] cmd   original delete command
	 */
	ProxyModelDeleteCommand(EstimateModel* model, const QModelIndex& index,
		QUndoCommand* cmd);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Estimate tree model */
	EstimateModel* model;
	/** Index of the parent of the estimate to be deleted */
	QPersistentModelIndex parent;
	/** Row of the estimate to be deleted under the parent */
	int row;
	/** Original command */
	QUndoCommand* cmd;
};

}

#endif //PROXYMODELDELETECOMMAND_HPP

