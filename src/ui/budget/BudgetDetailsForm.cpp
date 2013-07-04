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
#include "ui/accounting/MoneyEdit.hpp"
#include "ui/budget/BudgetDetailsForm.hpp"
#include "ui/budget/BudgetingPeriodForm.hpp"
#include "ui/widgets/LineEdit.hpp"

namespace ub {

//------------------------------------------------------------------------------
BudgetDetailsForm::BudgetDetailsForm(QSharedPointer<Budget> budget,
		QUndoStack* stack, QWidget* parent)
	: QWidget(parent), budget(budget), undoStack(stack)
{
	// Setup name field
	nameField = new LineEdit(budget->name(), this);
	connect(nameField, SIGNAL(textEdited(QString)),
		this, SLOT(updateName(QString)));
	connect(budget.data(), SIGNAL(nameChanged(QString)),
		nameField, SLOT(setText(QString)));

	// Setup initial balance field
	initialBalanceField = new MoneyEdit(budget->initialBalance(), this);
	connect(initialBalanceField, SIGNAL(valueEdited(Money)),
		this, SLOT(updateInitialBalance(Money)));
	connect(budget.data(), SIGNAL(initialBalanceChanged(Money)),
		initialBalanceField, SLOT(setValue(Money)));

	// Setup budgeting period field
	periodField = new BudgetingPeriodForm(budget->budgetingPeriod(), stack, this);

	// Setup form layout
	QFormLayout* form = new QFormLayout;
	form->addRow(tr("Name"), nameField);
	form->addRow(tr("Initial Balance"), initialBalanceField);
	form->addRow(tr("Budgeting Period"), periodField);

	// Setup group box
	QGroupBox* group = new QGroupBox(tr("Budget Details"));
	group->setLayout(form);

	// Add to widget
	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(group);
	setLayout(layout);
}

//------------------------------------------------------------------------------
void BudgetDetailsForm::updateName(const QString& name)
{
	undoStack->push(budget->changeName(name));
}

//------------------------------------------------------------------------------
void BudgetDetailsForm::updateInitialBalance(const Money& amount)
{
	undoStack->push(budget->changeInitialBalance(amount));
}

}

