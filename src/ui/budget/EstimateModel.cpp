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
#include "analysis/Actuals.hpp"
#include "budget/BudgetingPeriod.hpp"
#include "ui/budget/AssignmentRulesModel.hpp"
#include "ui/budget/EstimateModel.hpp"
#include "ui/budget/ProxyModelAddCommand.hpp"
#include "ui/budget/ProxyModelChangeCommand.hpp"
#include "ui/budget/ProxyModelDeleteCommand.hpp"
#include "ui/budget/ProxyModelMoveCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
EstimateModel::EstimateModel(QSharedPointer<Estimate> root,
		QSharedPointer<BudgetingPeriod> period, AssignmentRulesModel* rules,
		Actuals* actuals, QUndoStack* stack, QObject* parent)
	: QAbstractItemModel(parent), root(root), period(period), rules(rules),
	  actualsModel(actuals), undoStack(stack)
{
	headers << tr("Name")
		// Definition columns
		<< tr("Description") << tr("Type") << tr("Amount")
		<< tr("Due Date") << tr("Finished") << tr("Rules")
		// Progress columns
		<< tr("Progress") << tr("Estimated") << tr("Actual") << tr("Difference") << tr("Notice")
		// Impact columns
		<< tr("Estimated") << tr("Actual") << tr("Expected") << tr("Notice");

	definitionColumns << 0 << 1 << 2 << 3 << 4 << 5 << 6;
	progressColumns << 0 << 7 << 8 << 9 << 10 << 11;
	impactColumns << 0 << 12 << 13 << 14 << 15;

	connect(actualsModel, SIGNAL(actualsChanged()),
		this, SLOT(cacheActuals()));
	// Make sure we pick up changes to the budgeting period start date
	// (have to use Qt5 style connect because of namespaced type)
	connect(period.data(), &BudgetingPeriod::paramsChanged,
		this, &EstimateModel::startDateChanged);
}

//------------------------------------------------------------------------------
void EstimateModel::cacheActuals()
{
	actuals = actualsModel->map();
	// columns progress "estimated" to impact "notice"
	emit dataChanged(createIndex(0, 7), createIndex(0, 15));
}

//------------------------------------------------------------------------------
void EstimateModel::startDateChanged()
{
	// columns definition "due date" to progress "notice"
	emit dataChanged(createIndex(0, 4), createIndex(0, 11));
}

//------------------------------------------------------------------------------
QList<int> EstimateModel::definitionFieldColumns() const
{
	return definitionColumns;
}

//------------------------------------------------------------------------------
QList<int> EstimateModel::progressFieldColumns() const
{
	return progressColumns;
}

//------------------------------------------------------------------------------
QList<int> EstimateModel::impactFieldColumns() const
{
	return impactColumns;
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
	Estimate::Progress progress
		= estimate->progress(actuals, period->startDate());
	Estimate::Impact impact = estimate->impact(actuals);

	switch (column)
	{
	case 0: // name
		return estimate->estimateName();
	case 1: // defined description
		return estimate->estimateDescription();
	case 2: // defined type
		return toString(estimate->estimateType());
	case 3: // defined amount
		return estimate->estimatedAmount().toString();
	case 4: // defined due date
	{
		int offset = estimate->activityDueDateOffset();
		if (offset == -1)
			return QDate();
		else
			return period->startDate().addDays(offset);
	}
	case 5: // defined finished state
		return estimate->isActivityFinished();
	case 6: // defined rules
		return tr("%1 rules").arg(rules->countFor(estimate->estimateId()));
	case 7: // progress
		return QVariant(progress.isHealthy ? 1 : 0).toByteArray()
			+ QVariant(progress.actual / progress.estimated).toByteArray();
	case 8: // progress estimated
		return progress.estimated.toString();
	case 9: // progress actual
		return progress.actual.toString();
	case 10: // progress difference
		return (progress.estimated - progress.actual).toString();
	case 11: // progress notice
		return progress.note;
	case 12: // impact estimated
		return impact.estimated.toString();
	case 13: // impact actual
		return impact.actual.toString();
	case 14: // impact expected
		return impact.expected.toString();
	case 15: // impact notice
		return impact.note;
	default:
		return QVariant();
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
Estimate* EstimateModel::at(const QModelIndex& index) const
{
	return cast(index);
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
	if ( ! child)
		return QModelIndex();
	Estimate* parent = child->parentEstimate();

	// If root
	if ( ! parent || ! parent->parentEstimate())
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
void EstimateModel::updateDueDateOffset(const QModelIndex& index, int offset)
{
	if (index.isValid())
	{
		undoStack->push(new ProxyModelChangeCommand(this, index,
			cast(index)->changeDueDateOffset(offset)));
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

		// Create estimate-delete command

		// Run both estimate and rule deletion as a single undoable action
		undoStack->beginMacro(tr("Delete %1").arg(estimate->estimateName()));
		QList<uint> estimates;
		populate(estimate, estimates);

		undoStack->push(new ProxyModelDeleteCommand(this,
			(parent ? parent->estimateId() : 0),
			estimate->estimateId(),
			index.row(),
			estimate->deleteEstimate()));
		rules->remove(estimates);

		undoStack->endMacro();
	}
}

//------------------------------------------------------------------------------
void EstimateModel::populate(Estimate* estimate, QList<uint>& estimates)
{
	// Propagate down estimate sub-tree first
	for (int i=(estimate->childCount()-1); i>=0; --i)
	{
		populate(estimate->childAt(i), estimates);
	}

	// Add this estimate
	estimates.append(estimate->estimateId());
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
void EstimateModel::recordExpanded(const QModelIndex& index)
{
	if (index.isValid())
	{
		Estimate* estimate = cast(index);
		qDebug() << "Remembering that estimate" << estimate->estimateId()
			<< "is expanded";
	}
}

//------------------------------------------------------------------------------
void EstimateModel::recordCollapsed(const QModelIndex& index)
{
	if (index.isValid())
	{
		Estimate* estimate = cast(index);
		qDebug() << "Remembering that estimate" << estimate->estimateId()
			<< "is collapsed";
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

	// Only one estimate/index can be moved at a time
	QModelIndex index = indices.at(0);
	if (index.isValid())
	{
		Estimate* estimate = cast(index);
		qDebug() << "Serializing estimate origin mime data:"
			<< "move" << estimate->estimateId()
			<< "from" << estimate->parentEstimate()->estimateId()
			<< "at index" << index.row();
		stream << estimate->estimateId();
		stream << estimate->parentEstimate()->estimateId();
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
	qDebug() << "Unserialized estimate origin mime data:"
		<< "move" << childId << "from" << oldParentId << "at index" << oldRow;

	// If new parent index is not valid, assume moving to root
	uint newParentId = parent.isValid() ? cast(parent)->estimateId() : 0;
	// Make sure new row is not < 0 (use rowCount if < 0)
	int newRow = (row < 0) ? rowCount(parent) : row;
	qDebug() << "Target estimate location:"
		<< "move to" << newParentId << "at index" << newRow;

	// Grab the estimates
	Estimate* child = root->find(childId);
	Estimate* newParent = root->find(newParentId);

	if (newParentId == oldParentId)
	{
		// Don't do anything for invalid or no-op moves
		// (i.e., moving 2 to 3, which is still 2)
		if (oldRow == newRow || (oldRow+1) == newRow)
			return true;
	}

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

