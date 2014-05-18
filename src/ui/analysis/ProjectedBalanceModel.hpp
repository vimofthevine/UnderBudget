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

#ifndef PROJECTEDBALANCEMODEL_HPP
#define PROJECTEDBALANCEMODEL_HPP

// Qt include(s)
#include <QAbstractTableModel>

namespace ub {

// Forward declaration(s)
class ProjectedBalance;

/**
 * Projected balance model.
 *
 * @ingroup ui_analysis
 */
class ProjectedBalanceModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	/**
	 * Constructs a new projected balance model.
	 *
	 * @param[in] estimated projected estimated balance
	 * @param[in] actual    projected actual balance
	 * @param[in] expected  projected expected balance
	 * @param[in] parent    parent object
	 */
	ProjectedBalanceModel(ProjectedBalance* estimated,
		ProjectedBalance* actual, ProjectedBalance* expected,
		QObject* parent = 0);

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
	 * new balance data.
	 */
	void balancesChanged();

private:
	/** Projected estimated balance */
	ProjectedBalance* estimatedBalance;
	/** Projected actual balance */
	ProjectedBalance* actualBalance;
	/** Projected expected balance */
	ProjectedBalance* expectedBalance;
};

}

#endif //PROJECTEDBALANCEMODEL_HPP
