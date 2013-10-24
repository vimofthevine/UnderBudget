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
#include "accounting/Money.hpp"
#include "ui/accounting/MoneyEdit.hpp"
#include "ui/accounting/MoneyDelegate.hpp"

namespace ub {

//------------------------------------------------------------------------------
MoneyDelegate::MoneyDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{ }

//------------------------------------------------------------------------------
QWidget* MoneyDelegate::createEditor(QWidget* parent,
	const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	MoneyEdit* moneyEditor = new MoneyEdit(parent);
	return moneyEditor;
}

//------------------------------------------------------------------------------
void MoneyDelegate::setEditorData(QWidget* editor,
	const QModelIndex& index) const
{
	Money value = index.model()->data(index, Qt::EditRole).value<Money>();
	MoneyEdit* moneyEditor = static_cast<MoneyEdit*>(editor);
	moneyEditor->setValue(value);
}

//------------------------------------------------------------------------------
void MoneyDelegate::setModelData(QWidget* editor,
	QAbstractItemModel* model, const QModelIndex& index) const
{
	MoneyEdit* moneyEditor = static_cast<MoneyEdit*>(editor);
	Money moneyValue = moneyEditor->value();
	QVariant value;
	value.setValue<Money>(moneyValue);
	model->setData(index, value, Qt::EditRole);
}

//------------------------------------------------------------------------------
void MoneyDelegate::updateEditorGeometry(QWidget* editor,
	const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}

}

