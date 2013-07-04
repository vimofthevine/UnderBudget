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
#include "budget/Estimate.hpp"
#include "ui/accounting/MoneyEdit.hpp"
#include "ui/budget/EstimateDetailsForm.hpp"
#include "ui/budget/EstimateModel.hpp"
#include "ui/widgets/IgnoreUndoRedo.hpp"
#include "ui/widgets/LineEdit.hpp"

namespace ub {

//------------------------------------------------------------------------------
EstimateDetailsForm::EstimateDetailsForm(EstimateModel* model,
		QUndoStack* stack, QWidget* parent)
	: QWidget(parent), model(model), undoStack(stack)
{
	// Setup name field
	nameField = new LineEdit("", this);
	nameField->setEnabled(false);
	connect(nameField, SIGNAL(textEdited(QString)),
		this, SLOT(updateName(QString)));

	// Setup description field
	descriptionField = new LineEdit("", this);
	descriptionField->setEnabled(false);
	connect(descriptionField, SIGNAL(textEdited(QString)),
		this, SLOT(updateDescription(QString)));

	// Setup type field
	typeField = new QComboBox(this);
	typeField->addItem(tr("Expense"), Estimate::Expense);
	typeField->addItem(tr("Income"), Estimate::Income);
	typeField->addItem(tr("Transfer"), Estimate::Transfer);
	typeField->setEnabled(false);
	connect(typeField, SIGNAL(currentIndexChanged(int)),
		this, SLOT(updateType(int)));

	// Setup amount field
	amountField = new MoneyEdit(this);
	amountField->setEnabled(false);
	connect(amountField, SIGNAL(valueEdited(Money)),
		this, SLOT(updateAmount(Money)));

	// Setup due date field
	dueDateField = new QDateEdit(this);
	dueDateField->installEventFilter(new IgnoreUndoRedo(this, this));
	dueDateField->setCalendarPopup(true);
	dueDateField->setSpecialValueText(tr("None"));
	dueDateField->setEnabled(false);
	clearDueDate();
	connect(dueDateField, SIGNAL(dateChanged(QDate)),
		this, SLOT(updateDueDate(QDate)));

	// Setup clear-due date button
	clearDueDateButton = new QPushButton(tr("Clear"), this);
	clearDueDateButton->setEnabled(false);
	connect(clearDueDateButton, SIGNAL(clicked(bool)),
		this, SLOT(clearDueDate()));

	// Put due date entry and clear button side-by-side
	QHBoxLayout* dueDateLayout = new QHBoxLayout;
	dueDateLayout->addWidget(dueDateField, 1);
	dueDateLayout->addWidget(clearDueDateButton);

	// Setup finished field
	finishedField = new QCheckBox(this);
	connect(finishedField, SIGNAL(toggled(bool)),
		this, SLOT(updateFinishedState(bool)));

	// Setup form layout
	QFormLayout* form = new QFormLayout;
	form->addRow(tr("Name"), nameField);
	form->addRow(tr("Description"), descriptionField);
	form->addRow(tr("Type"), typeField);
	form->addRow(tr("Amount"), amountField);
	form->addRow(tr("Due Date"), dueDateLayout);
	form->addRow(tr("Finished"), finishedField);

	// Add to widget
	setLayout(form);

	// Get updated data when the estimates change
	connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
		this, SLOT(dataChanged(QModelIndex,QModelIndex)));
}

//------------------------------------------------------------------------------
void EstimateDetailsForm::estimateSelected(const QModelIndex& current,
	const QModelIndex& previous)
{
	// Reset our current index so we don't do anything when we modify the fields
	currentIndex = QModelIndex();

	// If an invalid index, clear out everything
	if ( ! current.isValid())
	{
		nameField->clear();
		descriptionField->clear();
		typeField->setCurrentIndex(-1);
		amountField->clear();
		clearDueDate();
		finishedField->setChecked(false);

		nameField->setEnabled(false);
		descriptionField->setEnabled(false);
		typeField->setEnabled(false);
		amountField->setEnabled(false);
		dueDateField->setEnabled(false);
		finishedField->setEnabled(false);
	}
	else
	{
		populate(current);
	}

	// Store the current index so we can use it to update the estimate
	currentIndex = current;
}

//------------------------------------------------------------------------------
void EstimateDetailsForm::populate(const QModelIndex& index)
{
	Estimate* estimate = static_cast<Estimate*>(index.internalPointer());

	// Populate fields with selected estimate's values
	nameField->setText(estimate->estimateName());
	descriptionField->setText(estimate->estimateDescription());
	typeField->setCurrentIndex(typeField->findData(estimate->estimateType()));
	amountField->setValue(estimate->estimatedAmount());
	finishedField->setChecked(estimate->isActivityFinished());

	QDate dueDate = estimate->activityDueDate();
	if (dueDate.isNull())
	{
		clearDueDate();
	}
	else
	{
		dueDateField->setDate(dueDate);
	}

	// Enable fields that are applicable for all non-root estimates
	nameField->setEnabled(estimate->parentEstimate());
	descriptionField->setEnabled(estimate->parentEstimate());
	// Disable fields that aren't applicable for parents
	amountField->setEnabled(estimate->childCount() == 0);
	dueDateField->setEnabled(estimate->childCount() == 0);
	clearDueDateButton->setEnabled(estimate->childCount() == 0);
	finishedField->setEnabled(estimate->childCount() == 0);

	// Type can only be changed at top-level estimates
	Estimate* parent = estimate->parentEstimate();
	if ( ! parent) // Parent is root
	{
		typeField->setEnabled(false);
	}
	else if ( ! parent->parentEstimate()) // Parent of parent is root
	{
		typeField->setEnabled(true);
	}
	else // Not top-level
	{
		typeField->setEnabled(false);
	}

	// Clear fields that aren't applicable for parents
	if (estimate->childCount() != 0)
	{
		amountField->clear();
		clearDueDate();
		finishedField->setChecked(false);
	}
}

//------------------------------------------------------------------------------
void EstimateDetailsForm::dataChanged(const QModelIndex& topLeft,
	const QModelIndex& bottomRight)
{
	QItemSelectionRange range(topLeft, bottomRight);
	if ( ! bottomRight.isValid() || range.contains(currentIndex))
	{
		populate(currentIndex);
	}
}

//------------------------------------------------------------------------------
void EstimateDetailsForm::clearDueDate()
{
	dueDateField->setDate(dueDateField->minimumDate());
}

//------------------------------------------------------------------------------
void EstimateDetailsForm::updateName(const QString& name)
{
	if (currentIndex.isValid())
	{
		model->updateName(currentIndex, name);
	}
}

//------------------------------------------------------------------------------
void EstimateDetailsForm::updateDescription(const QString& descrip)
{
	if (currentIndex.isValid())
	{
		model->updateDescription(currentIndex, descrip);
	}
}

//------------------------------------------------------------------------------
void EstimateDetailsForm::updateType(int typeIndex)
{
	if (currentIndex.isValid())
	{
		int typeInt = typeField->itemData(typeIndex).toInt();
		Estimate::Type type = static_cast<Estimate::Type>(typeInt);
		model->updateType(currentIndex, type);
	}
}

//------------------------------------------------------------------------------
void EstimateDetailsForm::updateAmount(const Money& amount)
{
	if (currentIndex.isValid())
	{
		model->updateAmount(currentIndex, amount);
	}
}

//------------------------------------------------------------------------------
void EstimateDetailsForm::updateDueDate(const QDate& date)
{
	if (currentIndex.isValid())
	{
		QDate newDate = (date == dueDateField->minimumDate())
			? QDate() : date;

		model->updateDueDate(currentIndex, newDate);
	}
}

//------------------------------------------------------------------------------
void EstimateDetailsForm::updateFinishedState(bool finished)
{
	if (currentIndex.isValid())
	{
		model->updateFinishedState(currentIndex, finished);
	}
}

}
