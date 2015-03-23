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
#include "ui/budget/ConditionValueDelegate.hpp"
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
	setItemDelegateForColumn(5, new ConditionValueDelegate(this));

	// Give the estimate name column the most weight
	header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	header()->setSectionResizeMode(0, QHeaderView::Stretch);

	// Setup context menu
	setContextMenuPolicy(Qt::DefaultContextMenu);

	// Hide the estimate ID column (only used for filtering)
	hideColumn(1);

	// Enable re-ordering/moving
	setDragDropMode(QAbstractItemView::InternalMove);

	connect(selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
		this, SLOT(selectionChanged(QModelIndex, QModelIndex)));

	// Add shortcut actions
	QAction* delAction = new QAction(tr("Delete"), this);
	delAction->setShortcut(QKeySequence::Delete);
	connect(delAction, SIGNAL(triggered()),
		this, SLOT(deleteSelectedRule()));
	addAction(delAction);

	QAction* moveUpAction = new QAction(tr("Raise in Priority"), this);
	moveUpAction->setShortcut(Qt::CTRL + Qt::Key_Up);
	connect(moveUpAction, SIGNAL(triggered()),
		this, SLOT(moveSelectedRuleUp()));
	addAction(moveUpAction);

	QAction* moveDownAction = new QAction(tr("Lower in Priority"), this);
	moveDownAction->setShortcut(Qt::CTRL + Qt::Key_Down);
	connect(moveDownAction, SIGNAL(triggered()),
		this, SLOT(moveSelectedRuleDown()));
	addAction(moveDownAction);
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
	setDragDropMode(QAbstractItemView::NoDragDrop);
}

//------------------------------------------------------------------------------
void RulesListWidget::contextMenuEvent(QContextMenuEvent* event)
{
	QAction* cloneAction = new QAction(tr("Clone Rule"), this);
	cloneAction->setEnabled(currentIndex().isValid());
	connect(cloneAction, SIGNAL(triggered()),
		this, SLOT(cloneSelectedRule()));

	QAction* delAction = new QAction(tr("Delete"), this);
	delAction->setShortcut(QKeySequence::Delete);
	delAction->setEnabled(currentIndex().isValid());
	connect(delAction, SIGNAL(triggered()),
		this, SLOT(deleteSelectedRule()));

	QAction* addConditionAction = new QAction(tr("Add Condition"), this);
	addConditionAction->setEnabled(currentIndex().isValid());
	connect(addConditionAction, SIGNAL(triggered()),
		this, SLOT(addConditionToSelectedRule()));

	QAction* removeConditionAction = new QAction(tr("Remove Condition"), this);
	removeConditionAction->setEnabled(currentIndex().isValid()
		&& model->isCondition(ruleFilter->mapToSource(currentIndex())));
	connect(removeConditionAction, SIGNAL(triggered()),
		this, SLOT(removeSelectedCondition()));

	QAction* moveUpAction = new QAction(tr("Raise in Priority"), this);
	moveUpAction->setShortcut(Qt::CTRL + Qt::Key_Up);
	moveUpAction->setEnabled( ! filtered
		&& currentIndex().isValid()
		&& (currentIndex().row() > 0));
	connect(moveUpAction, SIGNAL(triggered()),
		this, SLOT(moveSelectedRuleUp()));

	QAction* moveDownAction = new QAction(tr("Lower in Priority"), this);
	moveDownAction->setShortcut(Qt::CTRL + Qt::Key_Down);
	moveDownAction->setEnabled( ! filtered
		&& currentIndex().isValid()
		&& (currentIndex().row() < model->rowCount() - 1));
	connect(moveDownAction, SIGNAL(triggered()),
		this, SLOT(moveSelectedRuleDown()));

	QMenu* menu = new QMenu(this);
	menu->addAction(cloneAction);
	menu->addAction(delAction);
	menu->addSeparator();
	menu->addAction(addConditionAction);
	menu->addAction(removeConditionAction);
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
void RulesListWidget::moveSelectedRuleUp()
{
	if (filtered)
		return;

	QModelIndex index = ruleFilter->mapToSource(currentIndex());
	// Can't move first rule up any higher
	if (index.row() > 0)
	{
		model->move(index.row(), index.row() - 1);
	}
}

//------------------------------------------------------------------------------
void RulesListWidget::moveSelectedRuleDown()
{
	if (filtered)
		return;

	QModelIndex index = ruleFilter->mapToSource(currentIndex());
	// Can't move last rule down any lower
	if (index.row() < (model->rowCount() - 1))
	{
		model->move(index.row(), index.row() + 1);
	}
}

//------------------------------------------------------------------------------
void RulesListWidget::addConditionToSelectedRule()
{
	model->addCondition(ruleFilter->mapToSource(currentIndex()));
}

//------------------------------------------------------------------------------
void RulesListWidget::removeSelectedCondition()
{
	model->removeCondition(ruleFilter->mapToSource(currentIndex()));
}

}

