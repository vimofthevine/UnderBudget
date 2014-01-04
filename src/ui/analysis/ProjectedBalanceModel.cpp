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
#include "analysis/ProjectedBalance.hpp"
#include "ui/analysis/ProjectedBalanceModel.hpp"

namespace ub {

//------------------------------------------------------------------------------
static const int NUM_ROWS      = 3;
static const int ESTIMATED_ROW = 0;
static const int ACTUAL_ROW    = 1;
static const int EXPECTED_ROW  = 2;

//------------------------------------------------------------------------------
static const int NUM_COLS      = 6;
static const int NAME_COL      = 0;
static const int INITIAL_COL   = 1;
static const int INCREASE_COL  = 2;
static const int DECREASE_COL  = 3;
static const int NET_COL       = 4;
static const int ENDING_COL    = 5;

//------------------------------------------------------------------------------
ProjectedBalanceModel::ProjectedBalanceModel(ProjectedBalance* estimated,
		ProjectedBalance* actual, ProjectedBalance* expected, QObject* parent)
	: QAbstractTableModel(parent), estimatedBalance(estimated),
	  actualBalance(actual), expectedBalance(expected)
{
	connect(estimated, SIGNAL(balanceChanged()),
		this, SLOT(balancesChanged()));
}

//------------------------------------------------------------------------------
void ProjectedBalanceModel::balancesChanged()
{
	emit dataChanged(createIndex(0, 0), createIndex(NUM_ROWS-1, NUM_COLS-1));
}

//------------------------------------------------------------------------------
int ProjectedBalanceModel::columnCount(const QModelIndex& parent) const
{
	return NUM_COLS;
}

//------------------------------------------------------------------------------
QVariant ProjectedBalanceModel::headerData(int section,
	Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (section)
		{
		case NAME_COL:
			return tr("Balance Type");
		case INITIAL_COL:
			return tr("Initial");
		case INCREASE_COL:
			return tr("Net Increase");
		case DECREASE_COL:
			return tr("Net Decrease");
		case NET_COL:
			return tr("Net Change");
		case ENDING_COL:
			return tr("Projected Balance");
		default:
			return QVariant();
		}
	}
	else
		return QVariant();
}

//------------------------------------------------------------------------------
int ProjectedBalanceModel::rowCount(const QModelIndex& parent) const
{
	return NUM_ROWS;
}

//------------------------------------------------------------------------------
QVariant ProjectedBalanceModel::data(const QModelIndex& index, int role) const
{
	if ( ! index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	int row = index.row();
	if (row < 0 || row >= NUM_ROWS)
		return QVariant();

	ProjectedBalance* balance;
	QString name;

	switch (row)
	{
	case ESTIMATED_ROW:
		name = tr("Estimated");
		balance = estimatedBalance;
		break;

	case ACTUAL_ROW:
		name = tr("Actual");
		balance = actualBalance;
		break;

	case EXPECTED_ROW:
		name = tr("Expected");
		balance = expectedBalance;
		break;

	default:
		balance = 0;
	}

	if (balance == 0)
		return QVariant();

	switch (index.column())
	{
	case NAME_COL:
		return name;
	case INITIAL_COL:
		return balance->initial().toString();
	case INCREASE_COL:
		return balance->netIncrease().toString();
	case DECREASE_COL:
		return balance->netDecrease().toString();
	case NET_COL:
		return balance->netChange().toString();
	case ENDING_COL:
		return balance->value().toString();
	default:
		return QVariant();
	}
}

}

