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
	updateValue();
	connect(balance.data(), SIGNAL(contributorAdded(const Contributor&, int)),
		this, SLOT(updateValue()));
	connect(balance.data(), SIGNAL(contributorRemoved(const Contributor&, int)),
		this, SLOT(updateValue()));
	connect(balance.data(), SIGNAL(contributorUpdated(const Contributor&, int)),
		this, SLOT(updateValue()));

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

	// Put it all together...
	QHBoxLayout* minimal = new QHBoxLayout;
	minimal->addWidget(valueField);
	minimal->addWidget(toggleButton);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addLayout(minimal);
	layout->addWidget(contributors);
	setLayout(layout);
}

//------------------------------------------------------------------------------
void InitialBalanceForm::updateValue()
{
	valueField->setValue(balance->value());
	valueField->setEnabled(balance->contributorCount() < 2);
}

//------------------------------------------------------------------------------
void InitialBalanceForm::showAdvanced(bool show)
{
	if (show)
	{
		toggleButton->setIcon(collapse);
		contributors->show();
	}
	else
	{
 		toggleButton->setIcon(expand);
		contributors->hide();
	}
}

}

