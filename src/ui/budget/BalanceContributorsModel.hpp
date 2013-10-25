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

#ifndef BALANCECONTRIBUTORSMODEL_HPP
#define BALANCECONTRIBUTORSMODEL_HPP

// Qt include(s)
#include <QAbstractItemModel>
#include <QSharedPointer>

// UnderBudget include(s)
#include "budget/Balance.hpp"

// Forward declaration(s)
class QUndoStack;

namespace ub {

/**
 * Balance contributors list model to serve as a proxy between various
 * UI views and the backing balance and contributors.
 */
class BalanceContributorsModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	/**
	 * Constructs a new balance contributors model
	 *
	 * @param[in] balance balance model
	 * @param[in] stack   undo stack
	 * @param[in] parent  parent object
	 */
	BalanceContributorsModel(QSharedPointer<Balance> balance,
		QUndoStack* stack, QObject* parent = 0);

	/**
	 * Creates a new contributor to the balance.
	 */
	void add();

	/**
	 * Removes the contributor located at the specified index.
	 *
	 * @param[in] index index of the contributor to be removed
	 */
	void remove(const QModelIndex& index);

	// Overridden methods

	/**
	 * Reimplemented to return the number of columns/fields used
	 * to represent balance contributors.
	 */
	int columnCount(const QModelIndex& parent = QModelIndex()) const;

	/**
	 * Reimplemented to return a title for the specified column/field.
	 */
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;

	/**
	 * Reimpelmented to return the number of contributors to the balance.
	 */
	int rowCount(const QModelIndex& parent = QModelIndex()) const;

	/**
	 * Reimplemented to return an index to a balance contributor.
	 */
	QModelIndex index(int row, int column,
		const QModelIndex& parent = QModelIndex()) const;

	/**
	 * Reimplemented to return the parent index to a balance contributor.
	 */
	QModelIndex parent(const QModelIndex& index) const;

	/**
	 * Reimplemented to return the appropriate data field for
	 * the balance contributor at the given index.
	 */
	QVariant data(const QModelIndex& index, int role) const;

	/**
	 * Reimplemented to return appropriate editable flags for
	 * items that are editable by the user.
	 */
	Qt::ItemFlags flags(const QModelIndex& index) const;

	/**
	 * Reimplemented to update contributor parameters.
	 */
	bool setData(const QModelIndex& index, const QVariant& value,
		int role = Qt::EditRole);

private:
	/** Balance model */
	QSharedPointer<Balance> balance;
	/** Undo stack for all commands */
	QUndoStack* undoStack;

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
	 * Emits a data changed signal for the given index.
	 *
	 * @param[in] row row index of the changed data
	 */
	void emitDataChanged(int row);

	// Allow command classes access to private data
	friend class ContributorAddProxyCommand;
	friend class ContributorChangeProxyCommand;
	friend class ContributorRemoveProxyCommand;
};

}

#endif //BALANCECONTRIBUTORSMODEL_HPP
