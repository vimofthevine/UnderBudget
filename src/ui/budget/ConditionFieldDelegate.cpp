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
#include "ui/budget/ConditionFieldDelegate.hpp"

namespace ub {

//------------------------------------------------------------------------------
ConditionFieldDelegate::ConditionFieldDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{ }

//------------------------------------------------------------------------------
QWidget* ConditionFieldDelegate::createEditor(QWidget* parent,
	const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QComboBox* combo = new QComboBox(parent);
	combo->addItem(toString(AssignmentRule::FieldNotDefined));
	combo->addItem(toString(AssignmentRule::Date));
	combo->addItem(toString(AssignmentRule::Amount));
	combo->addItem(toString(AssignmentRule::Payee));
	combo->addItem(toString(AssignmentRule::Memo));
	combo->addItem(toString(AssignmentRule::DepositAccount));
	combo->addItem(toString(AssignmentRule::WithdrawalAccount));
	return combo;
}

//------------------------------------------------------------------------------
void ConditionFieldDelegate::setEditorData(QWidget* editor,
	const QModelIndex& index) const
{
	AssignmentRule::Field field
		= index.model()->data(index, Qt::EditRole).value<AssignmentRule::Field>();
	QComboBox* combo = static_cast<QComboBox*>(editor);
	combo->setCurrentIndex(combo->findText(toString(field)));
}

//------------------------------------------------------------------------------
void ConditionFieldDelegate::setModelData(QWidget* editor,
	QAbstractItemModel* model, const QModelIndex& index) const
{
	QComboBox* combo = static_cast<QComboBox*>(editor);
	AssignmentRule::Field value
		= static_cast<AssignmentRule::Field>(combo->currentIndex());
	model->setData(index, value, Qt::EditRole);
}

//------------------------------------------------------------------------------
void ConditionFieldDelegate::updateEditorGeometry(QWidget* editor,
	const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}

}

