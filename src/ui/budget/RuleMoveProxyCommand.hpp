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

#ifndef RULEMOVEPROXYCOMMAND_HPP
#define RULEMOVEPROXYCOMMAND_HPP

// Qt include(s)
#include <QUndoCommand>

namespace ub {

// Forward declaration(s)
class AssignmentRulesModel;

/**
 * Undoable command proxy for a move-rule command on the underlying model,
 * that will trigger the correct signals on the original view model.
 *
 * @ingroup ui_budget
 */
class RuleMoveProxyCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new model move-rule proxy command.
	 *
	 * @param[in] model assignment rules list model
	 * @param[in] from  origin index from which the rule is to be moved
	 * @param[in] to    destination index to which the rule is to be moved
	 * @param[in] cmd   original add-child command
	 */
	RuleMoveProxyCommand(AssignmentRulesModel* model, int from,
		int to, QUndoCommand* cmd);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Assignment rules list model */
	AssignmentRulesModel* model;
	/** Origin index */
	int oldIndex;
	/** Destination index */
	int newIndex;
	/** Original command */
	QUndoCommand* cmd;
};

}

#endif //RULEMOVEPROXYCOMMAND_HPP
