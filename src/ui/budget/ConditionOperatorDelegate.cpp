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
#include "ui/budget/ConditionOperatorDelegate.hpp"

namespace ub {

//------------------------------------------------------------------------------
ConditionOperatorDelegate::ConditionOperatorDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{ }

//------------------------------------------------------------------------------
QWidget* ConditionOperatorDelegate::createEditor(QWidget* parent,
	const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QComboBox* combo = new QComboBox(parent);
//	combo->addItem(toString(AssignmentRule::OperatorNotDefined));
//	combo->addItem(toString(AssignmentRule::BeginsWith));
//	combo->addItem(toString(AssignmentRule::EndsWith));
//	combo->addItem(toString(AssignmentRule::StringEquals));
//	combo->addItem(toString(AssignmentRule::Contains));
//	combo->addItem(toString(AssignmentRule::Before));
//	combo->addItem(toString(AssignmentRule::After));
//	combo->addItem(toString(AssignmentRule::DateEquals));
//	combo->addItem(toString(AssignmentRule::LessThan));
//	combo->addItem(toString(AssignmentRule::LessThanOrEqual));
//	combo->addItem(toString(AssignmentRule::GreaterThan));
//	combo->addItem(toString(AssignmentRule::GreaterThanOrEqual));
//	combo->addItem(toString(AssignmentRule::AmountEquals));
	return combo;
}

//------------------------------------------------------------------------------
void ConditionOperatorDelegate::setEditorData(QWidget* editor,
	const QModelIndex& index) const
{
	AssignmentRule::Operator oper
		= index.model()->data(index, Qt::EditRole).value<AssignmentRule::Operator>();

	// The field is displayed in the column to the left of the operator
	QModelIndex fieldIndex = index.model()->index(index.row(),
		index.column() - 1, index.parent());
	// Get the field for the condition being edited
	AssignmentRule::Field field
		= index.model()->data(fieldIndex, Qt::EditRole).value<AssignmentRule::Field>();

	QComboBox* combo = static_cast<QComboBox*>(editor);
	combo->clear();
	combo->addItems(operatorsFor(field));
	combo->setCurrentIndex(combo->findText(toQString(oper)));
}

//------------------------------------------------------------------------------
void ConditionOperatorDelegate::setModelData(QWidget* editor,
	QAbstractItemModel* model, const QModelIndex& index) const
{
	QComboBox* combo = static_cast<QComboBox*>(editor);
	AssignmentRule::Operator value = toOperatorEnum(combo->currentText());
	model->setData(index, value, Qt::EditRole);
}

//------------------------------------------------------------------------------
void ConditionOperatorDelegate::updateEditorGeometry(QWidget* editor,
	const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}

}

