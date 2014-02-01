/*
 * Copyright 2014 Kyle Treubig
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
#include "budget/BudgetingPeriod.hpp"
#include "ui/analysis/AnalysisSummaryWidget.hpp"
#include "ui/analysis/EstimateDiffsModel.hpp"
#include "ui/analysis/ProjectedBalanceModel.hpp"

namespace ub {

//------------------------------------------------------------------------------
AnalysisSummaryWidget::AnalysisSummaryWidget(QSharedPointer<BudgetingPeriod> period,
		ProjectedBalanceModel* balances, EstimateDiffsModel* over,
		EstimateDiffsModel* under, QWidget* parent)
	: QWidget(parent)
{
	importSource = new QLineEdit(tr("None"));
	importSource->setReadOnly(true);
	importCount = new QLineEdit("0");
	importCount->setReadOnly(true);
	assignedCount = new QLineEdit("0");
	assignedCount->setReadOnly(true);
	overBudgetCount = new QLineEdit("0");
	overBudgetCount->setReadOnly(true);
	underBudgetCount = new QLineEdit("0");
	underBudgetCount->setReadOnly(true);

	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow(tr("Import Source"), importSource);
	formLayout->addRow(tr("Number of Imported Transactions"), importCount);
	formLayout->addRow(tr("Number of Assigned Transactions"), assignedCount);
	formLayout->addRow(tr("Number of Over-budget Estimates"), overBudgetCount);
	formLayout->addRow(tr("Number of Under-budget Estimates"), underBudgetCount);

	QTableView* balanceTable = new QTableView(this);
	balanceTable->setModel(balances);
	balanceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	balanceTable->setSelectionBehavior(QTableView::SelectRows);

	QTableView* overBudgetTable = new QTableView(this);
	overBudgetTable->setModel(over);
	overBudgetTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	overBudgetTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	overBudgetTable->setSelectionBehavior(QTableView::SelectRows);

	QTableView* underBudgetTable = new QTableView(this);
	underBudgetTable->setModel(under);
	underBudgetTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	underBudgetTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	underBudgetTable->setSelectionBehavior(QTableView::SelectRows);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addLayout(formLayout);
	layout->addWidget(new QLabel(tr("Projected Ending Balances")));
	layout->addWidget(balanceTable);
	layout->addWidget(new QLabel(tr("Top Over-budget Estimates")));
	layout->addWidget(overBudgetTable);
	layout->addWidget(new QLabel(tr("Top Under-budget Estimates")));
	layout->addWidget(underBudgetTable);
	setLayout(layout);
}

//------------------------------------------------------------------------------
void AnalysisSummaryWidget::setImportedTransactionSource(const QString& loc)
{
	importSource->setText(loc);
}

//------------------------------------------------------------------------------
void AnalysisSummaryWidget::setNumberOfImportedTransactions(int num)
{
	importCount->setText(QString("%1").arg(num));
}

//------------------------------------------------------------------------------
void AnalysisSummaryWidget::setNumberOfAssignedTransactions(int num)
{
	assignedCount->setText(QString("%1").arg(num));
}

//------------------------------------------------------------------------------
void AnalysisSummaryWidget::setNumberOfOverBudgetEstimates(int num)
{
	overBudgetCount->setText(QString("%1").arg(num));
}

//------------------------------------------------------------------------------
void AnalysisSummaryWidget::setNumberOfUnderBudgetEstimates(int num)
{
	underBudgetCount->setText(QString("%1").arg(num));
}

}

