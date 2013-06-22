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
#include "ui/budget/ProxyModelMoveCommand.hpp"

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
Qt::ItemFlags EstimateModel::flags(const QModelIndex& index) const
{
	// Make sure drag and drop are enabled
	Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
	defaultFlags = Qt::ItemIsDropEnabled | defaultFlags;

	if (index.isValid() && index.column() == 0)
		return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
	else
		return defaultFlags;
}

//------------------------------------------------------------------------------
Qt::DropActions EstimateModel::supportedDragActions() const
{
	return Qt::MoveAction;
}

//------------------------------------------------------------------------------
Qt::DropActions EstimateModel::supportedDropActions() const
{
	return Qt::MoveAction;
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
QModelIndex EstimateModel::index(uint estimateId) const
{
	Estimate* estimate = root->find(estimateId);
	// If no estimate found
	if ( ! estimate)
	{
		return QModelIndex();
	}
	// If the estimate is root
	else if ( ! estimate->parentEstimate())
	{
		return QModelIndex();
	}
	else
	{
		// Work our way up the estimate tree
		Estimate* parent = estimate->parentEstimate();
		return index(parent->indexOf(estimate), 0, index(parent->estimateId()));
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
		Estimate* estimate = cast(index);
		Estimate* parent = estimate->parentEstimate();
		undoStack->push(new ProxyModelDeleteCommand(this,
			(parent ? parent->estimateId() : 0),
			estimate->estimateId(),
			index.row(),
			estimate->deleteEstimate()));
	}
}

//------------------------------------------------------------------------------
void EstimateModel::addChild(const QModelIndex& index)
{
	if (index.isValid())
	{
		Estimate* estimate = cast(index);
		undoStack->push(new ProxyModelAddCommand(this,
			estimate->estimateId(),
			estimate->addChild()));
	}
	else
	{
		undoStack->push(new ProxyModelAddCommand(this,
			root->estimateId(), root->addChild()));
	}
}

//------------------------------------------------------------------------------
QStringList EstimateModel::mimeTypes() const
{
	QStringList types;
	types << "application/estimate.origin";
	return types;
}

//------------------------------------------------------------------------------
QMimeData* EstimateModel::mimeData(const QModelIndexList& indices) const
{
	// Serialize original estimate location data
	QMimeData* mimeData = new QMimeData;
	QByteArray encoded;
	QDataStream stream(&encoded, QIODevice::WriteOnly);
	QModelIndex index = indices.at(0);
	if (index.isValid())
	{
		stream << cast(index)->estimateId();
		stream << cast(index)->parentEstimate()->estimateId();
		stream << index.row();
	}

	mimeData->setData("application/estimate.origin", encoded);
	return mimeData;
}

//------------------------------------------------------------------------------
bool EstimateModel::dropMimeData(const QMimeData* data, Qt::DropAction action,
		int row, int column, const QModelIndex& parent)
{
	if (action == Qt::IgnoreAction)
		return true;

	if (action != Qt::MoveAction
		|| ! data->hasFormat("application/estimate.origin"))
	{
		return false;
	}

	// Unserialize estimate's original location
	QByteArray encoded = data->data("application/estimate.origin");
	QDataStream stream(&encoded, QIODevice::ReadOnly);
	uint childId, oldParentId;
	int oldRow;
	stream >> childId;
	stream >> oldParentId;
	stream >> oldRow;

	// If new parent index is not valid, assume moving to root
	uint newParentId = parent.isValid() ? cast(parent)->estimateId() : 0;
	// Make sure new row is not < 0 (use rowCount if < 0)
	int newRow = (row < 0) ? rowCount(parent) : row;

	// Grab the estimates
	Estimate* child = root->find(childId);
	Estimate* newParent = root->find(newParentId);

	if (child && newParent)
	{
		undoStack->push(new ProxyModelMoveCommand(this,
			oldParentId, oldRow, newParentId, newRow,
			child->moveTo(newParent, newRow)));
	}
	else
	{
		qDebug() << "invalid child or parent";
		qDebug() << "mime data:" << childId << oldParentId << oldRow;
		qDebug() << "drop data:" << newParentId << newRow;
		qDebug() << "child" << child;
		qDebug() << "new parent" << newParent;
	}

	return true;
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

