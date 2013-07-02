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
#include "budget/Estimate.hpp"
#include "ui/budget/AssignmentRulesModel.hpp"

namespace ub {

//------------------------------------------------------------------------------
AssignmentRulesModel::AssignmentRulesModel(QSharedPointer<AssignmentRules> rules,
		QSharedPointer<Estimate> estimates, QUndoStack* stack, QObject* parent)
	: QAbstractItemModel(parent), rules(rules),
	  estimates(estimates), undoStack(stack)
{ }

//------------------------------------------------------------------------------
AssignmentRule* AssignmentRulesModel::ruleAt(const QModelIndex& index) const
{
	if (isRule(index))
		return castToRule(index);
	else if (isCondition(index))
		return rules->find(index.internalId());
	else
		return 0; // Not a valid index
}

//------------------------------------------------------------------------------
bool AssignmentRulesModel::isRule(const QModelIndex& index) const
{
	// Is a rule when index is valid and "containing rule ID" is 0
	return index.isValid() && (index.internalId() == 0);
}

//------------------------------------------------------------------------------
bool AssignmentRulesModel::isCondition(const QModelIndex& index) const
{
	// Is a condition when index is valid and has a containing rule ID
	return index.isValid() && (index.internalId() != 0);
}

//------------------------------------------------------------------------------
AssignmentRule* AssignmentRulesModel::castToRule(const QModelIndex& index) const
{
	// Rule index is stored in the index row
	int row = index.row();
	if (row >= 0 && row < rules->size())
		return rules->at(row);
	return 0;
}

//------------------------------------------------------------------------------
AssignmentRule::Condition AssignmentRulesModel::castToCondition(
	const QModelIndex& index) const
{
	// Rule ID is stored as the index internal ID
	AssignmentRule* rule = rules->find(index.internalId());
	return rule->conditionAt(index.row());
}

//------------------------------------------------------------------------------
int AssignmentRulesModel::columnCount(const QModelIndex& parent) const
{
	return 5;
	//if (isRule(parent))
		//return 4; // Conditions have 4 columns (field, operator, sensitive, value)
	//if (isCondition(parent))
		//return 0; // No items under conditions
	//return 1; // Rules have 1 column (estimate)
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
	{
		// If only one condition under rule, display it as the same row (no children)
		int count = castToRule(parent)->conditionCount();
		return (count == 1) ? 0 : count;
	}
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
		AssignmentRule* rule = castToRule(parent);
		return createIndex(row, column, rule->ruleId());
	}
	else if (isCondition(parent))
		return QModelIndex(); // Conditions don't have child items
	else
	{
		// Need to return index to rule
		return createIndex(row, column, (uint) 0);
	}
}

//------------------------------------------------------------------------------
QModelIndex AssignmentRulesModel::parent(const QModelIndex& index) const
{
	if (isCondition(index))
	{
		// Use rule ID stored as index internal ID
		int row = rules->indexOf(index.internalId());
		return createIndex(row, 0, (uint) 0);
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

	if (isRule(index))
	{
		AssignmentRule* rule = castToRule(index);
		int count = rule->conditionCount();

		switch (index.column())
		{
		case 0: // Estimate name
		{
			uint estimateId = rule->estimateId();
			Estimate* estimate = estimates->find(estimateId);
			return estimate ? estimate->estimateName() : QVariant(estimateId);
		}
		case 1:
			return (count == 1) ? toString(rule->conditionAt(0).field)
				: tr("%1 fields").arg(count);
		case 2:
			return (count == 1) ? toString(rule->conditionAt(0).op)
				: tr("%1 conditions").arg(count);
		case 3:
			return (count == 1) ? rule->conditionAt(0).sensitive
				: QVariant();
		case 4:
			return (count == 1) ? rule->conditionAt(0).value
				: tr("%1 values").arg(count);
		default:
			return QVariant();
		}
	}
	else if (isCondition(index))
	{
		AssignmentRule::Condition condition = castToCondition(index);
		switch (index.column())
		{
		case 1:
			return toString(condition.field);
		case 2:
			return toString(condition.op);
		case 3:
			return condition.sensitive;
		case 4:
			return condition.value;
		default:
			return QVariant();
		}
	}
	else
		return QVariant();
}

}

