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
#include "budget/Estimate.hpp"

namespace ub {

//------------------------------------------------------------------------------
Estimate::Estimate()
	: estimates(new QHash<uint, QSharedPointer<Estimate> >),
	  parent(-1), id(0), name(tr("Root")), type(Root), finished(false)
{
	// Add self to estimate pointer map
	estimates->insert(id, QSharedPointer<Estimate>(this));
}

//------------------------------------------------------------------------------
Estimate::Estimate(QSharedPointer<Estimate> parent, uint id,
		const QString& name, const QString& description, Type type,
		const Money& amount, const QDate& dueDate, bool finished)
	: parent(parent->id), id(id), name(name), description(description),
	  type(type), amount(amount), dueDate(dueDate), finished(finished)
{
	// Make parent category-compatible (before making it a category)
	parent->setAmount(Money());
	parent->setDueDate(QDate());
	parent->setFinishedState(false);

	// Add self to parent's children
	parent->children.append(id);

	// Grab estimate pointer map
	estimates = parent->estimates;
	// Add self to estimate pointer map
	estimates->insert(id, QSharedPointer<Estimate>(this));
}

//------------------------------------------------------------------------------
Estimate::Estimate(const Estimate& orig)
	: estimates(orig.estimates), parent(orig.parent), id(orig.id),
	  name(orig.name), description(orig.description), type(orig.type),
	  amount(orig.amount), dueDate(orig.dueDate), finished(orig.finished)
{ }

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
	return new ChangeEstimateFinishedCommand(estimates, id, finished, newState, cmd);
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
	const QDate& dueDate, bool finished)
{
	// Figure out what the index of the created child will be
	int index = children.size();

	new Estimate(&(*this), id, name, description,
		type, amount, dueDate, finished);

	emit childAdded(index);
}

//------------------------------------------------------------------------------
void Estimate::deleteSelf()
{
	// Remove self from parent's children
	if (estimates->contains(parent))
	{
		estimates->value(parent)->children.removeOne(id);
	}

	// Remove self from estimate pointer map
	estimates->remove(id);

	emit deleted();
}

//------------------------------------------------------------------------------
QSharedPointer<Estimate> Estimate::operator&() const
{
	return estimates->value(id);
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

}

