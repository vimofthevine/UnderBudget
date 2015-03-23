/*
 * Copyright 2014 Kyle Treubig
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

#ifndef ESTIMATEDIFFSMODEL_HPP
#define ESTIMATEDIFFSMODEL_HPP

// Qt include(s)
#include <QAbstractTableModel>
#include <QSharedPointer>

namespace ub {

// Forward declaration(s)
class Estimate;
class SortedDifferences;

/**
 * Estimate differences model.
 *
 * @ingroup ui_analysis
 */
class EstimateDiffsModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	/**
	 * Constructs a new estimate differences model.
	 *
	 * @param[in] estimates estimate tree
	 * @param[in] diffs     sorted estimate differences
	 * @param[in] parent    parent object
	 */
	EstimateDiffsModel(QSharedPointer<Estimate> estimates,
		SortedDifferences* diffs, QObject* parent = 0);

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
	 * Reimplemented to return the number of rows.
	 */
	int rowCount(const QModelIndex& parent = QModelIndex()) const;

	/**
	 * Reimplemented to return the appropriate data for the specified
	 * column/field at the given index.
	 */
	QVariant data(const QModelIndex& index, int role) const;

private slots:
	/**
	 * Emits a data-changed signal for views to synchronize with the
	 * new differences data.
	 */
	void diffsChanged();

private:
	/** Estimate tree */
	QSharedPointer<Estimate> estimates;
	/** Sorted estimate differences */
	SortedDifferences* diffs;
};

}

#endif //ESTIMATEDIFFSMODEL_HPP
