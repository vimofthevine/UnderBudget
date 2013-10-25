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

#ifndef CONTRIBUTORSLISTWIDGET_HPP
#define CONTRIBUTORSLISTWIDGET_HPP

// Qt include(s)
#include <QTableView>

namespace ub {

// Forward declaration
class BalanceContributorsModel;

/**
 * Customized QTableView for displaying balance contributors.
 */
class ContributorsListWidget : public QTableView
{
	Q_OBJECT

public:
	/**
	 * Constructs a new balance contributors list widget.
	 *
	 * @param[in] model  balance contributors list model
	 * @param[in] parent parent widget
	 */
	ContributorsListWidget(BalanceContributorsModel* model,
		QWidget* parent = 0);

signals:
	/**
	 * Indicates whether a valid contributor is selected that
	 * can be deleted.
	 *
	 * @param able `true` if a deleteable contributor is selected
	 */
	void canDelete(bool able);

public slots:
	/**
	 * Creates a new balance contributor.
	 */
	void createContributor();

	/**
	 * Deletes the currently selected balance contributor.
	 */
	void deleteSelectedContributor();

private slots:
	/**
	 * Emits the contributorSelected signal according to
	 * the current contributor selection.
	 *
	 * @param[in] current  currently selected contributor
	 * @param[in] previous previously selected contributor
	 */
	void selectionChanged(const QModelIndex& current, const QModelIndex& previous);

protected:
	/**
	 * Displays a context menu for operating on the contributors list.
	 *
	 * @param[in] event context menu event
	 */
	void contextMenuEvent(QContextMenuEvent* event);

private:
	/** Balance contributors list model */
	BalanceContributorsModel* model;
};

}

#endif //CONTRIBUTORSLISTWIDGET_HPP
