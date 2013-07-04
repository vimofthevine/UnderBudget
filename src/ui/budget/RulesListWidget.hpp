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

#ifndef RULESLISTWIDGET_HPP
#define RULESLISTWIDGET_HPP

// Qt include(s)
#include <QTreeView>

// Forward declaration(s)
class QSortFilterProxyModel;

namespace ub {

// Forward declaration
class AssignmentRulesModel;

/**
 * Customized QTreeView for displaying assignment rules.
 */
class RulesListWidget : public QTreeView
{
	Q_OBJECT

public:
	/**
	 * Constructs a new assignment rules list widget.
	 *
	 * @param[in] model  assignment rules list model
	 * @param[in] parent parent widget
	 */
	RulesListWidget(AssignmentRulesModel* model,
		QWidget* parent = 0);

signals:
	/**
	 * Emitted when a rule is selected to indicate the current
	 * associated estimate.
	 *
	 * @param estimateId ID of the estimate associated to the currently
	 *                   selected assignment rule
	 */
	void estimateSelected(uint estimateId);

public slots:
	/**
	 * Filters the list of displayed assignment rules to only those
	 * matching the given estimate ID.
	 *
	 * @param[in] estimateId ID of the estimate on which to filter
	 */
	void filter(uint estimateId);

private slots:
	/**
	 * Responds to rule or condition selections.
	 *
	 * @param[in] current  currently selected rule/condition index
	 * @param[in] previous previously selected rule/condition index
	 */
	void selectionChanged(const QModelIndex& current, const QModelIndex& previous);

	/**
	 * Clones the currently selected assignment rule.
	 */
	void cloneSelectedRule();

	/**
	 * Deletes the currently selected assignment rule.
	 */
	void deleteSelectedRule();

	/**
	 * Creates a new condition for the currently selected assignment rule.
	 */
	void addConditionToSelectedRule();

	/**
	 * Removes the currently selected condition from its parent assignment rule.
	 */
	void removeSelectedCondition();

protected:
	/**
	 * Displays a context menu for operating on the rules list.
	 *
	 * @param[in] event context menu event
	 */
	void contextMenuEvent(QContextMenuEvent* event);

private:
	/** Assignment rules list model */
	AssignmentRulesModel* model;
	/** Filter proxy model */
	QSortFilterProxyModel* ruleFilter;
	/** Tracks whether list is currently filtered */
	bool filtered;
};

}

#endif //RULESLISTWIDGET_HPP
