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
#include "ui/accounting/MoneyEdit.hpp"
#include "ui/budget/AssignmentRulesModel.hpp"
#include "ui/budget/ConditionValueDelegate.hpp"
#include "ui/widgets/IgnoreUndoRedo.hpp"
#include "ui/widgets/LineEdit.hpp"

namespace ub {

//------------------------------------------------------------------------------
ConditionValueDelegate::ConditionValueDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{ }

//------------------------------------------------------------------------------
AssignmentRule::Field ConditionValueDelegate::field(const QModelIndex& index) const
{
	// The field is displayed in the 3rd column to the left of the value
	QModelIndex fieldIndex = index.model()->index(index.row(),
		index.column() - 3, index.parent());
	// Get the field for the condition being edited
	return index.model()->data(fieldIndex, Qt::EditRole).value<AssignmentRule::Field>();
}

//------------------------------------------------------------------------------
QWidget* ConditionValueDelegate::createEditor(QWidget* parent,
	const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	switch (field(index))
	{
	case AssignmentRule::Date:
	{
		QDateEdit* dateEditor = new QDateEdit(parent);
		dateEditor = new QDateEdit(parent);
		dateEditor->installEventFilter(new IgnoreUndoRedo(parent, parent));
		dateEditor->setCalendarPopup(true);
		dateEditor->setSpecialValueText(tr("None"));
		return dateEditor;
	}

	case AssignmentRule::Amount:
		return new MoneyEdit(parent);

	default:
		return new LineEdit("", parent);
	}
}

//------------------------------------------------------------------------------
void ConditionValueDelegate::setEditorData(QWidget* editor,
	const QModelIndex& index) const
{
	QString value = index.model()->data(index, Qt::EditRole).toString();

	switch (field(index))
	{
	case AssignmentRule::Date:
	{
		QDateEdit* dateEditor = static_cast<QDateEdit*>(editor);
		QDate date = QVariant(value).toDate();
		if (date.isValid())
			dateEditor->setDate(date);
		else
			dateEditor->setDate(QDate::currentDate());
	}
		break;

	case AssignmentRule::Amount:
	{
		MoneyEdit* moneyEditor = static_cast<MoneyEdit*>(editor);
		if ( ! value.contains(","))
			return;
		QStringList amtParts = value.split(",");
		if (amtParts.size() != 2)
			return;
		Money amount = Money(QVariant(amtParts[0]).toDouble(), amtParts[1]);
		moneyEditor->setValue(amount);
	}
		break;

	default:
	{
		LineEdit* stringEditor = static_cast<LineEdit*>(editor);
		stringEditor->setText(value);
	}
		break;
	}
}

//------------------------------------------------------------------------------
void ConditionValueDelegate::setModelData(QWidget* editor,
	QAbstractItemModel* model, const QModelIndex& index) const
{
	switch (field(index))
	{
	case AssignmentRule::Date:
	{
		QDateEdit* dateEditor = static_cast<QDateEdit*>(editor);
		QDate date = dateEditor->date();
		model->setData(index, date.toString(Qt::ISODate), Qt::EditRole);
	}
		break;

	case AssignmentRule::Amount:
	{
		MoneyEdit* moneyEditor = static_cast<MoneyEdit*>(editor);
		Money amount = moneyEditor->value();
		QString newVal = QString("%1,%2")
			.arg(amount.amount()).arg(amount.currency().code());
		model->setData(index, newVal, Qt::EditRole);
	}
		break;

	default:
	{
		LineEdit* stringEditor = static_cast<LineEdit*>(editor);
		model->setData(index, stringEditor->text(), Qt::EditRole);
	}

	}
}

//------------------------------------------------------------------------------
void ConditionValueDelegate::updateEditorGeometry(QWidget* editor,
	const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}

}

