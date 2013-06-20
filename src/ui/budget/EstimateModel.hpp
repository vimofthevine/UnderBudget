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

#ifndef ESTIMATEMODEL_HPP
#define ESTIMATEMODEL_HPP

// Qt include(s)
#include <QAbstractItemModel>

// UnderBudget include(s)
#include "budget/Estimate.hpp"

// Forward declaration(s)

namespace ub {

/**
 * Estimate tree model to serve as a proxy between various UI
 * views and the backing estimate tree structure.
 */
class EstimateModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	/**
	 * Constructs a new estimate tree model.
	 *
	 * @param[in] root   root of the estimate tree
	 * @param[in] stack  undo stack
	 * @param[in] parent parent object
	 */
	EstimateModel(QSharedPointer<Estimate> root, QUndoStack* stack,
		QObject* parent = 0);

	// Overridden methods

	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	int columnCount(const QModelIndex& parent = QModelIndex()) const;

	QVariant data(const QModelIndex& index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;

	QModelIndex index(int row, int column,
		const QModelIndex& parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex& index) const;

	/**
	 * Emits a data changed signal for the given index.
	 *
	 * @param[in] index index of the data changed
	 */
	void emitDataChanged(const QModelIndex& index);

	/**
	 * Emits a data changed signal for the given index range.
	 *
	 * @param[in] topLeft     top left index of the changed data
	 * @param[in] bottomRight bottom right index of the changed data
	 */
	void emitDataChanged(const QModelIndex& topLeft,
		const QModelIndex& bottomRight);

public slots:
	/**
	 * Updates the specified estimate's name.
	 *
	 * @param[in] index index of the estimate to be updated
	 * @param[in] name  new estimate name
	 */
	void updateName(const QModelIndex& index, const QString& name);

	/**
	 * Updates the specified estimate's description.
	 *
	 * @param[in] index   index of the estimate to be updated
	 * @param[in] descrip new estimate description
	 */
	void updateDescription(const QModelIndex& index, const QString& desrip);

	/**
	 * Updates the specified estimate's type.
	 *
	 * @param[in] index index of the estimate to be updated
	 * @param[in] type  new estimate type
	 */
	void updateType(const QModelIndex& index, Estimate::Type type);

	/**
	 * Updates the specified estimate's estimated amount.
	 *
	 * @param[in] index  index of the estimate to be updated
	 * @param[in] amount new estimated amount
	 */
	void updateAmount(const QModelIndex& index, const Money& amount);

	/**
	 * Updates the specified estimate's due date.
	 *
	 * @param[in] index index of the estimate to be updated
	 * @param[in] date  new due date
	 */
	void updateDueDate(const QModelIndex& index, const QDate& date);

	/**
	 * Updates the specified estimate's finished state.
	 *
	 * @param[in] index    index of the estimate to be updated
	 * @param[in] finished new finished state
	 */
	void updateFinishedState(const QModelIndex& index, bool finished);

	/**
	 * Removes the specified estimate from the estimate tree.
	 *
	 * @param[in] index index of the estimate to be deleted
	 */
	void deleteEstimate(const QModelIndex& index);

	/**
	 * Adds a child to the specified estimate.
	 *
	 * @param[in] index index of the estimate to add a child
	 */
	void addChild(const QModelIndex& index);

private:
	/** Root estimate */
	QSharedPointer<Estimate> root;
	/** Undo stack for all commands */
	QUndoStack* undoStack;
	/** Header titles */
	QList<QString> headers;

	/** Activity actuals */
	QHash<uint,Money> actuals;

	/**
	 * Extracts the estimate object referenced by the model index.
	 * This method must only be called when the index is known to be valid.
	 *
	 * @param[in] index model index
	 * @return pointer to estimate referenced by the model index
	 */
	Estimate* cast(const QModelIndex& index) const;

	// Allow command classes access to private data
	friend class ProxyModelAddCommand;
	friend class ProxyModelDeleteCommand;
};

}

#endif //ESTIMATEMODEL_HPP
