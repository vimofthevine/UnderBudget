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
#include "ui/budget/ConditionFieldDelegate.hpp"
#include "ui/budget/ConditionOperatorDelegate.hpp"
#include "ui/budget/RulesListWidget.hpp"

namespace ub {

//------------------------------------------------------------------------------
RulesListWidget::RulesListWidget(AssignmentRulesModel* model,
		QWidget* parent)
	: QTreeView(parent), model(model), filtered(false)
{
	ruleFilter = new QSortFilterProxyModel(this);
	ruleFilter->setSourceModel(model);
	ruleFilter->setFilterKeyColumn(1); // Filter on estimate ID
	setModel(ruleFilter);

	setAlternatingRowColors(true);
	setItemDelegateForColumn(2, new ConditionFieldDelegate(this));
	setItemDelegateForColumn(3, new ConditionOperatorDelegate(this));

	// Give the estimate name column the most weight
	header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	header()->setSectionResizeMode(0, QHeaderView::Stretch);

	// Setup context menu
	setContextMenuPolicy(Qt::DefaultContextMenu);

	// Hide the estimate ID column (only used for filtering)
	hideColumn(1);

	connect(selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
		this, SLOT(selectionChanged(QModelIndex, QModelIndex)));
}

//------------------------------------------------------------------------------
void RulesListWidget::selectionChanged(const QModelIndex& current,
	const QModelIndex& previous)
{
	AssignmentRule* oldRule = model->ruleAt(ruleFilter->mapToSource(previous));
	AssignmentRule* newRule = model->ruleAt(ruleFilter->mapToSource(current));
	if (oldRule != newRule)
	{
		emit estimateSelected(newRule ? newRule->estimateId() : 0);
	}
}

//------------------------------------------------------------------------------
void RulesListWidget::filter(uint estimateId)
{
	QString idString = QString("%1").arg(estimateId);
	ruleFilter->setFilterRegExp(QRegExp(idString));
	filtered = true;
}

//------------------------------------------------------------------------------
void RulesListWidget::contextMenuEvent(QContextMenuEvent* event)
{
	QAction* cloneAction = new QAction(tr("Clone Rule"), this);
	cloneAction->setEnabled(currentIndex().isValid());
	connect(cloneAction, SIGNAL(triggered()),
		this, SLOT(cloneSelectedRule()));

	QAction* delAction = new QAction(tr("Delete"), this);
	delAction->setEnabled(currentIndex().isValid());
	connect(delAction, SIGNAL(triggered()),
		this, SLOT(deleteSelectedRule()));

	QAction* addConditionAction = new QAction(tr("Add Condition"), this);
	addConditionAction->setEnabled(currentIndex().isValid());
	connect(addConditionAction, SIGNAL(triggered()),
		this, SLOT(addConditionToSelectedRule()));

	QAction* moveUpAction = new QAction(tr("Raise in Priority"), this);
	moveUpAction->setEnabled( ! filtered
		&& currentIndex().isValid()
		&& (currentIndex().row() > 0));

	QAction* moveDownAction = new QAction(tr("Lower in Priority"), this);
	moveDownAction->setEnabled( ! filtered
		&& currentIndex().isValid()
		&& (currentIndex().row() < model->rowCount() - 1));

	QMenu* menu = new QMenu(this);
	menu->addAction(cloneAction);
	menu->addAction(delAction);
	menu->addSeparator();
	menu->addAction(addConditionAction);
	menu->addSeparator();
	menu->addAction(moveUpAction);
	menu->addAction(moveDownAction);
	menu->exec(event->globalPos());
}

//------------------------------------------------------------------------------
void RulesListWidget::cloneSelectedRule()
{
	model->clone(ruleFilter->mapToSource(currentIndex()));
}

//------------------------------------------------------------------------------
void RulesListWidget::deleteSelectedRule()
{
	model->remove(ruleFilter->mapToSource(currentIndex()));
}

//------------------------------------------------------------------------------
void RulesListWidget::addConditionToSelectedRule()
{
	model->addCondition(ruleFilter->mapToSource(currentIndex()));
}

}

