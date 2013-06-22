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
class QTabWidget;

namespace ub {

// Forward declaration(s)
class Estimate;
class EstimateModel;
class EstimateTreeWidget;

/**
 * A widget for displaying all estimates in the current budget.
 *
 * The widget includes a tree view for viewing all estimates in their
 * hierarchical form, as well as views for modifying estimates and their
 * associated assignment rules.
 */
class EstimateDisplayWidget : public QSplitter
{
	Q_OBJECT

public:
	/**
	 * Constructs a new estimate display widget.
	 *
	 * @param[in] root   root of the estimate tree
	 * @param[in] stack  undoable command stack
	 * @param[in] parent parent widget
	 */
	EstimateDisplayWidget(QSharedPointer<Estimate> root,
		QUndoStack* stack, QWidget* parent = 0);

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

	/** Undo stack for all commands */
	QUndoStack* undoStack;
};

}

#endif //ESTIMATEDISPLAYWIDGET_HPP
