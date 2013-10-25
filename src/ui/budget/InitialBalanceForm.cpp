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
#include "ui/budget/BalanceContributorsModel.hpp"
#include "ui/budget/ContributorsListWidget.hpp"
#include "ui/budget/InitialBalanceForm.hpp"

namespace ub {

//------------------------------------------------------------------------------
InitialBalanceForm::InitialBalanceForm(QSharedPointer<Balance> balance,
		QUndoStack* stack, QWidget* parent)
	: QWidget(parent), undoStack(stack), balance(balance)
{
	// Set up balance value field
	valueField = new MoneyEdit(balance->value(), this);
	connect(valueField, SIGNAL(valueEdited(Money)),
		this, SLOT(updateValue(Money)));
	connect(balance.data(), SIGNAL(contributorAdded(Contributor, int)),
		this, SLOT(updateDisplayedValue()));
	connect(balance.data(), SIGNAL(contributorRemoved(Contributor, int)),
		this, SLOT(updateDisplayedValue()));
	connect(balance.data(), SIGNAL(contributorUpdated(Contributor, int)),
		this, SLOT(updateDisplayedValue()));

	// Set up expand/collapse button
	expand = QIcon(":/icons/expand");
	collapse = QIcon(":/icons/collapse");
	toggleButton = new QPushButton(expand, "", this);
	toggleButton->setCheckable(true);
	connect(toggleButton, SIGNAL(toggled(bool)), this, SLOT(showAdvanced(bool)));

	// Set up contributors table
	BalanceContributorsModel* model
		= new BalanceContributorsModel(balance, stack, this);
	contributors = new ContributorsListWidget(model, this);
	contributors->hide();

	// Set up add-contributor button
	addButton = new QPushButton(QIcon(":/icons/add"), "", this);
	addButton->hide();
	connect(addButton, SIGNAL(clicked()), contributors, SLOT(createContributor()));

	// Set up delete-contributor button
	deleteButton = new QPushButton(QIcon(":/icons/delete"), "", this);
	deleteButton->setEnabled(false);
	deleteButton->hide();
	connect(deleteButton, SIGNAL(clicked()), contributors,
		SLOT(deleteSelectedContributor()));
	connect(contributors, SIGNAL(canDelete(bool)),
		deleteButton, SLOT(setEnabled(bool)));

	// Put it all together...
	QHBoxLayout* minimal = new QHBoxLayout;
	minimal->addWidget(valueField);
	minimal->addWidget(toggleButton);
	minimal->addWidget(addButton);
	minimal->addWidget(deleteButton);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addLayout(minimal);
	layout->addWidget(contributors);
	setLayout(layout);

	// Initialize field
	updateDisplayedValue();
}

//------------------------------------------------------------------------------
void InitialBalanceForm::updateValue(const Money& value)
{
	Balance::Contributor contributor = balance->contributorAt(0);
	contributor.amount = value;
	undoStack->push(balance->updateContributor(0, contributor));
}

//------------------------------------------------------------------------------
void InitialBalanceForm::updateDisplayedValue()
{
	valueField->setValue(balance->value());

	// Always disable the field when the advanced editor is open
	if (toggleButton->isChecked())
	{
		valueField->setEnabled(false);
	}
	else
	{
		valueField->setEnabled(balance->contributorCount() < 2);
	}
}

//------------------------------------------------------------------------------
void InitialBalanceForm::showAdvanced(bool show)
{
	if (show)
	{
		toggleButton->setIcon(collapse);
		contributors->show();
		addButton->show();
		deleteButton->show();

		// Make sure net value field is disabled--regardless of whether there
		// is only one contributor or not. This forces the user to edit the
		// sole contributor.
		valueField->setEnabled(false);
	}
	else
	{
 		toggleButton->setIcon(expand);
		contributors->hide();
		addButton->hide();
		deleteButton->hide();

		// Make sure net value field will be re-enabled if necessary
		updateDisplayedValue();
	}
}

}

