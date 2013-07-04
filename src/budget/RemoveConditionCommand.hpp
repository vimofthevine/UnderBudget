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

#ifndef REMOVECONDITIONCOMMAND_HPP
#define REMOVECONDITIONCOMMAND_HPP

// Qt include(s)
#include <QUndoCommand>

namespace ub {

// Forward declaration(s)
class AssignmentRules;

/**
 * An undoable command to remove an existing condition from an assignment rule.
 */
class RemoveConditionCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new remove condition command. It is critical to note that
	 * the assignment rules list pointer being given is not owned by this
	 * object, and it may be deleted at any time. Since the `QUndoStack` that
	 * will take ownership of this command is tied to the `Session` that also
	 * holds a `QSharedPointer` to the encompassing budget, it should be
	 * impossible for the rules list to be deleted while this command is still
	 * relevant.
	 *
	 * @param[in] rules  assignment rules list
	 * @param[in] ruleId ID of the rule from which to delete the condition
	 * @param[in] index  index of the condition to be removed
	 * @param[in] parent parent undoable command for grouping
	 */
	RemoveConditionCommand(AssignmentRules* rules, uint ruleId, int index,
		QUndoCommand* parent = 0);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Remove condition command ID */
	static const int ID;
	/** Assignment rules list */
	AssignmentRules* rules;
	/** The ID of the assignment rule */
	uint ruleId;
	/** Index of the condition to be removed */
	int index;
	/** Original condition parameters */
	AssignmentRule::Condition condition;
};

}

#endif //REMOVECONDITIONCOMMAND_HPP
