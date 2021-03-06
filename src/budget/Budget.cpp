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
#include "accounting/Money.hpp"
#include "budget/AssignmentRules.hpp"
#include "budget/Balance.hpp"
#include "budget/Budget.hpp"
#include "budget/ChangeBudgetNameCommand.hpp"
#include "budget/UIPrefs.hpp"

namespace ub {

//------------------------------------------------------------------------------
Budget::Budget()
	: budgetName(tr("New Budget")), period(new BudgetingPeriod),
	  initial(Balance::create())
{
	rootEstimate = Estimate::createRoot();
	Estimate* root = rootEstimate.data();
	Estimate* expenses = Estimate::create(root, 1001,
		"Expenses", "", Estimate::Expense, Money(12.0), -1, false);
	Estimate* incomes = Estimate::create(root, 1002,
		"Incomes", "", Estimate::Income, Money(32.0), -1, true);
	Estimate* salary = Estimate::create(incomes, 2001,
		"Salary", "", Estimate::Income, Money(100.0), -1, false);
	Estimate* utilities = Estimate::create(expenses, 3001,
		"Utilities", "", Estimate::Expense, Money(50.0), -1, true);
	Estimate* rent = Estimate::create(utilities, 4001,
		"Rent", "Apt.", Estimate::Expense, Money(500.0), 27, false);
	Estimate* water = Estimate::create(utilities, 4002,
		"Water", "", Estimate::Expense, Money(25.34), 10, true);
	Estimate* food = Estimate::create(expenses, 3002,
		"Food", "", Estimate::Expense, Money(120.0), -1, false);
	Estimate* loan = Estimate::create(root, 1003,
		"Loan Payment", "", Estimate::Transfer, Money(50.0), 20, false);

	assignmentRules = AssignmentRules::create();
	QList<AssignmentRule::Condition> conditions;
	conditions << AssignmentRule::Condition(AssignmentRule::Payee, AssignmentRule::StringEquals, false, "Vendor");

	assignmentRules->createRule(20010, 2001, conditions);
	assignmentRules->createRule(40010, 4001, conditions);
	conditions << AssignmentRule::Condition(AssignmentRule::DepositAccount, AssignmentRule::EndsWith, true, "Acct");
	assignmentRules->createRule(30020, 3002, conditions);
	conditions.removeAt(0);
	assignmentRules->createRule(10030, 1003, conditions);

	uiPrefs = UIPrefs::create();
}

//------------------------------------------------------------------------------
Budget::Budget(const QString& name, QSharedPointer<BudgetingPeriod> period,
		QSharedPointer<Balance> initial, QSharedPointer<Estimate> root,
		QSharedPointer<AssignmentRules> rules,
		QSharedPointer<UIPrefs> uiPrefs)
	: budgetName(name), period(period), initial(initial), rootEstimate(root),
	  assignmentRules(rules), uiPrefs(uiPrefs)
{ }

//------------------------------------------------------------------------------
QString Budget::name() const
{
	return budgetName;
}

//------------------------------------------------------------------------------
QSharedPointer<Balance> Budget::initialBalance() const
{
	return initial;
}

//------------------------------------------------------------------------------
QSharedPointer<BudgetingPeriod> Budget::budgetingPeriod() const
{
	return period;
}

//------------------------------------------------------------------------------
QSharedPointer<Estimate> Budget::estimates() const
{
	return rootEstimate;
}

//------------------------------------------------------------------------------
QSharedPointer<AssignmentRules> Budget::rules() const
{
	return assignmentRules;
}

//------------------------------------------------------------------------------
QSharedPointer<UIPrefs> Budget::uiPreferences() const
{
	return uiPrefs;
}

//------------------------------------------------------------------------------
QUndoCommand* Budget::changeName(const QString& newName, QUndoCommand* parent)
{
	return new ChangeBudgetNameCommand(this, budgetName, newName, parent);
}

//------------------------------------------------------------------------------
void Budget::setName(const QString& name)
{
	budgetName = name;
	emit nameChanged(budgetName);
}

}

