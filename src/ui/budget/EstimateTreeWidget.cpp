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
#include "ui/budget/AssignmentRulesModel.hpp"
#include "ui/budget/EstimateModel.hpp"
#include "ui/budget/EstimateTreeWidget.hpp"
#include "ui/budget/ProgressDelegate.hpp"

namespace ub {

//------------------------------------------------------------------------------
EstimateTreeWidget::EstimateTreeWidget(EstimateModel* model,
		AssignmentRulesModel* rules, QWidget* parent)
	: QTreeView(parent), model(model), rules(rules)
{
	setModel(model);
	expandAll();
	showEstimateDefinitionColumns();

	// Give the name column the most weight
	header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	header()->setSectionResizeMode(0, QHeaderView::Stretch);

	// Setup context menu
	setContextMenuPolicy(Qt::DefaultContextMenu);

	// Enable re-ordering/moving
	setDragDropMode(QAbstractItemView::InternalMove);

	// Display progress bar for estimate progress
	setItemDelegateForColumn(7, new ProgressDelegate(this));

	// Record expanded/collapsed items
	connect(this, SIGNAL(expanded(QModelIndex)),
		model, SLOT(recordExpanded(QModelIndex)));
	connect(this, SIGNAL(collapsed(QModelIndex)),
		model, SLOT(recordCollapsed(QModelIndex)));
}

//------------------------------------------------------------------------------
void EstimateTreeWidget::contextMenuEvent(QContextMenuEvent* event)
{
	// Actions specific to the selected estimate

	QAction* addChildAction = new QAction(tr("Add Child"), this);
	addChildAction->setEnabled(currentIndex().isValid());
	connect(addChildAction, SIGNAL(triggered()),
		this, SLOT(addChildToSelectedEstimate()));

	QAction* delAction = new QAction(tr("Delete"), this);
	delAction->setEnabled(currentIndex().isValid());
	connect(delAction, SIGNAL(triggered()), this, SLOT(deleteSelectedEstimate()));

	QAction* addRuleAction = new QAction(tr("Add Rule"), this);
	addRuleAction->setEnabled(currentIndex().isValid()
		&& (model->at(currentIndex())->childCount() == 0));
	connect(addRuleAction, SIGNAL(triggered()), this, SLOT(addRuleToSelectedEstimate()));

	// Global actions

	QAction* addToRootAction = new QAction(tr("Add New Estimate"), this);
	connect(addToRootAction, SIGNAL(triggered()),
		this, SLOT(addToRootEstimate()));

	QMenu* menu = new QMenu(this);
	menu->addAction(addChildAction);
	menu->addAction(delAction);
	menu->addSeparator();
	menu->addAction(addRuleAction);
	menu->addSeparator();
	menu->addAction(addToRootAction);
	menu->exec(event->globalPos());
}

//------------------------------------------------------------------------------
void EstimateTreeWidget::showEstimateDefinitionColumns()
{
	int column;

	// Have to display columns before hiding others, otherwise the widths get
	// all screwed up
	foreach (column, model->definitionFieldColumns())
	{
		showColumn(column);
	}

	foreach (column, model->progressFieldColumns())
	{
		if (column == 0) continue;
		hideColumn(column);
	}
	foreach (column, model->impactFieldColumns())
	{
		if (column == 0) continue;
		hideColumn(column);
	}
}

//------------------------------------------------------------------------------
void EstimateTreeWidget::showEstimateProgressColumns()
{
	int column;

	// Have to display columns before hiding others, otherwise the widths get
	// all screwed up
	foreach (column, model->progressFieldColumns())
	{
		showColumn(column);
	}

	foreach (column, model->definitionFieldColumns())
	{
		if (column == 0) continue;
		hideColumn(column);
	}
	foreach (column, model->impactFieldColumns())
	{
		if (column == 0) continue;
		hideColumn(column);
	}
}

//------------------------------------------------------------------------------
void EstimateTreeWidget::showBalanceImpactColumns()
{
	int column;

	// Have to display columns before hiding others, otherwise the widths get
	// all screwed up
	foreach (column, model->impactFieldColumns())
	{
		showColumn(column);
	}

	foreach (column, model->definitionFieldColumns())
	{
		if (column == 0) continue;
		hideColumn(column);
	}
	foreach (column, model->progressFieldColumns())
	{
		if (column == 0) continue;
		hideColumn(column);
	}
}

//------------------------------------------------------------------------------
void EstimateTreeWidget::deleteSelectedEstimate()
{
	QModelIndex current = currentIndex();
	if (current.isValid())
	{
		model->deleteEstimate(current);
	}
}

//------------------------------------------------------------------------------
void EstimateTreeWidget::addChildToSelectedEstimate()
{
	QModelIndex current = currentIndex();
	if (current.isValid())
	{
		model->addChild(current);
	}
}

//------------------------------------------------------------------------------
void EstimateTreeWidget::addRuleToSelectedEstimate()
{
	QModelIndex current = currentIndex();
	if (current.isValid())
	{
		Estimate* estimate = model->at(current);
		if (estimate)
		{
			rules->addTo(estimate->estimateId());
		}
	}
}

//------------------------------------------------------------------------------
void EstimateTreeWidget::addToRootEstimate()
{
	model->addChild(QModelIndex());
}

}

