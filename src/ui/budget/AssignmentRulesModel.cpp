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
#include "ui/budget/RuleAddProxyCommand.hpp"
#include "ui/budget/RuleChangeProxyCommand.hpp"

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
	return 6;
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
			return tr("Estimate ID");
		case 2:
			return tr("Field");
		case 3:
			return tr("Operator");
		case 4:
			return tr("Case-Sensitive?");
		case 5:
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
QModelIndex AssignmentRulesModel::indexFor(uint ruleId) const
{
	return index(rules->indexOf(ruleId), 0, QModelIndex());
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
	if (role == Qt::CheckStateRole)
		return checkStateData(index);
	if (role == Qt::DisplayRole)
		return displayData(index);
	if (role == Qt::EditRole)
		return editData(index);
	return QVariant();
}

//------------------------------------------------------------------------------
QVariant AssignmentRulesModel::checkStateData(const QModelIndex& index) const
{
	// Only column 4 (case-sensitivity) is checkable
	if (index.column() != 4)
		return QVariant();

	if (isCondition(index))
	{
		AssignmentRule::Condition condition = castToCondition(index);
		return condition.sensitive ? Qt::Checked : Qt::Unchecked;
	}
	else if (isRule(index))
	{
		// Only when rule has just one condition
		AssignmentRule* rule = castToRule(index);
		if (rule->conditionCount() == 1)
		{
			AssignmentRule::Condition condition = rule->conditionAt(0);
			return condition.sensitive ? Qt::Checked : Qt::Unchecked;
		}
	}

	return QVariant();
}

//------------------------------------------------------------------------------
QVariant AssignmentRulesModel::displayData(const QModelIndex& index) const
{
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
			return rule->estimateId();
		case 2:
			return (count == 1) ? toString(rule->conditionAt(0).field)
				: tr("%1 fields").arg(count);
		case 3:
			return (count == 1) ? toString(rule->conditionAt(0).op)
				: tr("%1 conditions").arg(count);
		case 5:
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
		{
			AssignmentRule* rule = rules->find(index.internalId());
			return rule->estimateId();
		}
		case 2:
			return toString(condition.field);
		case 3:
			return toString(condition.op);
		case 5:
			return condition.value;
		default:
			return QVariant();
		}
	}

	return QVariant();
}

//------------------------------------------------------------------------------
QVariant AssignmentRulesModel::editData(const QModelIndex& index) const
{
	if ( ! index.isValid())
		return QVariant();

	if (isRule(index))
	{
		AssignmentRule* rule = castToRule(index);
		if (rule->conditionCount() == 1)
		{
			switch (index.column())
			{
			case 2:
				return rule->conditionAt(0).field;
			case 3:
				return rule->conditionAt(0).op;
			case 4:
				return rule->conditionAt(0).sensitive;
			case 5:
				return rule->conditionAt(0).value;
			default:
				return QVariant();
			}
		}
	}
	else if (isCondition(index))
	{
		AssignmentRule::Condition condition = castToCondition(index);
		switch (index.column())
		{
		case 2:
			return condition.field;
		case 3:
			return condition.op;
		case 4:
			return condition.sensitive;
		case 5:
			return condition.value;
		default:
			return QVariant();
		}
	}

	return QVariant();
}

//------------------------------------------------------------------------------
Qt::ItemFlags AssignmentRulesModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);

	// If the case-sensitivity column
	if (index.column() == 4)
	{
		flags |= Qt::ItemIsUserCheckable;
	}

	// If a condition and one of the condition columns (not 0 or 1)
	if (isCondition(index) && index.column() > 1)
	{
		flags |= Qt::ItemIsEditable;
	}
	// If a rule and one of the condition columns (not 0 or 1)
	else if (isRule(index) && index.column() > 1)
	{
		// Only editable if only one condition for the rule
		if (castToRule(index)->conditionCount() == 1)
		{
			flags |= Qt::ItemIsEditable;
		}
	}

	return flags;
}

//------------------------------------------------------------------------------
bool AssignmentRulesModel::setData(const QModelIndex& index,
	const QVariant& value, int role)
{
	AssignmentRule* rule = 0;
	AssignmentRule::Condition condition;
	int row = -1;

	if (isRule(index))
	{
		rule = castToRule(index);
		if (rule->conditionCount() == 1)
		{
			condition = rule->conditionAt(0);
			row = 0;
		}
	}
	else if (isCondition(index))
	{
		rule = rules->find(index.internalId());
		condition = castToCondition(index);
		row = index.row();
	}

	if (rule && row >= 0)
	{
		switch (index.column())
		{
		case 2:
			condition.field = static_cast<AssignmentRule::Field>(value.toInt());
			break;
		case 3:
			condition.op = static_cast<AssignmentRule::Operator>(value.toInt());
			break;
		case 4:
			condition.sensitive = value.toBool();
			break;
		case 5:
			condition.value = value.toString();
			break;
		default:
			break;
		}

		undoStack->push(new RuleChangeProxyCommand(this, rule->ruleId(),
			rule->updateCondition(row, condition)));
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------
void AssignmentRulesModel::emitDataChanged(const QModelIndex& changed)
{
	// Emit signal for the entire row
	emit dataChanged(
		index(changed.row(), 0, changed.parent()),
		index(changed.row(), columnCount(changed)-1, changed.parent())
	);
}

//------------------------------------------------------------------------------
void AssignmentRulesModel::clone(const QModelIndex& index)
{
	AssignmentRule* rule = 0;
	int row = -1;

	if (isRule(index))
	{
		rule = castToRule(index);
		row = index.row();
	}
	else if (isCondition(index))
	{
		rule = rules->find(index.internalId());
		row = rules->indexOf(rule->ruleId());
	}

	if (rule && row >= 0)
	{
		qDebug() << "cloneing" << rule->ruleId() << "at" << row;
		undoStack->push(new RuleAddProxyCommand(this, row,
			rules->cloneRule(rule->ruleId())));
	}
}

}

