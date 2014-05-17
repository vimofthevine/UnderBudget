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

#ifndef ASSIGNMENTRULES_HPP
#define ASSIGNMENTRULES_HPP

// Qt include(s)
#include <QHash>
#include <QMultiHash>
#include <QObject>

// UnderBudget include(s)
#include "budget/AssignmentRule.hpp"

namespace ub {

/**
 * A user-defined list of assignment rules.
 *
 * @ingroup rule
 */
class AssignmentRules : public QObject
{
	Q_OBJECT

public:
	/**
	 * Destroys all rules in this list.
	 */
	~AssignmentRules();

	/**
	 * Creates a new assignment rules list. The new list is allocated on the
	 * heap, and the memory is owned by the returned shared pointer. When all
	 * references to the list have gone out of scope, the `QSharedPointer`
	 * will delete the list, and all rules along with it.
	 *
	 * @return shared pointer to the assignment rules list
	 */
	static QSharedPointer<AssignmentRules> create();

	/**
	 * Adds a new rule to the end of this list. This should only be used
	 * when constructing the rules list from a persistent storage medium.
	 * Once constructed, new rules should be added to the list using
	 * `addRule()`.
	 *
	 * @param[in] ruleId     unique ID
	 * @param[in] estimateId associated estimate ID
	 * @param[in] conditions rule conditions
	 */
	AssignmentRule* createRule(uint ruleId, uint estimateId,
		const QList<AssignmentRule::Condition>& conditions);

	// -- Property access methods

	/**
	 * Returns the current size of the rules list.
	 *
	 * @return size of the rules list
	 */
	int size() const;

	/**
	 * Returns the index at which the specified rule is located in this
	 * rules list. If the rule does not exist in the list, -1 is returned.
	 *
	 * @param[in] ruleID ID of the rule whose index is to be returned
	 * @return index at which the rule exists, or -1 if it does not exist
	 */
	int indexOf(uint ruleId) const;

	/**
	 * Returns the assignment rule at the given index. Ownership of the
	 * returned pointer remains with this rules list.
	 *
	 * @return assignment rule at the given index.
	 */
	AssignmentRule* at(int index) const;

	/**
	 * Searches the rules list for a rule with the given ID. Ownership
	 * of the returned pointer remains with this rules list.
	 *
	 * @param[in] ruleId ID of the rule to be searched
	 * @return assignment rule, if found, else a null pointer
	 */
	AssignmentRule* find(uint ruleId) const;

	/**
	 * Searches the rules list for rules associated with the given
	 * estimate ID. Ownership of the returned pointer remains with
	 * this rules list.
	 *
	 * @param[in] estimateId ID of the estimate to be searched
	 * @return list of assignment rules associated with the given
	 *         estimate ID, or an empty list if none exist
	 */
	QList<AssignmentRule*> findFor(uint estimateId) const;

	// -- Modification methods (via command pattern)

	/**
	 * Creates an undoable command to add a new rule to the end of
	 * this rules list.
	 *
	 * When the rule is added as a result of executing the created command,
	 * the `ruleAdded()` signal is emitted. When the rule is removed as a result
	 * of undoing the created command, the `ruleRemoved()` signal is emitted.
	 *
	 * @param[in] estimateId ID of the estimate to be associated with the rule
	 * @param[in] cmd        parent command to be used for grouping
	 */
	QUndoCommand* addRule(uint estimateId, QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to clone an existing rule in the rules list.
	 *
	 * When the clone is created as a result of executing the created command,
	 * the `ruleAdded()` signal is emitted. When the rule is removed as a result
	 * of undoing the created command, the `ruleRemoved()` signal is emitted.
	 *
	 * @param[in] ruleId ID of the rule to be cloned
	 * @param[in] cmd    parent command to be used for grouping
	 */
	QUndoCommand* cloneRule(uint ruleId, QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to remove all rules associated with the
	 * given estimate.
	 *
	 * When the rules are removed as a result of executing the created command,
	 * a `ruleRemoved()` signal is emitted for each rule. When the rules are
	 * added as a result of undoing the created command, a `ruleAdded()` signal
	 * is emitted for each rule.
	 *
	 * @param[in] estimateID ID of the estimate whose associated rules are to
	 *                       be deleted
	 * @param[in] cmd        parent command to be used for grouping
	 */
	QUndoCommand* removeAll(uint estimateId, QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to remove the specified rule from the list.
	 *
	 * When the rule is removed as a result of executing the created command,
	 * the `ruleRemoved()` signal is emitted. When the rule is re-added as a
	 * result of undoing the created command, the `ruleAdded()` signal is emitted.
	 *
	 * @param[in] ruleId ID of the rule to be removed
	 * @param[in] cmd    parent command to be used for grouping
	 */
	QUndoCommand* removeRule(uint ruleId, QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to remove the rule at the specified index.
	 *
	 * When the rule is removed as a result of executing the created command,
	 * the `ruleRemoved()` signal is emitted. When the rule is re-added as a
	 * result of undoing the created command, the `ruleAdded()` signal is emitted.
	 *
	 * @param[in] index index of the rule to be removed
	 * @param[in] cmd   parent command to be used for grouping
	 */
	QUndoCommand* removeAt(int index, QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to move the rule at the specified origin
	 * index to the given destination index.
	 *
	 * When the rule is moved as a result of executing or undoing the created
	 * command, the `ruleMoved()` signal is emitted.
	 *
	 * @param[in] from origin index from which to move a rule
	 * @param[in] to   destination index to which to move a rule
	 * @param[in] cmd  parent command to be used for grouping
	 */
	QUndoCommand* move(int from, int to, QUndoCommand* cmd = 0);

signals:
	/**
	 * Emitted when a new rule is added to the rules list.
	 *
	 * @param rule  new rule
	 * @param index index of the new rule
	 */
	void ruleAdded(AssignmentRule* rule, int index);

	/**
	 * Emitted when a rule has been removed from the rules list.
	 *
	 * @param rule  rule that was removed
	 * @param index old index of the rule
	 */
	void ruleRemoved(AssignmentRule* rule, int index);

	/**
	 * Emitted when a rule has been moved within the rules list.
	 *
	 * @param rule rule that was moved
	 * @param from old index of the rule
	 * @param to   new index of the rule
	 */
	void ruleMoved(AssignmentRule* rule, int from, int to);

private:
	/** List of rules */
	QList<AssignmentRule*> rules;
	/** Map of rule IDs to indices (for faster lookup) */
	QHash<uint, int> ridToIndex;
	/** Map of estimate IDs to indices (for estimate-based lookup) */
	QMultiHash<uint, int> eidToIndex;

	/**
	 * Constructs a new assignment rules list. This constructor is
	 * private so that client code cannot create an assignment
	 * rule list on the stack.
	 */
	AssignmentRules();

	/**
	 * Re-indexes the entire rules list. This operation could take a substantial
	 * amount of time if run too often.
	 */
	void reindex();

	/**
	 * Creates a new rule at the end of this rules list, with the given
	 * parameters. The `ruleAdded()` signal is emitted.
	 *
	 * @param[in] index      index of the rule at which to insert the rule
	 * @param[in] ruleId     ID of the rule to be created
	 * @param[in] estimateId ID of the associated estimate
	 * @param[in] conditions rule conditions
	 * @return newly created rule
	 */
	AssignmentRule* insert(int index, uint ruleId, uint estimateId,
		const QList<AssignmentRule::Condition>& conditions);

	/**
	 * Removes the rule at the specified index from the rules list. The
	 * `ruleRemoved()` signal is emitted.
	 *
	 * @param[in] index index of the rule to be deleted
	 */
	void remove(int index);

	/**
	 * Moves the rule from the specified origin index to the destination index.
	 * The `ruleMoved()` signal is emitted.
	 *
	 * @param[in] from origin index of the rule to be moved
	 * @param[in] to   destination index of the rule to be moved
	 */
	void moveRule(int from, int to);

	// Allow undoable commands private access
	friend class InsertRuleCommand;
	friend class MoveRuleCommand;
	friend class RemoveRuleCommand;
};

}

#endif //ASSIGNMENTRULES_HPP
