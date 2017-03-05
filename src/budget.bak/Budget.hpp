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

#ifndef BUDGET_HPP
#define BUDGET_HPP

// Qt include(s)
#include <QObject>
#include <QSharedPointer>

// UnderBudget include(s)
#include "budget/BudgetingPeriod.hpp"
#include "budget/Estimate.hpp"

// Forward declaration(s)
class QUndoCommand;

namespace ub {

// Forward declaration(s)
class AssignmentRules;
class Balance;
class UIPrefs;

/**
 * Budget definition.
 *
 * @ingroup budget
 */
class Budget : public QObject {
	Q_OBJECT

public:
	/**
	 * Constructs a new, unpopulated budget with default values.
	 */
	Budget();

	/**
	 * Constructs a new budget with the given parameters.
	 *
	 * @param[in] name    budget name
	 * @param[in] period  budgeting period
	 * @param[in] initial initial balance
	 * @param[in] root    root of the estimate tree
	 * @param[in] rules   assignment rules list
	 * @param[in] uiPrefs budget-specific UI preferences
	 */
	Budget(const QString& name, QSharedPointer<BudgetingPeriod> period,
		QSharedPointer<Balance> initial, QSharedPointer<Estimate> root,
		QSharedPointer<AssignmentRules> rules,
		QSharedPointer<UIPrefs> uiPrefs);

	/**
	 * Returns the user-defined name for this budget.
	 *
	 * @return budget name
	 */
	QString name() const;

	/**
	 * Returns the budgeting period for which this budget applies.
	 *
	 * @return budgeting period
	 */
	QSharedPointer<BudgetingPeriod> budgetingPeriod() const;

	/**
	 * Returns the initial, starting balance for this budget.
	 *
	 * @return initial balance
	 */
	QSharedPointer<Balance> initialBalance() const;

	/**
	 * Returns the root to the estimate tree.
	 *
	 * @return estimate tree root
	 */
	QSharedPointer<Estimate> estimates() const;

	/**
	 * Returns the assignment rules list.
	 *
	 * @return assignment rules list
	 */
	QSharedPointer<AssignmentRules> rules() const;

	/**
	 * Returns the budget-specific UI preferences.
	 *
	 * @return budget-specific UI preferences.
	 */
	QSharedPointer<UIPrefs> uiPreferences() const;

	/**
	 * Creates an undoable command to change the budget's name.
	 * Ownership of the returned pointer is transfered to the
	 * caller of this function.
	 *
	 * @param[in] newName new budget name
	 * @param[in] parent  parent undoable command for grouping
	 * @return undoable command to apply the name change
	 */
	QUndoCommand* changeName(const QString& newName, QUndoCommand* parent = 0);

signals:
	/**
	 * Emitted when the name of the budget is changed.
	 *
	 * @param name new budget name
	 */
	void nameChanged(const QString& name);

private:
	/** User-defined name */
	QString budgetName;
	/** Budgeting period */
	QSharedPointer<BudgetingPeriod> period;
	/** Initial balance */
	QSharedPointer<Balance> initial;
	/** Estimate tree (root) */
	QSharedPointer<Estimate> rootEstimate;
	/** Assignment rules */
	QSharedPointer<AssignmentRules> assignmentRules;
	/** UI Preferences */
	QSharedPointer<UIPrefs> uiPrefs;

	/**
	 * Sets the budget's name to the given name, emitting
	 * the `nameChanged` signal.
	 *
	 * @param[in] name new budget name
	 */
	void setName(const QString& name);

	// Allow undoable commands direct field access
	friend class ChangeBudgetNameCommand;
};

}

#endif //BUDGET_HPP
