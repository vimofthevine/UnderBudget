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
#include "ui/ledger/ImportedTransactionsListWidget.hpp"
#include "ui/ledger/ImportedTransactionsModel.hpp"
#include "ui/ledger/MoneyAwareSortFilterProxyModel.hpp"

namespace ub {

//------------------------------------------------------------------------------
ImportedTransactionsListWidget::ImportedTransactionsListWidget(
		ImportedTransactionsModel* model, QWidget* parent)
	: QTableView(parent), model(model), filtered(false)
{
	//filter = new QSortFilterProxyModel(this);
	filter = new MoneyAwareSortFilterProxyModel(this);
	filter->setSourceModel(model);
	setModel(filter);
	setSortingEnabled(true);
	sortByColumn(3, Qt::AscendingOrder);

	// Give the payee column the most weight
	horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

	setAlternatingRowColors(true);

	// Setup context menu
	setContextMenuPolicy(Qt::DefaultContextMenu);

	// Hide the ID columns (only used for filtering)
	hideColumn(0);
	hideColumn(1);
	hideColumn(2);
	// Columns 3-6 are always visible
	// Show standard view initially
	showStandardColumns();

	connect(selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
		this, SLOT(selectionChanged(QModelIndex, QModelIndex)));
}

//------------------------------------------------------------------------------
void ImportedTransactionsListWidget::selectionChanged(const QModelIndex& current,
	const QModelIndex& previous)
{
	qDebug() << "selection change";
}

//------------------------------------------------------------------------------
void ImportedTransactionsListWidget::filterByEstimate(uint estimateId)
{
	QString idString = QString("%1").arg(estimateId);
	filter->setFilterKeyColumn(1);
	filter->setFilterRegExp(QRegExp(idString));
	filtered = true;
}

//------------------------------------------------------------------------------
void ImportedTransactionsListWidget::filterByRule(uint ruleId)
{
	QString idString = QString("%1").arg(ruleId);
	filter->setFilterKeyColumn(1);
	filter->setFilterRegExp(QRegExp(idString));
	filtered = true;
}

//------------------------------------------------------------------------------
void ImportedTransactionsListWidget::showStandardColumns()
{
	hideColumn(7); // Withdrawal
	hideColumn(8); // Deposit
	showColumn(9); // Assigned
	hideColumn(10); // Estimate
}

//------------------------------------------------------------------------------
void ImportedTransactionsListWidget::showDetailedColumns()
{
	showColumn(7); // Withdrawal
	showColumn(8); // Deposit
	showColumn(9); // Assigned
	showColumn(10); // Estimate
}

//------------------------------------------------------------------------------
void ImportedTransactionsListWidget::showMinimalColumns()
{
	hideColumn(7); // Withdrawal
	hideColumn(8); // Deposit
	hideColumn(9); // Assigned
	hideColumn(10); // Estimate
}

//------------------------------------------------------------------------------
void ImportedTransactionsListWidget::contextMenuEvent(QContextMenuEvent* event)
{
	QAction* standardAction = new QAction(tr("Standard View"), this);
	connect(standardAction, SIGNAL(triggered()),
		this, SLOT(showStandardColumns()));

	QAction* detailedAction = new QAction(tr("Detailed View"), this);
	connect(detailedAction, SIGNAL(triggered()),
		this, SLOT(showDetailedColumns()));

	QAction* minimalAction = new QAction(tr("Minimal View"), this);
	connect(minimalAction, SIGNAL(triggered()),
		this, SLOT(showMinimalColumns()));

	QMenu* menu = new QMenu(this);
	menu->addAction(standardAction);
	menu->addAction(detailedAction);
	menu->addAction(minimalAction);
	menu->exec(event->globalPos());
}

}

