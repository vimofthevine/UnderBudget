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

// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "analysis/SortedDifferences.hpp"
#include "budget/Estimate.hpp"
#include "ui/analysis/EstimateDiffsModel.hpp"

namespace ub {

//------------------------------------------------------------------------------
static const int NUM_COLS = 2;
static const int NAME_COL = 0;
static const int DIFF_COL = 1;

//------------------------------------------------------------------------------
EstimateDiffsModel::EstimateDiffsModel(QSharedPointer<Estimate> estimates,
		SortedDifferences* diffs, QObject* parent)
	: QAbstractTableModel(parent), estimates(estimates), diffs(diffs)
{
	connect(diffs, SIGNAL(listChanged()),
		this, SLOT(diffsChanged()));
}

//------------------------------------------------------------------------------
void EstimateDiffsModel::diffsChanged()
{
	beginResetModel();
	endResetModel();
}

//------------------------------------------------------------------------------
int EstimateDiffsModel::columnCount(const QModelIndex& parent) const
{
	return NUM_COLS;
}

//------------------------------------------------------------------------------
QVariant EstimateDiffsModel::headerData(int section,
	Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (section)
		{
		case NAME_COL:
			return tr("Estimate");
		case DIFF_COL:
			return tr("Difference");
		default:
			return QVariant();
		}
	}
	else
		return QVariant();
}

//------------------------------------------------------------------------------
int EstimateDiffsModel::rowCount(const QModelIndex& parent) const
{
	if (diffs->size() > 5)
		return 5;
	return diffs->size();
}

//------------------------------------------------------------------------------
QVariant EstimateDiffsModel::data(const QModelIndex& index, int role) const
{
	if ( ! index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	int row = index.row();
	if (row < 0 || row >= diffs->size())
		return QVariant();

	switch (index.column())
	{
	case NAME_COL:
	{
		Estimate* estimate = estimates->find(diffs->estimate(row));
		if (estimate)
			return estimate->estimateName();
		else
			return QVariant();
	}
	case DIFF_COL:
		return diffs->difference(row).toString();
	default:
		return QVariant();
	}
}

}

