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

#ifndef REMOVERULECOMMAND_HPP
#define REMOVERULECOMMAND_HPP

// Qt include(s)
#include <QList>
#include <QUndoCommand>

// UnderBudget include(s)
#include "budget/AssignmentRule.hpp"

// Forward delcaration(s)
class AssignmentRules;

namespace ub {

/**
 * An undoable command to remove an existing rule from the rules list.
 *
 * @ingroup rule_commands
 */
class RemoveRuleCommand : public QUndoCommand
{
public:
	/**
	 * Constructs a new remove rule command. It is critical to note that the
	 * rules list pointer being given is not owned by this object, and it
	 * may be deleted externally at any time. Since the `QUndoStack` that
	 * will take ownership of this command is tied to the `Session` that
	 * also holds a `QSharedPointer` to the encompassing budget, it should
	 * be impossible for the rules list to be deleted while this command
	 * is still relevant.
	 *
	 * @param[in] rules      rules list
	 * @param[in] index      index from which to remove the rule
	 * @param[in] ruleId     ID of the original rule to be removed
	 * @param[in] estimateId ID of the associated estimate for the rule
	 * @param[in] conditions list of rule conditions
	 * @param[in] parent     parent undoable command for grouping
	 */
	RemoveRuleCommand(AssignmentRules* rules, int index,
		uint ruleId, uint estimateId,
		const QList<AssignmentRule::Condition>& conditions, QUndoCommand* parent);

	// Overriding methods

	int id() const;
	bool mergeWith(const QUndoCommand* command);
	void redo();
	void undo();

private:
	/** Remove rule command ID */
	static const int ID;
	/** Rules list */
	AssignmentRules* rules;
	/** Index from which to remove the rule */
	const int index;
	/** Original ID of the rule to be removed */
	const uint ruleId;
	/** ID of the associated estimate */
	const uint estimateId;
	/** Rule conditions */
	const QList<AssignmentRule::Condition> conditions;
};

}

#endif //REMOVERULECOMMAND_HPP
