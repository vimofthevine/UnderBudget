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

#ifndef IMPORTEDTRANSACTIONSLISTWIDGET_HPP
#define IMPORTEDTRANSACTIONSLISTWIDGET_HPP

// Qt include(s)
#include <QTableView>

// Forward declaration(s)
class QSortFilterProxyModel;

namespace ub {

// Forward declaration(s)
class ImportedTransactionsModel;

/**
 * Customized QTableView for displaying imported transactions.
 */
class ImportedTransactionsListWidget : public QTableView
{
	Q_OBJECT

public:
	/**
	 * Constructs a new imported transactions list widget.
	 *
	 * @param[in] model  imported transactions list model
	 * @param[in] parent parent widget
	 */
	ImportedTransactionsListWidget(ImportedTransactionsModel* model,
		QWidget* parent = 0);

signals:
	/**
	 * Emitted when a transaction is selected to indicate the current
	 * associated estimate.
	 *
	 * @param estimateId ID of the estimate associated to the currently
	 *                   selected transaction
	 */
	void estimateSelected(uint estimateId);

	/**
	 * Emitted when a transaction is selected to indicate the current
	 * associated assignment rule.
	 *
	 * @param ruleId ID of the assignment rule associated to the currently
	 *               selected transaction
	 */
	void ruleSelected(uint ruleId);

public slots:
	/**
	 * Filters the list of displayed transactions to only those
	 * matching the given estimate ID.
	 *
	 * @param[in] estimateId ID of the estimate on which to filter
	 */
	void filterByEstimate(uint estimateId);

	/**
	 * Filters the list of displayed transactions to only those
	 * matching the given assignment rule ID.
	 *
	 * @param[in] ruleId ID of the assignment rule on which to filter
	 */
	void filterByRule(uint ruleId);

	/**
	 * Show the normal columns/fields.
	 *
	 * This includes the transaction's
	 *  - date
	 *  - payee
	 *  - memo
	 *  - amount
	 *  - whether the transaction has been assigned
	 */
	void showStandardColumns();

	/**
	 * Show the detailed columns/fields.
	 *
	 * This includes the transaction's
	 *  - date
	 *  - payee
	 *  - memo
	 *  - amount
	 *  - withdrawal account
	 *  - deposit account
	 *  - whether the transaction has been assigned
	 *  - assigned estimate's name
	 */
	void showDetailedColumns();

	/**
	 * Show the minimal columns/fields.
	 *
	 * This includes the transaction's
	 *  - date
	 *  - payee
	 *  - memo
	 *  - amount
	 */
	void showMinimalColumns();

private slots:
	/**
	 * Responds to transaction selections.
	 *
	 * @param[in] current  currently selected transaction index
	 * @param[in] previous previously selected transaction index
	 */
	void selectionChanged(const QModelIndex& current, const QModelIndex& previous);

protected:
	/**
	 * Displays a context menu for operating on the transactions list.
	 *
	 * @param[in] event context menu event
	 */
	void contextMenuEvent(QContextMenuEvent* event);

private:
	/** Imported transactions list model */
	ImportedTransactionsModel* model;
	/** Filter proxy model */
	QSortFilterProxyModel* filter;
	/** Tracks whether list is currently filtered */
	bool filtered;
};

}

#endif //IMPORTEDTRANSACTIONSLISTWIDGET_HPP
