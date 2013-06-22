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

#ifndef ESTIMATETREEWIDGET_HPP
#define ESTIMATETREEWIDGET_HPP

// Qt include(s)
#include <QTreeView>

namespace ub {

// Forward declaration
class EstimateModel;

/**
 * Customized QTreeView for displaying estimates.
 */
class EstimateTreeWidget : public QTreeView
{
	Q_OBJECT

public:
	/**
	 * Constructs a new estimate tree widget.
	 *
	 * @param[in] model  estimate tree model
	 * @param[in] parent parent widget
	 */
	EstimateTreeWidget(EstimateModel* model,
		QWidget* parent = 0);

public slots:
	/**
	 * Displays only the columns relevant to estimate progress.
	 */
	void showEstimateProgressColumns();

	/**
	 * Displays only the columns relevant to balance impact.
	 */
	void showBalanceImpactColumns();

private slots:
	/**
	 * Deletes the currently selected estimate.
	 */
	void deleteSelectedEstimate();

	/**
	 * Adds a child to the currently selected estimate.
	 */
	void addChildToSelectedEstimate();

	/**
	 * Adds a child to the root estimate.
	 */
	void addToRootEstimate();

protected:
	/**
	 * Displays a context menu for operating on the estimate tree.
	 *
	 * @param[in] event context menu event
	 */
	void contextMenuEvent(QContextMenuEvent* event);

private:
	/** Estimate tree model */
	EstimateModel* model;
};

}

#endif //ESTIMATETREEWIDGET_HPP
