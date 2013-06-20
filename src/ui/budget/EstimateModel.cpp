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
#include "ui/budget/EstimateModel.hpp"
#include "ui/budget/ProxyModelAddCommand.hpp"
#include "ui/budget/ProxyModelChangeCommand.hpp"
#include "ui/budget/ProxyModelDeleteCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
EstimateModel::EstimateModel(QSharedPointer<Estimate> root, QUndoStack* stack,
		QObject* parent)
	: QAbstractItemModel(parent), root(root), undoStack(stack)
{
	headers << tr("Name")
		// Progress columns
		<< tr("Progress")
		<< tr("Estimated") << tr("Actual") << tr("Difference") << tr("Notice")
		// Impact colums
		<< tr("Estimated") << tr("Actual") << tr("Expected") << tr("Notice");
}

//------------------------------------------------------------------------------
Estimate* EstimateModel::cast(const QModelIndex& index) const
{
	return static_cast<Estimate*>(index.internalPointer());
}

//------------------------------------------------------------------------------
int EstimateModel::columnCount(const QModelIndex& parent) const
{
	return headers.size();
}

//------------------------------------------------------------------------------
QVariant EstimateModel::headerData(int section, Qt::Orientation orientation,
	int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return headers.at(section);
	return QVariant();
}

//------------------------------------------------------------------------------
int EstimateModel::rowCount(const QModelIndex& parent) const
{
	if (parent.column() > 0)
		return 0;

	if ( ! parent.isValid())
		return root->childCount();
	else
		return cast(parent)->childCount();
}

//------------------------------------------------------------------------------
QVariant EstimateModel::data(const QModelIndex& index, int role) const
{
	if ( ! index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	Estimate* estimate = cast(index);
	int column = index.column();
	Estimate::Progress progress = estimate->progress(actuals);
	Estimate::Impact impact = estimate->impact(actuals);

	switch (column)
	{
	case 0: // name
		return estimate->estimateName();
	case 1: // progress
		return (progress.actual / progress.estimated);
	case 2: // progress estimated
		return progress.estimated.toString();
	case 3: // progress actual
		return progress.actual.toString();
	case 4: // progress difference
		return (progress.estimated - progress.actual).toString();
	case 5: // progress notice
		return progress.note;
	case 6: // impact estimated
		return impact.estimated.toString();
	case 7: // impact actual
		return impact.actual.toString();
	case 8: // impact expected
		return impact.expected.toString();
	case 9: // impact notice
		return impact.note;
	}
}

//------------------------------------------------------------------------------
QModelIndex EstimateModel::index(int row, int column, const QModelIndex& parent) const
{
	if ( ! hasIndex(row, column, parent))
		return QModelIndex();

	Estimate* parentEstimate = ! parent.isValid() ? root.data() : cast(parent);
	Estimate* child = parentEstimate->childAt(row);
	if (child)
		return createIndex(row, column, child);
	else
		return QModelIndex();
}

//------------------------------------------------------------------------------
QModelIndex EstimateModel::parent(const QModelIndex& index) const
{
	if ( ! index.isValid())
		return QModelIndex();

	Estimate* child = cast(index);
	Estimate* parent = child->parentEstimate();

	// If root
	if ( ! parent->parentEstimate())
		return QModelIndex();
	return createIndex(parent->parentEstimate()->indexOf(parent), 0, parent);
}

//------------------------------------------------------------------------------
void EstimateModel::updateName(const QModelIndex& index, const QString& name)
{
	if (index.isValid())
	{
		undoStack->push(new ProxyModelChangeCommand(this, index,
			cast(index)->changeName(name)));
	}
}

//------------------------------------------------------------------------------
void EstimateModel::updateDescription(const QModelIndex& index, const QString& descrip)
{
	if (index.isValid())
	{
		undoStack->push(new ProxyModelChangeCommand(this, index,
			cast(index)->changeDescription(descrip)));
	}
}

//------------------------------------------------------------------------------
void EstimateModel::updateType(const QModelIndex& index, Estimate::Type type)
{
	if (index.isValid())
	{
		// Use invalid index so that entire tree is redrawn and the details
		// widget is re-populated because the type change is cascading--we
		// don't know for sure how many estimates are affected
		undoStack->push(new ProxyModelChangeCommand(this, QModelIndex(),
			cast(index)->changeType(type)));
	}
}

//------------------------------------------------------------------------------
void EstimateModel::updateAmount(const QModelIndex& index, const Money& amount)
{
	if (index.isValid())
	{
		undoStack->push(new ProxyModelChangeCommand(this, index,
			cast(index)->changeAmount(amount)));
	}
}

//------------------------------------------------------------------------------
void EstimateModel::updateDueDate(const QModelIndex& index, const QDate& date)
{
	if (index.isValid())
	{
		undoStack->push(new ProxyModelChangeCommand(this, index,
			cast(index)->changeDueDate(date)));
	}
}

//------------------------------------------------------------------------------
void EstimateModel::updateFinishedState(const QModelIndex& index, bool finished)
{
	if (index.isValid())
	{
		undoStack->push(new ProxyModelChangeCommand(this, index,
			cast(index)->changeFinishedState(finished)));
	}
}

//------------------------------------------------------------------------------
void EstimateModel::deleteEstimate(const QModelIndex& index)
{
	if (index.isValid())
	{
		undoStack->push(new ProxyModelDeleteCommand(this, index,
			cast(index)->deleteEstimate()));
	}
}

//------------------------------------------------------------------------------
void EstimateModel::addChild(const QModelIndex& index)
{
	if (index.isValid())
	{
		undoStack->push(new ProxyModelAddCommand(this, index,
			cast(index)->addChild()));
	}
}

//------------------------------------------------------------------------------
void EstimateModel::emitDataChanged(const QModelIndex& changed)
{
	// Emit signal for the entire row
	emit dataChanged(
		index(changed.row(), 0, changed.parent()),
		index(changed.row(), columnCount(changed)-1, changed.parent())
	);
}

//------------------------------------------------------------------------------
void EstimateModel::emitDataChanged(const QModelIndex& topLeft,
	const QModelIndex& bottomRight)
{
	emit dataChanged(topLeft, bottomRight);
}

}

