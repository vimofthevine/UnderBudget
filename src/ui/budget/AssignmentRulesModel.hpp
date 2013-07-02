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

#ifndef ASSIGNMENTRULESMODEL_HPP
#define ASSIGNMENTRULESMODEL_HPP

// Qt include(s)
#include <QAbstractItemModel>
#include <QSharedPointer>

// UnderBudget include(s)
#include "budget/AssignmentRules.hpp"

// Forward declaration(s)
class QUndoStack;

namespace ub {

// Forward declaration(s)
class Estimate;

/**
 * Assignment rules list model to serve as a proxy between various UI
 * views and the backing assignment rules list structure.
 */
class AssignmentRulesModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	/**
	 * Constructs a new estimate tree model.
	 *
	 * @param[in] rules     assignment rules list
	 * @param[in] estimates root of the estimate tree
	 * @param[in] stack     undo stack
	 * @param[in] parent    parent object
	 */
	AssignmentRulesModel(QSharedPointer<AssignmentRules> rules,
		QSharedPointer<Estimate> estimates,
		QUndoStack* stack, QObject* parent = 0);

	/**
	 * Returns the assignment rule located at the specified index. If the
	 * index is for a condition, the containing rule is returned.
	 *
	 * @param[in] index model index at which to retrieve a rule
	 * @return assignment rule located at the specified index
	 */
	AssignmentRule* ruleAt(const QModelIndex& index) const;

	// Overridden methods

	/**
	 * Reimplemented to return the number of columns/fields according
	 * to whether the parent is the root index or a rule index.
	 */
	int columnCount(const QModelIndex& parent = QModelIndex()) const;

	/**
	 * Reimplemented to return a title for the specified column/field.
	 */
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;

	/**
	 * Reimpelmented to return the number of rules when parent is
	 * the root index, and the number of conditions when the parent
	 * is a rule index.
	 */
	int rowCount(const QModelIndex& parent = QModelIndex()) const;

	/**
	 * Reimplemented to return an index of a condition under a rule.
	 */
	QModelIndex index(int row, int column,
		const QModelIndex& parent = QModelIndex()) const;

	/**
	 * Reimplemented to return an index of the containing rule
	 * for a given condition.
	 */
	QModelIndex parent(const QModelIndex& index) const;

	/**
	 * Reimplemented to return the appropriate data field for
	 * the rule or condition at the given index.
	 */
	QVariant data(const QModelIndex& index, int role) const;

private:
	/** Assignment rules list */
	QSharedPointer<AssignmentRules> rules;
	/** Estimate tree */
	QSharedPointer<Estimate> estimates;
	/** Undo stack for all commands */
	QUndoStack* undoStack;

	/**
	 * Extracts the assignment rule object references by the model
	 * index. This method must only be called when the index is
	 * known to be valid and referencing a rule.
	 *
	 * @param[in] index model index
	 * @return pointer to assignment rule referenced by the model index
	 */
	AssignmentRule* castToRule(const QModelIndex& index) const;

	/**
	 * Extracts the rule condition object referenced by the model
	 * index. This method must only be called when the index is
	 * known to be valid and referencing a rule condition.
	 *
	 * @param[in] index model index
	 * @return assignment rule condition referenced by the model index
	 */
	AssignmentRule::Condition castToCondition(const QModelIndex& index) const;

	/**
	 * Checks if the given index references an assignment rule.
	 *
	 * @param[in] index model index
	 * @return `true` if the index references an assignment rule
	 */
	bool isRule(const QModelIndex& index) const;

	/**
	 * Checks if the given index references a rule condition.
	 *
	 * @param[in] index model index
	 * @return `true` if the index references a rule condition
	 */
	bool isCondition(const QModelIndex& index) const;
};

}

#endif //ASSIGNMENTRULESMODEL_HPP
