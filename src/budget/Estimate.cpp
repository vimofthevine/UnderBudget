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
	Estimate* root = new Estimate;
	return root->pointer();
}

//------------------------------------------------------------------------------
Estimate::Estimate()
	: estimates(new QHash<uint, QSharedPointer<Estimate> >),
	  parent(-1), id(0), name(tr("Root")), type(Root), finished(false)
{
	// Add self to estimate pointer map
	estimates->insert(id, QSharedPointer<Estimate>(this));
}

//------------------------------------------------------------------------------
QSharedPointer<Estimate> Estimate::create(QSharedPointer<Estimate> parent,
	uint id, const QString& name, const QString& description, Type type,
	const Money& amount, const QDate& dueDate, bool finished, int index)
{
	Estimate* child = new Estimate(parent, id, name, description, type,
		amount, dueDate, finished, index);
	return child->pointer();
}

//------------------------------------------------------------------------------
Estimate::Estimate(QSharedPointer<Estimate> parent, uint id,
		const QString& name, const QString& description, Type type,
		const Money& amount, const QDate& dueDate, bool finished, int index)
	: parent(parent->id), id(id), name(name), description(description),
	  type(type), amount(amount), dueDate(dueDate), finished(finished)
{
	// Make parent category-compatible (before making it a category)
	parent->setAmount(Money());
	parent->setDueDate(QDate());
	parent->setFinishedState(false);

	// Grab estimate pointer map
	estimates = parent->estimates;
	// Add self to estimate pointer map
	estimates->insert(id, QSharedPointer<Estimate>(this));

	// Add self to parent's children
	parent->addChild(pointer(), index);
}

//------------------------------------------------------------------------------
Estimate::Estimate(const Estimate& orig)
	: estimates(orig.estimates), parent(orig.parent), id(orig.id),
	  name(orig.name), description(orig.description), type(orig.type),
	  amount(orig.amount), dueDate(orig.dueDate), finished(orig.finished)
{ }

//------------------------------------------------------------------------------
void Estimate::addChild(QSharedPointer<Estimate> child, int index)
{
	if (index >= 0 && index <= children.size())
	{
		children.insert(index, child->estimateId());
	}
	else
	{
		index = children.size();
		children.append(child->estimateId());
	}

	emit childAdded(child, index);
}

//------------------------------------------------------------------------------
void Estimate::removeChild(QSharedPointer<Estimate> child)
{
	int index = indexOf(child);
	if (index != -1)
	{
		children.removeOne(child->estimateId());
		emit childRemoved(child, index);
	}
}

//------------------------------------------------------------------------------
void Estimate::moveChild(QSharedPointer<Estimate> child, int newIndex)
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
		emit childMoved(child, oldIndex, newIndex);
	}
}

//------------------------------------------------------------------------------
bool Estimate::isRoot() const
{
	return (type == Root) && (parent == -1);
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
	return new ChangeEstimateNameCommand(estimates, id, name, newName, cmd);
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
	return new ChangeEstimateDescriptionCommand(estimates, id,
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

	new ChangeEstimateTypeCommand(estimates, id, type, newType, cmd);

	// Propagate type change down estimate sub-tree
	for (int i=0; i<children.size(); ++i)
	{
		uint child = children.at(i);
		if (estimates->contains(child))
		{
			estimates->value(child)->changeType(newType, cmd);
		}
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
	return new ChangeEstimateAmountCommand(estimates, id, amount, newAmt, cmd);
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
	return new ChangeEstimateDueDateCommand(estimates, id, dueDate, newDate, cmd);
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

	new ChangeEstimateFinishedCommand(estimates, id, finished, newState, cmd);

	// Propagate finished state change down estimate sub-tree
	for (int i=0; i<children.size(); ++i)
	{
		uint child = children.at(i);
		if (estimates->contains(child))
		{
			estimates->value(child)->changeFinishedState(newState, cmd);
		}
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
		emit finishedChanged(finished);
	}
}

//------------------------------------------------------------------------------
QUndoCommand* Estimate::addChild(QUndoCommand* cmd)
{
	return new AddChildEstimateCommand(estimates, id, cmd);
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
		uint child = children.at(i);
		if (estimates->contains(child))
		{
			estimates->value(child)->deleteEstimate(cmd);
		}
	}

	// Delete parent after children
	new DeleteEstimateCommand(estimates, id, cmd);

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
	new Estimate(pointer(), id, name, description,
		type, amount, dueDate, finished, index);
}

//------------------------------------------------------------------------------
void Estimate::deleteSelf()
{
	// Remove self from parent's children
	parentEstimate()->removeChild(pointer());

	// Remove self from estimate pointer map
	estimates->remove(id);

	emit deleted();
}

//------------------------------------------------------------------------------
QUndoCommand* Estimate::moveTo(QSharedPointer<Estimate> newParent, int newIndex,
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

	new MoveEstimateCommand(estimates, id, newParent, newIndex, cmd);

	return cmd;
}

//------------------------------------------------------------------------------
void Estimate::moveTo(uint newParentId, int newIndex)
{
	// Moving from one parent to another
	if (parent != newParentId)
	{
		parentEstimate()->removeChild(pointer());
		parent = newParentId;
		parentEstimate()->addChild(pointer(), (newIndex < 0) ? 0 : newIndex);
	}
	else
	{
		parentEstimate()->moveChild(pointer(), newIndex);
	}
}

//------------------------------------------------------------------------------
QSharedPointer<Estimate> Estimate::operator&() const
{
	return pointer();
}

//------------------------------------------------------------------------------
QSharedPointer<Estimate> Estimate::pointer() const
{
	return estimates->value(id);
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
		uint child = children.at(i);
		if (estimates->contains(child))
		{
			sum += estimates->value(child)->totalEstimatedAmount();
		}
	}

	return sum;
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
QSharedPointer<Estimate> Estimate::parentEstimate() const
{
	return estimates->value(parent, QSharedPointer<Estimate>());
}

//------------------------------------------------------------------------------
int Estimate::childCount() const
{
	return children.size();
}

//------------------------------------------------------------------------------
QSharedPointer<Estimate> Estimate::childAt(int index) const
{
	// Perform array bounds checking
	if (index < 0 || index >= children.size())
		return QSharedPointer<Estimate>();

	return estimates->value(children.at(index), QSharedPointer<Estimate>());
}

//------------------------------------------------------------------------------
int Estimate::indexOf(QSharedPointer<Estimate> child) const
{
	return children.indexOf(child->id);
}

//------------------------------------------------------------------------------
QSharedPointer<Estimate> Estimate::find(uint estimateId) const
{
	return estimates->value(estimateId, QSharedPointer<Estimate>());
}

}

