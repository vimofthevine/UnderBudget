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

// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "ui/budget/BalanceContributorsModel.hpp"
#include "ui/budget/ContributorAddProxyCommand.hpp"
#include "ui/budget/ContributorChangeProxyCommand.hpp"
#include "ui/budget/ContributorRemoveProxyCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
BalanceContributorsModel::BalanceContributorsModel(QSharedPointer<Balance> balance,
		QUndoStack* stack, QObject* parent)
	: QAbstractItemModel(parent), balance(balance),
	  undoStack(stack)
{ }

//------------------------------------------------------------------------------
int BalanceContributorsModel::columnCount(const QModelIndex& parent) const
{
	// name, amount, increase/decrease
	return 3;
}

//------------------------------------------------------------------------------
QVariant BalanceContributorsModel::headerData(int section, Qt::Orientation orientation,
	int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (section)
		{
		case 0:
			return tr("Contributor");
		case 1:
			return tr("Amount");
		case 2:
			return tr("Debit");
		default:
			return QVariant();
		}
	}
	else
		return QVariant();
}

//------------------------------------------------------------------------------
int BalanceContributorsModel::rowCount(const QModelIndex& parent) const
{
	if (parent.column() > 0)
		return 0;
	else
		return balance->contributorCount();
}

//------------------------------------------------------------------------------
QModelIndex BalanceContributorsModel::index(int row, int column,
	const QModelIndex& parent) const
{
	if ( ! hasIndex(row, column, parent))
		return QModelIndex();

	return createIndex(row, column);
}

//------------------------------------------------------------------------------
QModelIndex BalanceContributorsModel::parent(const QModelIndex& index) const
{
	return QModelIndex();
}

//------------------------------------------------------------------------------
QVariant BalanceContributorsModel::data(const QModelIndex& index, int role) const
{
	if ( ! index.isValid())
		return QVariant();
	if (role == Qt::CheckStateRole)
		return checkStateData(index);
	if (role == Qt::DisplayRole)
		return displayData(index);
	if (role == Qt::EditRole)
		return editData(index);
	return QVariant();
}

//------------------------------------------------------------------------------
QVariant BalanceContributorsModel::checkStateData(const QModelIndex& index) const
{
	// Only column 2 (increase/decrease) is checkable
	if (index.column() != 2)
		return QVariant();

	if (index.row() >= 0 && index.row() < balance->contributorCount())
	{
		return balance->contributorAt(index.row()).increase
			? Qt::Checked : Qt::Unchecked;
	}

	return QVariant();
}

//------------------------------------------------------------------------------
QVariant BalanceContributorsModel::displayData(const QModelIndex& index) const
{
	if ( ! index.isValid())
		return QVariant();

	if (index.row() >= 0 && index.row() < balance->contributorCount())
	{
		Balance::Contributor contributor = balance->contributorAt(index.row());
		switch (index.column())
		{
		case 0:
			return contributor.name;
		case 1:
			return contributor.amount.toString();
		// Column 2 (increase/decrease) intentionally omitted
		default:
			return QVariant();
		}
	}

	return QVariant();
}

//------------------------------------------------------------------------------
QVariant BalanceContributorsModel::editData(const QModelIndex& index) const
{
	if ( ! index.isValid())
		return QVariant();

	if (index.row() >= 0 && index.row() < balance->contributorCount())
	{
		Balance::Contributor contributor = balance->contributorAt(index.row());
		switch (index.column())
		{
		case 0:
			return contributor.name;
		case 1:
		{
			QVariant val;
			val.setValue<Money>(contributor.amount);
			return val;
		}
		case 2:
			return contributor.increase;
		default:
			return QVariant();
		}
	}

	return QVariant();
}

//------------------------------------------------------------------------------
Qt::ItemFlags BalanceContributorsModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);
	flags |= Qt::ItemIsEditable;

	// If the increase/decrease column
	if (index.column() == 2)
	{
		flags |= Qt::ItemIsUserCheckable;
	}

	return flags;
}

//------------------------------------------------------------------------------
bool BalanceContributorsModel::setData(const QModelIndex& index,
	const QVariant& value, int role)
{
	if (index.row() >= 0 && index.row() < balance->contributorCount())
	{
		Balance::Contributor contributor = balance->contributorAt(index.row());

		switch (index.column())
		{
		case 0:
			contributor.name = value.toString();
			break;
		case 1:
			contributor.amount = value.value<Money>();
			break;
		case 2:
			contributor.increase = value.toBool();
			break;
		default:
			break;
		}

		undoStack->push(new ContributorChangeProxyCommand(this, index.row(),
			balance->updateContributor(index.row(), contributor)));
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------
void BalanceContributorsModel::emitDataChanged(int row)
{
	// Emit signal for the entire row
	emit dataChanged(
		index(row, 0, QModelIndex()),
		index(row, columnCount(QModelIndex())-1, QModelIndex())
	);
}

//------------------------------------------------------------------------------
void BalanceContributorsModel::add()
{
	undoStack->push(new ContributorAddProxyCommand(this,
		balance->contributorCount(), balance->addContributor()));
}

//------------------------------------------------------------------------------
void BalanceContributorsModel::remove(const QModelIndex& index)
{
	if (index.row() >= 0 && index.row() < balance->contributorCount())
	{
		int row = index.row();
		undoStack->push(new ContributorRemoveProxyCommand(this, row,
			balance->removeContributor(row)));
	}
}

}

