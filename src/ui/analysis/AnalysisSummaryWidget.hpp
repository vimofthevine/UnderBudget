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

#ifndef ANALYSISSUMMARYWIDGET_HPP
#define ANALYSISSUMMARYWIDGET_HPP

// Qt include(s)
#include <QSharedPointer>
#include <QWidget>

// Forward declaration(s)
class QLineEdit;

namespace ub {

// Forward declaration(s)
class BudgetingPeriod;
class EstimateDiffsModel;
class ProjectedBalanceModel;

/**
 * Analysis summary display widget.
 */
class AnalysisSummaryWidget : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Constructs an analysis summary widget.
	 */
	AnalysisSummaryWidget(QSharedPointer<BudgetingPeriod> period,
		ProjectedBalanceModel* balances, EstimateDiffsModel* over,
		EstimateDiffsModel* under, QWidget* parent = 0);

	/**
	 * Sets the imported transaction source location to be displayed.
	 *
	 * @param[in] loc imported transaction source location
	 */
	void setImportedTransactionSource(const QString& loc);

	/**
	 * Sets the number of imported transactions to be displayed.
	 *
	 * @param[in] num number of imported transactions
	 */
	void setNumberOfImportedTransactions(int num);

	/**
	 * Sets the number of assigned transactions to be displayed.
	 *
	 * @param[in] num number of assigned transactions
	 */
	void setNumberOfAssignedTransactions(int num);

	/**
	 * Sets the number of over-budget estimates to be displayed.
	 *
	 * @param[in] num number of over-budget estimates
	 */
	void setNumberOfOverBudgetEstimates(int num);

	/**
	 * Sets the number of under-budget estimates to be displayed.
	 *
	 * @param[in] num number of under-budget estimates
	 */
	void setNumberOfUnderBudgetEstimates(int num);

private:
	/** Imported transaction source display widget */
	QLineEdit* importSource;
	/** Imported transaction count display widget */
	QLineEdit* importCount;
	/** Assigned transaction count display widget */
	QLineEdit* assignedCount;
	/** Over-budget estimates count display widget */
	QLineEdit* overBudgetCount;
	/** Under-budget estimates count display widget */
	QLineEdit* underBudgetCount;
};

}

#endif //ANALYSISSUMMARYWIDGET_HPP
