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

namespace ub {

// Forward declaration(s)
class Actuals;
class AssignmentRulesModel;

/**
 * Estimate tree model to serve as a proxy between various UI
 * views and the backing estimate tree structure.
 *
 * @ingroup ui_budget
 */
class EstimateModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	/**
	 * Constructs a new estimate tree model.
	 *
	 * @param[in] root    root of the estimate tree
	 * @param[in] rules   assignment rules list model
	 * @param[in] actuals actual values
	 * @param[in] stack   undo stack
	 * @param[in] parent  parent object
	 */
	EstimateModel(QSharedPointer<Estimate> root, AssignmentRulesModel* rules,
		Actuals* actuals, QUndoStack* stack, QObject* parent = 0);

	// Overridden methods

	/**
	 * Reimplemented to return the total number of available columns/fields.
	 */
	int columnCount(const QModelIndex& parent = QModelIndex()) const;

	/**
	 * Reimplemented to return a title for the specified column/field.
	 */
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;

	/**
	 * Reimplemented to return the appropriate field for the estimate
	 * at the given index.
	 */
	QVariant data(const QModelIndex& index, int role) const;

	/**
	 * Reimplemented to return the child count for the estimate at the
	 * given index.
	 */
	int rowCount(const QModelIndex& parent = QModelIndex()) const;

	/**
	 * Reimplemented to return an index of a child to the given parent.
	 */
	QModelIndex index(int row, int column,
		const QModelIndex& parent = QModelIndex()) const;

	/**
	 * Reimplemented to return an index of the parent to the given child.
	 */
	QModelIndex parent(const QModelIndex& index) const;

	/**
	 * Reimplemented to return the appropriate flags that will allow:
	 * - moving of an estimate
	 * - editing of user-defined fields
	 */
	Qt::ItemFlags flags(const QModelIndex& index) const;

	/**
	 * Reimplemented to return a move action.
	 */
	Qt::DropActions supportedDragActions() const;

	/**
	 * Reimplemented to return a move action.
	 */
	Qt::DropActions supportedDropActions() const;

	/**
	 * Reimplemented to return the following mime types:
	 * - Moved estimate origin data
	 */
	QStringList mimeTypes() const;

	/**
	 * Reimplemented to create mime data consisting of origin
	 * location data for a moved estimate.
	 */
	QMimeData* mimeData(const QModelIndexList& indices) const;

	/**
	 * Reimplemented to execute a move-estimate operation based on
	 * the origin location data in the given mime data.
	 */
	bool dropMimeData(const QMimeData* data, Qt::DropAction action,
		int row, int column, const QModelIndex& parent);

	// End of overridden methods

	/**
	 * Creates an index to the given estimate, if the estimate
	 * exists in this estimate tree.
	 *
	 * @param[in] estimateId ID of the estimate
	 * @return index to the estimate, if it exists in the tree,
	 *         else an invalid index
	 */
	QModelIndex index(uint estimateId) const;

	/**
	 * Returns the estimate located at the given index.
	 *
	 * @param[in] index model index at which to retrieve the estimate
	 * @return estimate located at the given index
	 */
	Estimate* at(const QModelIndex& index) const;

	/**
	 * Returns a list of columns that display the editable
	 * definition fields of an estimate.
	 *
	 * @return list of definition columns
	 */
	QList<int> definitionFieldColumns() const;

	/**
	 * Returns a list of columns that display the estimate
	 * progress analysis fields of an estimate.
	 *
	 * @return list of progress columns
	 */
	QList<int> progressFieldColumns() const;

	/**
	 * Returns a list of columns that display the balance
	 * impact analysis fields of an estimate.
	 *
	 * @return list of impact columns
	 */
	QList<int> impactFieldColumns() const;

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

private slots:
	/**
	 * Updates the cached actuals.
	 */
	void cacheActuals();

private:
	/** Root estimate */
	QSharedPointer<Estimate> root;
	/** Assignment rules list model */
	AssignmentRulesModel* rules;
	/** Undo stack for all commands */
	QUndoStack* undoStack;

	/** Header titles */
	QList<QString> headers;
	/** Definition columns */
	QList<int> definitionColumns;
	/** Progress columns */
	QList<int> progressColumns;
	/** Impact columns */
	QList<int> impactColumns;

	/** Activity actuals model */
	Actuals* actualsModel;
	/** Cached actuals */
	QHash<uint,Money> actuals;

	/**
	 * Extracts the estimate object referenced by the model index.
	 * This method must only be called when the index is known to be valid.
	 *
	 * @param[in] index model index
	 * @return pointer to estimate referenced by the model index
	 */
	Estimate* cast(const QModelIndex& index) const;

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

	/**
	 * Recursively adds the unique IDs of this estimate and its children
	 * to the given list.
	 *
	 * @param[in] estimate  estimate whose ID is to be added to the list
	 * @param[in] estimates list of unique estimate IDs
	 */
	void populate(Estimate* estimate, QList<uint>& estimates);

	// Allow command classes access to private data
	friend class ProxyModelChangeCommand;
	friend class ProxyModelAddCommand;
	friend class ProxyModelDeleteCommand;
	friend class ProxyModelMoveCommand;
};

}

#endif //ESTIMATEMODEL_HPP
