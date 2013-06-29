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
#include "ui/budget/EstimateDetailsForm.hpp"
#include "ui/budget/EstimateDisplayWidget.hpp"
#include "ui/budget/EstimateModel.hpp"
#include "ui/budget/EstimateTreeWidget.hpp"

namespace ub {

//------------------------------------------------------------------------------
EstimateDisplayWidget::EstimateDisplayWidget(QSharedPointer<Estimate> root,
		QUndoStack* stack, QWidget* parent)
	: QSplitter(Qt::Vertical, parent), root(root), undoStack(stack)
{
	model = new EstimateModel(root, undoStack, this);

	tree = new EstimateTreeWidget(model, this);
	selectionModel = tree->selectionModel();

	EstimateDetailsForm* details = new EstimateDetailsForm(model, undoStack, this);
	connect(selectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
		details, SLOT(estimateSelected(QModelIndex,QModelIndex)));

	tabs = new QTabWidget(this);
	tabs->addTab(details, tr("Estimate Details"));

	addWidget(tree);
	addWidget(tabs);

	// Give tree stretch priority
	setStretchFactor(0, 1);
}

//------------------------------------------------------------------------------
void EstimateDisplayWidget::showEstimateDefinitions()
{
	tree->showEstimateDefinitionColumns();
}

//------------------------------------------------------------------------------
void EstimateDisplayWidget::showEstimateProgress()
{
	tree->showEstimateProgressColumns();
}

//------------------------------------------------------------------------------
void EstimateDisplayWidget::showBalanceImpact()
{
	tree->showBalanceImpactColumns();
}

}

