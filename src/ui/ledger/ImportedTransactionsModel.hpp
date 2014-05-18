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

#ifndef IMPORTEDTRANSACTIONSMODEL_HPP
#define IMPORTEDTRANSACTIONSMODEL_HPP

// Qt include(s)
#include <QAbstractTableModel>
#include <QList>
#include <QSharedPointer>

// UnderBudget include(s)
#include "ledger/ImportedTransaction.hpp"

namespace ub {

// Forward declaration(s)
class Assignments;
class Estimate;

/**
 * Imported transaction list model to serve as a proxy
 * between various UI views and the backing list of
 * imported transactions.
 *
 * This model is a read-only model. Views cannot modify
 * transactions via this model.
 *
 * @ingroup ui_ledger
 */
class ImportedTransactionsModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	/**
	 * Constructs a new imported transactions model.
	 *
	 * @param[in] estimates   root estimate
	 * @param[in] assignments transaction assignments list
	 * @param[in] parent      parent object
	 */
	ImportedTransactionsModel(QSharedPointer<Estimate> estimates,
		Assignments* assignments, QObject* parent = 0);

	// Overridden methods

	/**
	 * Reimplemented to return the number of columns/fields.
	 */
	int columnCount(const QModelIndex& parent = QModelIndex()) const;

	/**
	 * Reimplemented to return a title for the specified column/field.
	 */
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;

	/**
	 * Reimplemented to return the number of imported transactions.
	 */
	int rowCount(const QModelIndex& parent = QModelIndex()) const;

	/**
	 * Reimplemented to return the appropriate data for the specified
	 * column/field at the given index.
	 */
	QVariant data(const QModelIndex& index, int role) const;

	/**
	 * Reimplemented to return appropriate flags.
	 */
	Qt::ItemFlags flags(const QModelIndex& index) const;

public slots:
	/**
	 * Sets the list of imported transactions.
	 */
	void setTransactions(const QList<ImportedTransaction>& trns);

private slots:
	/**
	 * Emits a data-changed signal in response to new assignment info.
	 */
	void assignmentsChanged();

private:
	/** Estimates */
	QSharedPointer<Estimate> estimates;
	/** Assignments list */
	Assignments* assignments;
	/** Imported transactions list */
	QList<ImportedTransaction> transactions;

	/**
	 * Returns check state data for the given index.
	 *
	 * @param[in] index model index
	 * @return check state data for the given index
	 */
	QVariant checkStateData(const QModelIndex& index) const;

	/**
	 * Returns display data for the given index.
	 *
	 * @param[in] index model index
	 * @return display data for the given index
	 */
	QVariant displayData(const QModelIndex& index) const;

	/**
	 * Returns edit data for the given index.
	 *
	 * @param[in] index model index
	 * @return edit data for the given index
	 */
	QVariant editData(const QModelIndex& index) const;

	/**
	 * Returns tooltip data for the given index.
	 *
	 * @param[in] index model index
	 * @return tooltip data for the given index
	 */
	QVariant toolTipData(const QModelIndex& index) const;
};

}

#endif //IMPORTEDTRANSACTIONSMODEL_HPP
