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

#ifndef MOVERULECOMMAND_HPP
#define MOVERULECOMMAND_HPP

// Qt include(s)
#include <QList>
#include <QUndoCommand>

// UnderBudget include(s)
#include "budget/AssignmentRule.hpp"

// Forward delcaration(s)
class AssignmentRules;

namespace ub {

/**
 * An undoable command to move an existing rule within the rules list.
 *
 * @ingroup rule_commands
 */
class MoveRuleCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new move rule command. It is critical to note that the
	 * rules list pointer being given is not owned by this object, and it
	 * may be deleted externally at any time. Since the `QUndoStack` that
	 * will take ownership of this command is tied to the `Session` that
	 * also holds a `QSharedPointer` to the encompassing budget, it should
	 * be impossible for the rules list to be deleted while this command
	 * is still relevant.
	 *
	 * @param[in] rules  rules list
	 * @param[in] from   original index from which to move the rule
	 * @param[in] to     destination index to which to move the rule
	 * @param[in] parent parent undoable command for grouping
	 */
	MoveRuleCommand(AssignmentRules* rules, int from, int to,
		QUndoCommand* parent);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Move rule command ID */
	static const int ID;
	/** Rules list */
	AssignmentRules* rules;
	/** Original index of the rule to be moved */
	const int oldIndex;
	/** New index of the rule to be moved */
	const int newIndex;
};

}

#endif //MOVERULECOMMAND_HPP
