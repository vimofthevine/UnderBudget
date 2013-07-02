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
	: QTreeView(parent), model(model)
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
		emit estimateSelected(newRule->estimateId());
	}
}

//------------------------------------------------------------------------------
void RulesListWidget::filter(uint estimateId)
{
	QString idString = QString("%1").arg(estimateId);
	ruleFilter->setFilterRegExp(QRegExp(idString));
}

}

