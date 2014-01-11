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

#ifndef RULESREMOVEPROXYCOMMAND_HPP
#define RULESREMOVEPROXYCOMMAND_HPP

// Qt include(s)
#include <QUndoCommand>

namespace ub {

// Forward declaration(s)
class AssignmentRulesModel;

/**
 * Undoable command proxy for a remove-rules command on the underlying model,
 * that will trigger the correct signals on the original view model.
 */
class RulesRemoveProxyCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new model remove-rules proxy command.
	 *
	 * @param[in] model assignment rules list model
	 * @param[in] cmd   original remove-rules command
	 */
	RulesRemoveProxyCommand(AssignmentRulesModel* model, QUndoCommand* cmd);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Assignment rules list model */
	AssignmentRulesModel* model;
	/** Original command */
	QUndoCommand* cmd;
};

}

#endif //RULESREMOVEPROXYCOMMAND_HPP
