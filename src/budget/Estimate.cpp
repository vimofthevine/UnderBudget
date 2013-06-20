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
#include <QtCore>

// UnderBudget include(s)
#include "budget/AddChildEstimateCommand.hpp"
#include "budget/ChangeEstimateAmountCommand.hpp"
#include "budget/ChangeEstimateDescriptionCommand.hpp"
#include "budget/ChangeEstimateDueDateCommand.hpp"
#include "budget/ChangeEstimateFinishedCommand.hpp"
#include "budget/ChangeEstimateNameCommand.hpp"
#include "budget/ChangeEstimateTypeCommand.hpp"
#include "budget/DeleteEstimateCommand.hpp"
#include "budget/Estimate.hpp"
#include "budget/MoveEstimateCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
QSharedPointer<Estimate> Estimate::createRoot()
{
	return QSharedPointer<Estimate>(new Estimate);
}

//------------------------------------------------------------------------------
Estimate::Estimate()
	: paths(new QHash<uint, QList<int> >),
	  parent(0), id(0), name(tr("Root")), type(Root), finished(false)
{ }

//------------------------------------------------------------------------------
Estimate::~Estimate()
{
	qDeleteAll(children);
}

//------------------------------------------------------------------------------
Estimate* Estimate::create(Estimate* parent,
	uint id, const QString& name, const QString& description, Type type,
	const Money& amount, const QDate& dueDate, bool finished, int index)
{
	return new Estimate(parent, id, name, description, type,
		amount, dueDate, finished, index);
}

//------------------------------------------------------------------------------
Estimate::Estimate(Estimate* parent, uint id,
		const QString& name, const QString& description, Type type,
		const Money& amount, const QDate& dueDate, bool finished, int index)
	: parent(parent), id(id), name(name), description(description),
	  type(type), amount(amount), dueDate(dueDate), finished(finished)
{
	// Make parent category-compatible (before making it a category)
	parent->setAmount(Money());
	parent->setDueDate(QDate());
	parent->setFinishedState(false);

	// Add self to parent's children
	parent->addChild(this, index);

	// Grab estimate path map
	paths = parent->paths;
	// Add self to estimate pointer map
	paths->insert(id, path());
}

//------------------------------------------------------------------------------
Estimate::Estimate(const Estimate& orig)
	: paths(orig.paths), parent(orig.parent), id(orig.id),
	  name(orig.name), description(orig.description), type(orig.type),
	  amount(orig.amount), dueDate(orig.dueDate), finished(orig.finished)
{ }

//------------------------------------------------------------------------------
void Estimate::addChild(Estimate* child, int index)
{
	if (index >= 0 && index <= children.size())
	{
		children.insert(index, child);
	}
	else
	{
		index = children.size();
		children.append(child);
	}

	emit childAdded(child, index);
}

//------------------------------------------------------------------------------
void Estimate::removeChild(Estimate* child, bool del)
{
	int index = indexOf(child);
	if (index != -1)
	{
		children.removeOne(child);
		emit childRemoved(child, index);
	}

	if (del)
	{
		delete child;
	}
}

//------------------------------------------------------------------------------
void Estimate::moveChild(Estimate* child, int newIndex)
{
	// Make sure new index is within the bounds of the child list
	if (newIndex < 0)
		newIndex = 0;
	if (newIndex >= children.size())
		newIndex = children.size() - 1;

	int oldIndex = indexOf(child);
	if (oldIndex != -1)
	{
		children.move(oldIndex, newIndex);

		// Re-determine all paths for children under this estimate
		for (int i=0; i<children.size(); ++i)
		{
			Estimate* c = children.at(i);
			paths->insert(c->estimateId(), c->path());
		}

		emit childMoved(child, oldIndex, newIndex);
	}
}

//------------------------------------------------------------------------------
Estimate* Estimate::root() const
{
	if (isRoot())
		return const_cast<Estimate*>(this);
	else
		return parent->root();
}

//------------------------------------------------------------------------------
Estimate* Estimate::fromPath(const QList<int>& path) const
{
	if (path.size() == 0)
		return 0;

	Estimate* estimate = root();
	int index;

	foreach(index, path)
	{
		if ( ! estimate)
		{
			return 0;
		}
		else
		{
			estimate = estimate->childAt(index);
		}
	}

	return estimate;
}

//------------------------------------------------------------------------------
QList<int> Estimate::path() const
{
	QList<int> path;
	Estimate* estimate = const_cast<Estimate*>(this);

	// Work our way up the tree until we hit root
	while ( ! estimate->isRoot())
	{
		path.insert(0, estimate->parentEstimate()->indexOf(estimate));
		estimate = estimate->parentEstimate();
	}

	return path;
}

//------------------------------------------------------------------------------
bool Estimate::isRoot() const
{
	return (type == Root) && ( ! parent);
}

//------------------------------------------------------------------------------
bool Estimate::isCategory() const
{
	return (children.size() > 0);
}

//------------------------------------------------------------------------------
QUndoCommand* Estimate::changeName(const QString& newName,
		QUndoCommand* cmd)
{
	return new ChangeEstimateNameCommand(root(), id, name, newName, cmd);
}

//------------------------------------------------------------------------------
void Estimate::setName(const QString& newName)
{
	// Root's name cannot change
	if (isRoot())
		return;

	if (name != newName)
	{
		name = newName;
		emit nameChanged(name);
	}
}

//------------------------------------------------------------------------------
QUndoCommand* Estimate::changeDescription(const QString& newDescrip,
		QUndoCommand* cmd)
{
	return new ChangeEstimateDescriptionCommand(root(), id,
		description, newDescrip, cmd);
}

//------------------------------------------------------------------------------
void Estimate::setDescription(const QString& newDescrip)
{
	// Root's description cannot change
	if (isRoot())
		return;

	if (description != newDescrip)
	{
		description = newDescrip;
		emit descriptionChanged(description);
	}
}

//------------------------------------------------------------------------------
QUndoCommand* Estimate::changeType(Type newType, QUndoCommand* cmd)
{
	if (cmd == 0)
	{
		cmd = new QUndoCommand;
	}

	new ChangeEstimateTypeCommand(root(), id, type, newType, cmd);

	// Propagate type change down estimate sub-tree
	for (int i=0; i<children.size(); ++i)
	{
		children.at(i)->changeType(newType, cmd);
	}

	return cmd;
}

//------------------------------------------------------------------------------
void Estimate::setType(Type newType)
{
	// Root cannot have any other type than Root
	if (isRoot())
		return;

	if (type != newType)
	{
		type = newType;
		emit typeChanged(type);
	}
}

//------------------------------------------------------------------------------
QUndoCommand* Estimate::changeAmount(const Money& newAmt, QUndoCommand* cmd)
{
	return new ChangeEstimateAmountCommand(root(), id, amount, newAmt, cmd);
}

//------------------------------------------------------------------------------
void Estimate::setAmount(const Money& newAmt)
{
	// Root and categories cannot have estimated amounts
	if (isRoot() || isCategory())
		return;

	if (amount != newAmt)
	{
		amount = newAmt;
		emit amountChanged(amount);
	}
}

//------------------------------------------------------------------------------
QUndoCommand* Estimate::changeDueDate(const QDate& newDate, QUndoCommand* cmd)
{
	return new ChangeEstimateDueDateCommand(root(), id, dueDate, newDate, cmd);
}

//------------------------------------------------------------------------------
void Estimate::setDueDate(const QDate& newDate)
{
	// Root and categories cannot have due dates
	if (isRoot() || isCategory())
		return;

	if (dueDate != newDate)
	{
		dueDate = newDate;
		emit dueDateChanged(dueDate);
	}
}

//------------------------------------------------------------------------------
QUndoCommand* Estimate::changeFinishedState(bool newState, QUndoCommand* cmd)
{
	if (cmd == 0)
	{
		cmd = new QUndoCommand;
	}

	new ChangeEstimateFinishedCommand(root(), id, finished, newState, cmd);

	// Propagate finished state change down estimate sub-tree
	for (int i=0; i<children.size(); ++i)
	{
		children.at(i)->changeFinishedState(newState, cmd);
	}

	return cmd;
}

//------------------------------------------------------------------------------
void Estimate::setFinishedState(bool newState)
{
	// Root and categories cannot be finished
	if (isRoot() || isCategory())
		return;

	if (finished != newState)
	{
		finished = newState;
		emit finishedStateChanged(finished);
	}
}

//------------------------------------------------------------------------------
QUndoCommand* Estimate::addChild(QUndoCommand* cmd)
{
	return new AddChildEstimateCommand(root(), id, cmd);
}

//------------------------------------------------------------------------------
QUndoCommand* Estimate::deleteEstimate(QUndoCommand* cmd)
{
	if (cmd == 0)
	{
		cmd = new QUndoCommand;
	}

	// Propagate delete down estimate sub-tree, starting with children
	for (int i=0; i<children.size(); ++i)
	{
		children.at(i)->deleteEstimate(cmd);
	}

	// Delete parent after children
	new DeleteEstimateCommand(root(), id, cmd);

	return cmd;
}

//------------------------------------------------------------------------------
uint Estimate::createChild()
{
	uint id = QDateTime::currentDateTime().toTime_t();
	createChild(id, name, description,
		(type == Root) ? Expense : type,
		amount, dueDate, finished);
	return id;
}

//------------------------------------------------------------------------------
void Estimate::createChild(uint id, const QString& name,
	const QString& description, Type type, const Money& amount,
	const QDate& dueDate, bool finished, int index)
{
	new Estimate(this, id, name, description,
		type, amount, dueDate, finished, index);
}

//------------------------------------------------------------------------------
void Estimate::deleteSelf()
{
	// Remove self from estimate paths map
	paths->remove(id);

	emit deleted();

	// Remove self from parent's children and delete the estimate
	parentEstimate()->removeChild(this, true);
}

//------------------------------------------------------------------------------
QUndoCommand* Estimate::moveTo(Estimate* newParent, int newIndex,
	QUndoCommand* cmd)
{
	if (cmd == 0)
	{
		cmd = new QUndoCommand;
	}

	// Make new parent parent-compatible (if it isn't already)
	newParent->changeAmount(Money(), cmd);
	newParent->changeDueDate(QDate(), cmd);
	newParent->changeFinishedState(false, cmd);

	// Change type to that of the new parent (if it differs)
	changeType(newParent->estimateType(), cmd);

	new MoveEstimateCommand(root(), id, newParent->estimateId(),
		newIndex, cmd);

	return cmd;
}

//------------------------------------------------------------------------------
void Estimate::moveTo(uint newParentId, int newIndex)
{
	Estimate* newParent = root()->find(newParentId);
	if ( ! newParent)
		return;

	// Moving from one parent to another
	if (parent != newParent)
	{
		// Remove from old parent
		parentEstimate()->removeChild(this);
		paths->remove(id);

		// Add to new parent
		parent = newParent;
		parentEstimate()->addChild(this, (newIndex < 0) ? 0 : newIndex);
		paths->insert(id, path());
	}
	else
	{
		parentEstimate()->moveChild(this, newIndex);
	}
}

//------------------------------------------------------------------------------
uint Estimate::estimateId() const
{
	return id;
}

//------------------------------------------------------------------------------
QString Estimate::estimateName() const
{
	return name;
}

//------------------------------------------------------------------------------
QString Estimate::estimateDescription() const
{
	return description;
}

//------------------------------------------------------------------------------
Estimate::Type Estimate::estimateType() const
{
	return type;
}

//------------------------------------------------------------------------------
Money Estimate::estimatedAmount() const
{
	return amount;
}

//------------------------------------------------------------------------------
Money Estimate::totalEstimatedAmount() const
{
	Money sum(amount);

	for (int i=0; i<children.size(); ++i)
	{
		sum += children.at(i)->totalEstimatedAmount();
	}

	return sum;
}

//------------------------------------------------------------------------------
Money Estimate::totalActualAmount(const QHash<uint,Money>& actuals) const
{
	if (isCategory())
	{
		Money sum;

		for (int i=0; i<children.size(); ++i)
		{
			sum += children.at(i)->totalActualAmount(actuals);
		}

		return sum;
	}
	else
	{
		return actuals.value(id, Money());
	}
}

//------------------------------------------------------------------------------
QDate Estimate::activityDueDate() const
{
	return dueDate;
}

//------------------------------------------------------------------------------
bool Estimate::isActivityFinished() const
{
	return finished;
}

//------------------------------------------------------------------------------
Estimate* Estimate::parentEstimate() const
{
	return parent;
}

//------------------------------------------------------------------------------
int Estimate::childCount() const
{
	return children.size();
}

//------------------------------------------------------------------------------
Estimate* Estimate::childAt(int index) const
{
	// Perform array bounds checking
	if (index < 0 || index >= children.size())
		return 0;

	return children.at(index);
}

//------------------------------------------------------------------------------
int Estimate::indexOf(Estimate* child) const
{
	return children.indexOf(child);
}

//------------------------------------------------------------------------------
Estimate* Estimate::find(uint estimateId) const
{
	if (estimateId == 0)
		return root();
	return fromPath(paths->value(estimateId, QList<int>()));
}

//------------------------------------------------------------------------------
Estimate::Progress Estimate::progress(const QHash<uint,Money>& actuals) const
{
	Estimate::Progress progress;
	progress.isHealthy = true;

	// Root is a special case, it is never populated
	if ( ! isRoot())
	{
		// Get total/hierarchical values
		progress.estimated = totalEstimatedAmount();
		progress.actual = totalActualAmount(actuals);

		if (progress.actual.isZero() && dueDate.isValid())
		{
			progress.note = tr("Due on %1").arg(dueDate.toString());
		}

		switch (type)
		{
		case Expense:
		case Transfer:
			progress.isHealthy = progress.actual <= progress.estimated;
			break;

		case Income:
			progress.isHealthy = progress.actual >= progress.estimated;
			break;
		}
	}

	return progress;
}

//------------------------------------------------------------------------------
Estimate::Impact Estimate::impact(const QHash<uint,Money>& actuals) const
{
	Estimate::Impact impact;

	// Categories have no impact
	if ( ! isCategory())
	{
		const Money actual = actuals.value(id, Money());

		switch (type)
		{
		case Expense:
			// Negate the values so they are subtracted from the balance
			impact.estimated = -amount;
			impact.actual = -actual;
			break;

		case Income:
			// Keep the values positive so they are added to the balance
			impact.estimated = amount;
			impact.actual = actual;
			break;

		// Transfers have no impact
		}

		if (finished)
		{
			// Use actual if activity is finished
			impact.expected = impact.actual;
		}
		else
		{
			// Use actual if it is greater than the estimated
			impact.expected = (actual > amount)
				? impact.actual : impact.estimated;
		}
	}

	return impact;
}

}

