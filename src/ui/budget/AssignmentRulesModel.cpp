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
#include "ui/budget/RuleMoveProxyCommand.hpp"
#include "ui/budget/RuleRemoveProxyCommand.hpp"
#include "ui/budget/RulesRemoveProxyCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
const int EST_COL = 0;
const int ID_COL = 1;
const int FIELD_COL = 2;
const int OPER_COL = 3;
const int CASE_COL = 4;
const int VAL_COL = 5;

//------------------------------------------------------------------------------
bool fieldIsStringType(AssignmentRule::Field field)
{
	return (field != AssignmentRule::FieldNotDefined)
		&& (field != AssignmentRule::Date)
		&& (field != AssignmentRule::Amount);
}

//------------------------------------------------------------------------------
AssignmentRulesModel::AssignmentRulesModel(QSharedPointer<AssignmentRules> rules,
		QSharedPointer<Estimate> estimates, QUndoStack* stack, QObject* parent)
	: QAbstractItemModel(parent), rules(rules),
	  estimates(estimates), undoStack(stack)
{ }

//------------------------------------------------------------------------------
int AssignmentRulesModel::countFor(uint estimateId) const
{
	return rules->findFor(estimateId).size();
}

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
		case EST_COL:
			return tr("Estimate");
		case ID_COL:
			return tr("Estimate ID");
		case FIELD_COL:
			return tr("Field");
		case OPER_COL:
			return tr("Operator");
		case CASE_COL:
			return tr("Case-Sensitive?");
		case VAL_COL:
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
		return createIndex(row, column, (quintptr) 0);
	}
}

//------------------------------------------------------------------------------
QModelIndex AssignmentRulesModel::parent(const QModelIndex& index) const
{
	if (isCondition(index))
	{
		// Use rule ID stored as index internal ID
		int row = rules->indexOf(index.internalId());
		return createIndex(row, 0, (quintptr) 0);
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
	// Only case-sensitivity is checkable
	if (index.column() != CASE_COL)
		return QVariant();

	if (isCondition(index))
	{
		AssignmentRule::Condition condition = castToCondition(index);
		if (fieldIsStringType(condition.field))
			return condition.sensitive ? Qt::Checked : Qt::Unchecked;
	}
	else if (isRule(index))
	{
		// Only when rule has just one condition
		AssignmentRule* rule = castToRule(index);
		if (rule->conditionCount() == 1)
		{
			AssignmentRule::Condition condition = rule->conditionAt(0);
			if (fieldIsStringType(condition.field))
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
		case EST_COL: // Estimate name
		{
			uint estimateId = rule->estimateId();
			Estimate* estimate = estimates->find(estimateId);
			return estimate ? estimate->estimateName() : QVariant(estimateId);
		}
		case ID_COL:
			return rule->estimateId();
		case FIELD_COL:
			return (count == 1) ? toString(rule->conditionAt(0).field)
				: tr("%1 fields").arg(count);
		case OPER_COL:
			return (count == 1) ? toString(rule->conditionAt(0).op)
				: tr("%1 conditions").arg(count);
		case VAL_COL:
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
		case ID_COL:
		{
			AssignmentRule* rule = rules->find(index.internalId());
			return rule->estimateId();
		}
		case FIELD_COL:
			return toString(condition.field);
		case OPER_COL:
			return toString(condition.op);
		case VAL_COL:
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

	QVariant value;

	if (isRule(index))
	{
		AssignmentRule* rule = castToRule(index);
		if (rule->conditionCount() == 1)
		{
			switch (index.column())
			{
			case FIELD_COL:
				value.setValue<AssignmentRule::Field>(rule->conditionAt(0).field);
				break;
			case OPER_COL:
				value.setValue<AssignmentRule::Operator>(rule->conditionAt(0).op);
				break;
			case CASE_COL:
				return rule->conditionAt(0).sensitive;
			case VAL_COL:
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
		case FIELD_COL:
			value.setValue<AssignmentRule::Field>(condition.field);
			break;
		case OPER_COL:
			value.setValue<AssignmentRule::Operator>(condition.op);
			break;
		case CASE_COL:
			return condition.sensitive;
		case VAL_COL:
			return condition.value;
		default:
			return QVariant();
		}
	}

	return value;
}

//------------------------------------------------------------------------------
Qt::ItemFlags AssignmentRulesModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);

	// If the case-sensitivity column
	if (index.column() == CASE_COL)
	{
		flags |= Qt::ItemIsUserCheckable;
	}

	// If a condition and one of the condition columns (not estimate name or id)
	if (isCondition(index) && index.column() > ID_COL)
	{
		// Case-sensitivity is only editable when field is a string type
		if (index.column() == CASE_COL)
		{
			AssignmentRule::Condition condition = castToCondition(index);
			if (fieldIsStringType(condition.field))
			{
				flags |= Qt::ItemIsEditable;
			}
		}
		// All others are always editable
		else
		{
			flags |= Qt::ItemIsEditable;
		}
	}
	// If a rule and the estimate name column
	else if (isRule(index) && index.column() == EST_COL)
	{
		flags |= Qt::ItemIsDragEnabled;
	}
	// If a rule and one of the condition columns (not estimate name or id)
	else if (isRule(index) && index.column() > ID_COL)
	{
		// Only editable if only one condition for the rule
		if (castToRule(index)->conditionCount() == 1)
		{
			// Case-sensitivity is only editable when field is a string type
			if (index.column() == CASE_COL)
			{
				AssignmentRule::Condition condition = castToRule(index)->conditionAt(0);
				if (fieldIsStringType(condition.field))
				{
					flags |= Qt::ItemIsEditable;
				}
			}
			// All others are always editable
			else
			{
				flags |= Qt::ItemIsEditable;
			}
		}
	}
	// Allows moving of rules to indices between other rules
	else if ( ! index.isValid())
	{
		flags |= Qt::ItemIsDropEnabled;
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
		case FIELD_COL:
			condition.field = static_cast<AssignmentRule::Field>(value.toInt());
			// Make sure operator selection is applicable to newly selected field
			if ( ! operatorsFor(condition.field).contains(toString(condition.op)))
			{
				condition.op = toOperatorEnum(operatorsFor(condition.field).at(0));
			}
			break;
		case OPER_COL:
			condition.op = static_cast<AssignmentRule::Operator>(value.toInt());
			break;
		case CASE_COL:
			condition.sensitive = value.toBool();
			break;
		case VAL_COL:
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
QStringList AssignmentRulesModel::mimeTypes() const
{
	QStringList types;
	types << "application/rule.origin";
	return types;
}

//------------------------------------------------------------------------------
Qt::DropActions AssignmentRulesModel::supportedDragActions() const
{
	return Qt::MoveAction;
}

//------------------------------------------------------------------------------
Qt::DropActions AssignmentRulesModel::supportedDropActions() const
{
	return Qt::MoveAction;
}

//------------------------------------------------------------------------------
QMimeData* AssignmentRulesModel::mimeData(const QModelIndexList& indices) const
{
	// Serialize original rule location data
	QMimeData* mimeData = new QMimeData;
	QByteArray encoded;
	QDataStream stream(&encoded, QIODevice::WriteOnly);

	// Only one rule/index can be moved at a time
	QModelIndex index = indices.at(0);
	if (isRule(index))
	{
		qDebug() << "Serializing rule origin mime data:"
			<< "from" << index.row();
		stream << index.row();
		mimeData->setData("application/rule.origin", encoded);
	}

	return mimeData;
}

//------------------------------------------------------------------------------
bool AssignmentRulesModel::dropMimeData(const QMimeData* data,
	Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	if (action == Qt::IgnoreAction)
		return true;

	if (action != Qt::MoveAction
		|| ! data->hasFormat("application/rule.origin"))
	{
		qDebug() << "not move action or origin mime";
		return false;
	}

	// Unserialize rule's original location
	QByteArray encoded = data->data("application/rule.origin");
	QDataStream stream(&encoded, QIODevice::ReadOnly);
	int oldRow;
	stream >> oldRow;
	qDebug() << "Unserialized rule origin mime data:"
		<< "from" << oldRow;

	// Make sure new row is not < 0 (use rowCount if < 0)
	int newRow = (row < 0) ? rowCount() : row;
	// If trying to move down the list, adjust target index so it points to
	// the desired end-result index, not the index of the next item
	if (newRow > oldRow)
	{
		newRow = newRow - 1;
	}
	qDebug() << "Target rule location:"
		<< "move to" << row << newRow;

	undoStack->push(new RuleMoveProxyCommand(this, oldRow, newRow,
		rules->move(oldRow, newRow)));
	return true;
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
void AssignmentRulesModel::emitLayoutAboutToBeChanged()
{
	emit layoutAboutToBeChanged();
}

//------------------------------------------------------------------------------
void AssignmentRulesModel::emitLayoutChanged()
{
	emit layoutChanged();
}

//------------------------------------------------------------------------------
void AssignmentRulesModel::addTo(uint estimateId)
{
	undoStack->push(new RuleAddProxyCommand(this, rules->size(),
		rules->addRule(estimateId)));
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
		undoStack->push(new RuleAddProxyCommand(this, row,
			rules->cloneRule(rule->ruleId())));
	}
}

//------------------------------------------------------------------------------
void AssignmentRulesModel::remove(const QModelIndex& index)
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
		undoStack->push(new RuleRemoveProxyCommand(this, row,
			rules->removeRule(rule->ruleId())));
	}
}

//------------------------------------------------------------------------------
void AssignmentRulesModel::remove(const QList<uint>& estimates)
{
	QUndoCommand* cmd = new QUndoCommand;

	foreach(uint estimate, estimates)
	{
		rules->removeAll(estimate, cmd);
	}

	undoStack->push(new RulesRemoveProxyCommand(this, cmd));
}

//------------------------------------------------------------------------------
void AssignmentRulesModel::move(int from, int to)
{
	undoStack->push(new RuleMoveProxyCommand(this, from, to,
		rules->move(from, to)));
}

//------------------------------------------------------------------------------
void AssignmentRulesModel::addCondition(const QModelIndex& index)
{
	AssignmentRule* rule = 0;

	if (isRule(index))
	{
		rule = castToRule(index);
	}
	else if (isCondition(index))
	{
		rule = rules->find(index.internalId());
	}

	if (rule)
	{
		undoStack->push(new RuleChangeProxyCommand(this, rule->ruleId(),
			rule->addCondition(), true));
	}
}

//------------------------------------------------------------------------------
void AssignmentRulesModel::removeCondition(const QModelIndex& index)
{
	if (isCondition(index))
	{
		AssignmentRule* rule = rules->find(index.internalId());
		undoStack->push(new RuleChangeProxyCommand(this, rule->ruleId(),
			rule->removeCondition(index.row()), true));
	}
}

}

