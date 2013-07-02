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
#include "ui/budget/RulesListWidget.hpp"

namespace ub {

//------------------------------------------------------------------------------
RulesListWidget::RulesListWidget(AssignmentRulesModel* model,
		QWidget* parent)
	: QTreeView(parent), model(model)
{
	setModel(model);
	setAlternatingRowColors(true);

	// Give the estimate name column the most weight
	header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	header()->setSectionResizeMode(0, QHeaderView::Stretch);

	connect(selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
		this, SLOT(selectionChanged(QModelIndex, QModelIndex)));
}

//------------------------------------------------------------------------------
void RulesListWidget::selectionChanged(const QModelIndex& current,
	const QModelIndex& previous)
{
	AssignmentRule* oldRule = model->ruleAt(previous);
	AssignmentRule* newRule = model->ruleAt(current);
	if (oldRule != newRule)
	{
		emit estimateSelected(newRule->estimateId());
	}
}

}

