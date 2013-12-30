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
#include "analysis/Actuals.hpp"
#include "ui/budget/AssignmentRulesModel.hpp"
#include "ui/budget/EstimateDetailsForm.hpp"
#include "ui/budget/EstimateDisplayWidget.hpp"
#include "ui/budget/EstimateModel.hpp"
#include "ui/budget/EstimateTreeWidget.hpp"
#include "ui/budget/RulesListWidget.hpp"
#include "ui/ledger/ImportedTransactionsListWidget.hpp"
#include "ui/ledger/ImportedTransactionsModel.hpp"

namespace ub {

//------------------------------------------------------------------------------
EstimateDisplayWidget::EstimateDisplayWidget(QSharedPointer<Estimate> root,
		AssignmentRulesModel* rules, ImportedTransactionsModel* trns,
		Actuals* actuals, QUndoStack* stack, QWidget* parent)
	: QSplitter(Qt::Vertical, parent), root(root), undoStack(stack)
{
	model = new EstimateModel(root, rules, actuals, undoStack, this);

	tree = new EstimateTreeWidget(model, rules, this);
	selectionModel = tree->selectionModel();

	// Estimate details
	EstimateDetailsForm* details = new EstimateDetailsForm(model, undoStack, this);
	connect(selectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
		details, SLOT(estimateSelected(QModelIndex,QModelIndex)));

	// Associated rules
	ruleList = new RulesListWidget(rules, this);
	ruleList->filter(999999999); // Show nothing initially
	connect(selectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
		this, SLOT(updateRuleFilter(QModelIndex,QModelIndex)));

	// Assigned transactions
	transactionList = new ImportedTransactionsListWidget(trns);
	transactionList->filterByEstimate(999999999); // Show nothing initially
	connect(selectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
		this, SLOT(updateTransactionFilter(QModelIndex,QModelIndex)));

	tabs = new QTabWidget(this);
	tabs->addTab(details, tr("Estimate Details"));
	tabs->addTab(ruleList, tr("Associated Rules"));
	tabs->addTab(transactionList, tr("Assigned Transactions"));

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

//------------------------------------------------------------------------------
void EstimateDisplayWidget::selectEstimate(uint estimateId)
{
	QModelIndex index = model->index(estimateId);
	selectionModel->setCurrentIndex(index,
		QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

//------------------------------------------------------------------------------
void EstimateDisplayWidget::updateRuleFilter(const QModelIndex& current,
	const QModelIndex& previous)
{
	if (current.isValid())
	{
		Estimate* estimate = static_cast<Estimate*>(current.internalPointer());
		ruleList->filter(estimate->estimateId());
	}
	else
	{
		ruleList->filter(0); // Show nothing
	}
}

//------------------------------------------------------------------------------
void EstimateDisplayWidget::updateTransactionFilter(const QModelIndex& current,
	const QModelIndex& previous)
{
	if (current.isValid())
	{
		Estimate* estimate = static_cast<Estimate*>(current.internalPointer());
		transactionList->filterByEstimate(estimate->estimateId());
	}
	else
	{
		transactionList->filterByEstimate(999999999); // Show nothing
	}
}

}

