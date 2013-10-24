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
#include "ui/accounting/MoneyDelegate.hpp"
#include "ui/budget/BalanceContributorsModel.hpp"
#include "ui/budget/ContributorsListWidget.hpp"

namespace ub {

//------------------------------------------------------------------------------
ContributorsListWidget::ContributorsListWidget(
		BalanceContributorsModel* model, QWidget* parent)
	: QTableView(parent), model(model)
{
	setModel(model);
	setAlternatingRowColors(true);
	setItemDelegateForColumn(1, new MoneyDelegate(this));

	// Give the contributor name column the most weight
	horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

	// Setup context menu
	setContextMenuPolicy(Qt::DefaultContextMenu);

	// Add shortcut actions
	QAction* delAction = new QAction(tr("Delete"), this);
	delAction->setShortcut(QKeySequence::Delete);
	connect(delAction, SIGNAL(triggered()),
		this, SLOT(deleteSelectedContributor()));
	addAction(delAction);
}

//------------------------------------------------------------------------------
void ContributorsListWidget::contextMenuEvent(QContextMenuEvent* event)
{
	QAction* addAction = new QAction(tr("Add Contributor"), this);
	connect(addAction, SIGNAL(triggered()),
		this, SLOT(createContributor()));

	QAction* delAction = new QAction(tr("Delete"), this);
	delAction->setShortcut(QKeySequence::Delete);
	delAction->setEnabled(currentIndex().isValid());
	connect(delAction, SIGNAL(triggered()),
		this, SLOT(deleteSelectedContributor()));

	QMenu* menu = new QMenu(this);
	menu->addAction(addAction);
	menu->addAction(delAction);
	menu->exec(event->globalPos());
}

//------------------------------------------------------------------------------
void ContributorsListWidget::createContributor()
{
	model->add();
}

//------------------------------------------------------------------------------
void ContributorsListWidget::deleteSelectedContributor()
{
	model->remove(currentIndex());
}

}

