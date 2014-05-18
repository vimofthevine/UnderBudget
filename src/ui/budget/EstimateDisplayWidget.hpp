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

#ifndef ESTIMATEDISPLAYWIDGET_HPP
#define ESTIMATEDISPLAYWIDGET_HPP

// Qt include(s)
#include <QSharedPointer>
#include <QSplitter>

// Forward declaration(s)
class QItemSelectionModel;
class QSortFilterProxyModel;
class QTabWidget;

namespace ub {

// Forward declaration(s)
class Actuals;
class AssignmentRulesModel;
class Estimate;
class EstimateModel;
class EstimateTreeWidget;
class ImportedTransactionsListWidget;
class ImportedTransactionsModel;
class RulesListWidget;

/**
 * A widget for displaying all estimates in the current budget.
 *
 * The widget includes a tree view for viewing all estimates in their
 * hierarchical form, as well as views for modifying estimates and their
 * associated assignment rules.
 *
 * @ingroup ui_budget
 */
class EstimateDisplayWidget : public QSplitter
{
	Q_OBJECT

public:
	/**
	 * Constructs a new estimate display widget.
	 *
	 * @param[in] root    root of the estimate tree
	 * @param[in] rules   assignment rules model
	 * @param[in] trns    imported transactions model
	 * @param[in] actuals actual activity values
	 * @param[in] stack   undoable command stack
	 * @param[in] parent  parent widget
	 */
	EstimateDisplayWidget(QSharedPointer<Estimate> root,
		AssignmentRulesModel* rules, ImportedTransactionsModel* trns,
		Actuals* actuals, QUndoStack* stack, QWidget* parent = 0);

public slots:
	/**
	 * Displays the definitions of each estimate.
	 */
	void showEstimateDefinitions();

	/**
	 * Displays the progress of each estimate as a comparison against
	 * actual activity data.
	 */
	void showEstimateProgress();

	/**
	 * Displays the impact of each estimate on the ending balances.
	 */
	void showBalanceImpact();

	/**
	 * Updates the current selection to that of the given estimate ID.
	 *
	 * @param[in] estimateId ID of the estimate to be selected
	 */
	void selectEstimate(uint estimateId);

private slots:
	/**
	 * Updates the filtered rules list according to the currently
	 * selected estimate.
	 *
	 * @param[in] current  currently selected estimate index
	 * @param[in] previous previously selected estimate index
	 */
	void updateRuleFilter(const QModelIndex& current, const QModelIndex& previous);

	/**
	 * Updates the filtered transactions list according to the currently
	 * selected estimate.
	 *
	 * @param[in] current  currently selected estimate index
	 * @param[in] previous previously selected estimate index
	 */
	void updateTransactionFilter(const QModelIndex& current, const QModelIndex& previous);

private:
	/** Root estimate */
	QSharedPointer<Estimate> root;
	/** Estimate model */
	EstimateModel* model;
	/** Estimate selection model */
	QItemSelectionModel* selectionModel;

	/** Estimate tree widget */
	EstimateTreeWidget* tree;
	/** Tab widget for detailed views */
	QTabWidget* tabs;
	/** Associated rules list */
	RulesListWidget* ruleList;
	/** Assigned transactions list */
	ImportedTransactionsListWidget* transactionList;

	/** Undo stack for all commands */
	QUndoStack* undoStack;
};

}

#endif //ESTIMATEDISPLAYWIDGET_HPP
