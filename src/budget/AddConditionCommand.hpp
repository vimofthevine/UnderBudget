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

#ifndef ADDCONDITIONCOMMAND_HPP
#define ADDCONDITIONCOMMAND_HPP

// Qt include(s)
#include <QUndoCommand>

namespace ub {

// Forward declaration(s)
class AssignmentRules;

/**
 * An undoable command to add a new condition to an assignment rule.
 *
 * @ingroup rule_commands
 */
class AddConditionCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new add condition command. It is critical to note that
	 * the assignment rules list pointer being given is not owned by this
	 * object, and it may be deleted at any time. Since the `QUndoStack` that
	 * will take ownership of this command is tied to the `Session` that also
	 * holds a `QSharedPointer` to the encompassing budget, it should be
	 * impossible for the rules list to be deleted while this command is still
	 * relevant.
	 *
	 * @param[in] rules  assignment rules list
	 * @param[in] ruleId ID of the rule to which to add a condition
	 * @param[in] parent parent undoable command for grouping
	 */
	AddConditionCommand(AssignmentRules* rules, uint ruleId,
		QUndoCommand* parent = 0);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Add condition command ID */
	static const int ID;
	/** Assignment rules list */
	AssignmentRules* rules;
	/** The ID of the assignment rule */
	uint ruleId;
	/** Index at which the new condition will be inserted */
	int index;
};

}

#endif //ADDCONDITIONCOMMAND_HPP
