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

#ifndef ASSIGNMENTRULE_HPP
#define ASSIGNMENTRULE_HPP

// Qt include(s)
#include <QObject>
#include <QDebug>

// Forward declaration(s)
class QUndoCommand;

namespace ub {

// Forward declaration(s)
class AssignmentRules;

/**
 * Set of conditions necessary to match a transaction against an
 * individual estimate.
 *
 * Each rule may be comprised of one or more conditions, all of which
 * must be satisfied for a transaction to qualify for assignment to the
 * rule's associated estimate.
 *
 * @ingroup rule
 */
class AssignmentRule : public QObject
{
	Q_OBJECT

public:
	/**
	 * Transaction field enumeration.
	 */
	enum Field
	{
		/** Not defined */
		FieldNotDefined,
		/** Date on which the transaction was posted */
		Date,
		/** Amount of transferred funds */
		Amount,
		/** Payee to which funds were given */
		Payee,
		/** Description of the transaction */
		Memo,
		/** Account into which funds were transferred */
		DepositAccount,
		/** Account from which funds were transferred */
		WithdrawalAccount,
	};

	/**
	 * Comparison operator enumeration.
	 */
	enum Operator
	{
		/**  Not defined */
		OperatorNotDefined,
		/** String begins-with */
		BeginsWith,
		/** String ends-with */
		EndsWith,
		/** String equals */
		StringEquals,
		/** String contains */
		Contains,
		/** Date before */
		Before,
		/** Date after */
		After,
		/** Date on/exact */
		DateEquals,
		/** Amount less-than */
		LessThan,
		/** Amount less-than-or-equal */
		LessThanOrEqual,
		/** Amount greater-than */
		GreaterThan,
		/** Amount greater-than-or-equal */
		GreaterThanOrEqual,
		/** Amount equals */
		AmountEquals,
	};

	/**
	 * Assignment rule condition.
	 */
	struct Condition
	{
		/** Transaction field */
		Field field;
		/** Comparison operator */
		Operator op;
		/** Case-sensitive */
		bool sensitive;
		/** Comparison value */
		QString value;

		/** Default constructor */
		Condition()
			: field(FieldNotDefined), op(OperatorNotDefined), sensitive(false)
		{ }

		/** Parameterized constructor */
		Condition(Field field, Operator oper, bool sensitive, QString value)
			: field(field), op(oper), sensitive(sensitive), value(value)
		{ }
	};

	// -- Modification methods (via command pattern)

	/**
	 * Creates an undoable command to add a new condition
	 * to this assignment rule.
	 *
	 * When the condition is added or removed as a result of
	 * executing or undoing the created command, the `conditionAdded()`
	 * or `conditionRemoved()` signals are emitted.
	 *
	 * @param[in] cmd parent command to be used for grouping
	 */
	QUndoCommand* addCondition(QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to remove an existing condition
	 * from this assignment rule.
	 *
	 * When the condition is removed or added as a result of
	 * executing or undoing the created command, the `conditionRemoved()`
	 * or `conditionAdded()` signals are emitted.
	 *
	 * @param[in] index index of the condition to be removed
	 * @param[in] cmd   parent command to be used for grouping
	 */
	QUndoCommand* removeCondition(int index, QUndoCommand* cmd = 0);

	/**
	 * Creates an undoable command to update an existing condition
	 * for this assignment rule.
	 *
	 * When the condition is updated as a result of executing or
	 * undoing the created command, the `conditionUpdated()` signal
	 * is emitted.
	 *
	 * @param[in] index     index of the condition to be updated
	 * @param[in] condition new condition parameters
	 * @param[in] cmd       parent command to be used for grouping
	 */
	QUndoCommand* updateCondition(int index, const Condition& condition,
		QUndoCommand* cmd = 0);

	// -- Property access methods

	/**
	 * Returns the unique ID of this assignment rule.
	 *
	 * @return unique ID of this assignment rule.
	 */
	uint ruleId() const;

	/**
	 * Returns the ID of the estimate associated with this rule.
	 *
	 * @return ID of the associated estimate
	 */
	uint estimateId() const;

	/**
	 * Returns the number of conditions for this assignment rule.
	 *
	 * @return number of conditions for this assignment rule
	 */
	int conditionCount() const;

	/**
	 * Returns the condition for this assignment rule at the given
	 * index.
	 *
	 * @return condition at the specified index
	 */
	Condition conditionAt(int index) const;

signals:
	/**
	 * Emitted when a new condition is added to this assignment rule as
	 * a result of the commands created by either `addCondition()` or
	 * `removeCondition()`.
	 *
	 * @param condition new condition that was added
	 * @param index     index of the new condition
	 */
	void conditionAdded(const Condition& condition, int index);

	/**
	 * Emitted when an existing condition is removed from this assignment
	 * rule as a result of the commands created by either `addCondition()`
	 * or `removeCondition()`.
	 *
	 * @param condition condition that was removed
	 * @param index     old index of the condition
	 */
	void conditionRemoved(const Condition& condition, int index);

	/**
	 * Emitted when an existing condition is updated as a result of the
	 * command created by `updateCondition()`.
	 *
	 * @param condition new condition parameters
	 * @param index     index of the updated condition
	 */
	void conditionUpdated(const Condition& condition, int index);

private:
	/** Unique ID */
	const uint id;
	/** Associated estimate ID */
	const uint eid;
	/** Conditions */
	QList<Condition> conditions;
	/**
	 * Assignment rules list. This is stored as a pointer so that
	 * the rules list will be properly deleted once all references to
	 * it are out of scope. The rules list will delete all rules
	 * belonging to the list when it is deleted itself.
	 */
	AssignmentRules* rules;

	/**
	 * Creates a new rule with the given conditions.
	 *
	 * @param[in] ruleId     unique ID
	 * @param[in] estimateId associated estimate ID
	 * @param[in] conditions rule conditions
	 * @param[in] rules      assignment rules list
	 */
	AssignmentRule(uint ruleId, uint estimateId, const QList<Condition>& conditions,
		AssignmentRules* rules);

	/**
	 * Adds the given condition to this assignment rule at the specified index.
	 * The `conditionAdded()` signal is emitted.
	 *
	 * @param[in] condition new condition
	 * @param[in] index     index at which to insert the new condition
	 */
	void addCondition(const Condition& condition, int index);

	/**
	 * Removes the condition located at the specified index. The `conditionRemoved()`
	 * signal is emitted.
	 *
	 * @param[in] index index of the condition to be removed
	 */
	void deleteCondition(int index);

	/**
	 * Updates the condition located at the specified idnex. The
	 * `conditionUpdated()` signal is emitted.
	 *
	 * @param[in] condition new condition parameters
	 * @param[in] index     index of the condition to be updated
	 */
	void updateCondition(const Condition& condition, int index);

	// Allow undoable commands private access
	friend class AddConditionCommand;
	friend class RemoveConditionCommand;
	friend class UpdateConditionCommand;
	// Allow rules list private access (for creating new rules)
	friend class AssignmentRules;
};

/**
 * Returns a string representation of the given condition field.
 *
 * @param[in] field condition field enumeration
 * @return string representation of the condition field
 */
QString toString(AssignmentRule::Field field);

/**
 * Returns a string representation of the given condition operator.
 *
 * @param[in] oper condition operator enumeration
 * @return string representation of the condition operator
 */
QString toString(AssignmentRule::Operator oper);

/**
 * Returns a condition operator enumeration for the
 * given string representation.
 *
 * @param[in] str string representation of the condition operator
 * @return condition operator enumeration
 */
AssignmentRule::Operator toOperatorEnum(const QString& str);

/**
 * Returns the list of condition operator string representations
 * that are valid for the given condition field.
 *
 * @param[in] field condition field enumeration
 * @return string list of the applicable condition operators
 */
QStringList operatorsFor(AssignmentRule::Field field);

}

// Make types known to Qt meta object system
Q_DECLARE_METATYPE(ub::AssignmentRule::Field);
Q_DECLARE_METATYPE(ub::AssignmentRule::Operator);
Q_DECLARE_METATYPE(ub::AssignmentRule::Condition);

#endif // ASSIGNMENTRULE_HPP
