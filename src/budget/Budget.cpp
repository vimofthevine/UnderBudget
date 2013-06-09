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
#include "budget/Budget.hpp"
#include "budget/ChangeBudgetNameCommand.hpp"
#include "budget/ChangeInitialBalanceCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
Budget::Budget()
	: budgetName(tr("New Budget")), period(new BudgetingPeriod)
{ }

//------------------------------------------------------------------------------
QString Budget::name() const
{
	return budgetName;
}

//------------------------------------------------------------------------------
Money Budget::initialBalance() const
{
	return initial;
}

//------------------------------------------------------------------------------
QSharedPointer<BudgetingPeriod> Budget::budgetingPeriod() const
{
	return period;
}

//------------------------------------------------------------------------------
QUndoCommand* Budget::changeName(const QString& newName, QUndoCommand* parent)
{
	return new ChangeBudgetNameCommand(this, budgetName, newName, parent);
}

//------------------------------------------------------------------------------
QUndoCommand* Budget::changeInitialBalance(const Money& newBalance,
	QUndoCommand* parent)
{
	return new ChangeInitialBalanceCommand(this, initial, newBalance, parent);
}

//------------------------------------------------------------------------------
void Budget::setName(const QString& name)
{
	budgetName = name;
	emit nameChanged(budgetName);
}

//------------------------------------------------------------------------------
void Budget::setInitialBalance(const Money& balance)
{
	initial = balance;
	emit initialBalanceChanged(initial);
}

}

