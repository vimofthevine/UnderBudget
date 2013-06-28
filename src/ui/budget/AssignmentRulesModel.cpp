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

// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "budget/AssignmentRule.hpp"
#include "ui/budget/AssignmentRulesModel.hpp"

namespace ub {

//------------------------------------------------------------------------------
AssignmentRulesModel::AssignmentRulesModel(QSharedPointer<AssignmentRules> rules,
		QUndoStack* stack, QObject* parent)
	: QAbstractItemModel(parent), rules(rules), undoStack(stack)
{ }

//------------------------------------------------------------------------------
bool AssignmentRulesModel::isRule(const QModelIndex& index) const
{
	// Is a rule when index is valid but it's parent is not
	return index.isValid() && ! index.parent().isValid();
}

//------------------------------------------------------------------------------
bool AssignmentRulesModel::isCondition(const QModelIndex& index) const
{
	return index.isValid() && isRule(index.parent());
}

//------------------------------------------------------------------------------
AssignmentRule* AssignmentRulesModel::castToRule(const QModelIndex& index) const
{
	return static_cast<AssignmentRule*>(index.internalPointer());
}

//------------------------------------------------------------------------------
AssignmentRule::Condition AssignmentRulesModel::castToCondition(
	const QModelIndex& index) const
{
	AssignmentRule* rule = castToRule(index.parent());
	return rule->conditionAt(index.row());
}

//------------------------------------------------------------------------------
int AssignmentRulesModel::columnCount(const QModelIndex& parent) const
{
	if (isRule(parent))
		return 4; // Conditions have 4 columns (field, operator, sensitive, value)
	if (isCondition(parent))
		return 0; // No items under conditions
	return 1; // Rules have 1 column (estimate)
}

//------------------------------------------------------------------------------
QVariant AssignmentRulesModel::headerData(int section, Qt::Orientation orientation,
	int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (section)
		{
		case 0:
			return tr("Estimate");
		case 1:
			return tr("Field");
		case 2:
			return tr("Operator");
		case 3:
			return tr("Case-Sensitive?");
		case 4:
			return tr("Value");
		default:
			return QVariant();
		}
	}
	else
		return QVariant();
}

//------------------------------------------------------------------------------
int AssignmentRulesModel::rowCount(const QModelIndex& parent) const
{
	if (parent.column() > 0)
		return 0;

	if (isRule(parent))
		return castToRule(parent)->conditionCount();
	else if (isCondition(parent))
		return 0;
	else
		return rules->size();
}

//------------------------------------------------------------------------------
QModelIndex AssignmentRulesModel::index(int row, int column,
	const QModelIndex& parent) const
{
	if ( ! hasIndex(row, column, parent))
		return QModelIndex();

	if (isRule(parent))
	{
		// Need to return index to condition
		AssignmentRule::Condition condition = castToRule(parent)->conditionAt(row);
		return createIndex(row, column, 0);
	}
	else if (isRule(condition))
		return QModelIndex(); // Conditions don't have child items
	else
	{
		// Need to return index to rule
		AssignmentRule* rule = rules->at(row);
		return createIndex(row, column, rule);
	}
}

//------------------------------------------------------------------------------
QModelIndex AssignmentRulesModel::parent(const QModelIndex& index) const
{
	if (isCondition(index))
	{
		uint ruleId = castToRule(index.parent())->ruleId();
		int index = rules->indexOf(ruleId);
		return createIndex(index, 0, QModelIndex());
	}
	else // If a rule or the root index itself
		return QModelIndex();
}

//------------------------------------------------------------------------------
QVariant AssignmentRulesModel::data(const QModelIndex& index, int role) const
{
	if ( ! index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	if (isRule(index) && index.column() == 0)
	{
		return castToRule(index)->estimateId();
	}
	else if (isCondition(index))
	{
		AssignmentRule::Condition condition = castToCondition(index);
		switch (index.column())
		{
		case 0:
			return condition.field;
		case 1:
			return condition.op;
		case 2:
			return condition.sensitive;
		case 3:
			return condition.value;
		default:
			return QVariant();
		}
	}
	else
		return QVariant();
}

}

