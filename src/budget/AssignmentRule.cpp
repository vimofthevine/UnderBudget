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
#include "budget/AddConditionCommand.hpp"
#include "budget/AssignmentRule.hpp"
#include "budget/RemoveConditionCommand.hpp"
#include "budget/UpdateConditionCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
AssignmentRule::AssignmentRule(uint ruleId, uint estimateId,
		const QList<Condition>& conditions, AssignmentRules* rules)
	: id(ruleId), eid(estimateId), conditions(conditions), rules(rules)
{ }

//------------------------------------------------------------------------------
QUndoCommand* AssignmentRule::addCondition(QUndoCommand* cmd)
{
	return new AddConditionCommand(rules, id, cmd);
}

//------------------------------------------------------------------------------
QUndoCommand* AssignmentRule::removeCondition(int index, QUndoCommand* cmd)
{
	if (conditions.size() == 0)
		return new QUndoCommand(cmd);
	return new RemoveConditionCommand(rules, id, index, cmd);
}

//------------------------------------------------------------------------------
QUndoCommand* AssignmentRule::updateCondition(int index, const Condition& condition,
	QUndoCommand* cmd)
{
	return new UpdateConditionCommand(rules, id, index, condition, cmd);
}

//------------------------------------------------------------------------------
uint AssignmentRule::ruleId() const
{
	return id;
}

//------------------------------------------------------------------------------
uint AssignmentRule::estimateId() const
{
	return eid;
}

//------------------------------------------------------------------------------
int AssignmentRule::conditionCount() const
{
	return conditions.size();
}

//------------------------------------------------------------------------------
AssignmentRule::Condition AssignmentRule::conditionAt(int index) const
{
	return conditions.value(index, Condition());
}

//------------------------------------------------------------------------------
void AssignmentRule::addCondition(const Condition& condition, int index)
{
	if (index < 0)
		index = 0;
	if (index > conditions.size())
		index = conditions.size();
	conditions.insert(index, condition);
	emit conditionAdded(condition, index);
}

//------------------------------------------------------------------------------
void AssignmentRule::deleteCondition(int index)
{
	if (conditions.size() == 0)
		return;

	if (index < 0)
		index = 0;
	if (index >= conditions.size())
		index = conditions.size() - 1;
	Condition condition = conditions.takeAt(index);
	emit conditionRemoved(condition, index);
}

//------------------------------------------------------------------------------
void AssignmentRule::updateCondition(const Condition& condition, int index)
{
	if (index < 0)
		index = 0;
	if (index >= conditions.size())
		index = conditions.size() - 1;
	conditions.replace(index, condition);
	emit conditionUpdated(condition, index);
}

//------------------------------------------------------------------------------
QString toString(AssignmentRule::Field field)
{
	switch (field)
	{
	case ub::AssignmentRule::Date:
		return QObject::tr("Date");
	case ub::AssignmentRule::Amount:
		return QObject::tr("Amount");
	case ub::AssignmentRule::Payee:
		return QObject::tr("Payee");
	case ub::AssignmentRule::Memo:
		return QObject::tr("Memo");
	case ub::AssignmentRule::DepositAccount:
		return QObject::tr("Deposit Account");
	case ub::AssignmentRule::WithdrawalAccount:
		return QObject::tr("Withdrawal Account");
	default:
		return QObject::tr("Unknown field");
	}
}

//------------------------------------------------------------------------------
QString toString(AssignmentRule::Operator oper)
{
	switch (oper)
	{
	case ub::AssignmentRule::BeginsWith:
		return QObject::tr("Begins With");
	case ub::AssignmentRule::EndsWith:
		return QObject::tr("Ends With");
	case ub::AssignmentRule::StringEquals:
		return QObject::tr("Equals (String)");
	case ub::AssignmentRule::Contains:
		return QObject::tr("Contains");
	case ub::AssignmentRule::Before:
		return QObject::tr("Before");
	case ub::AssignmentRule::After:
		return QObject::tr("After");
	case ub::AssignmentRule::DateEquals:
		return QObject::tr("Equals (Date)");
	case ub::AssignmentRule::LessThan:
		return QObject::tr("Less Than");
	case ub::AssignmentRule::LessThanOrEqual:
		return QObject::tr("Less Than Or Equal");
	case ub::AssignmentRule::GreaterThan:
		return QObject::tr("Greater Than");
	case ub::AssignmentRule::GreaterThanOrEqual:
		return QObject::tr("Greater Than Or Equal");
	case ub::AssignmentRule::AmountEquals:
		return QObject::tr("Equals (Money)");
	default:
		return QObject::tr("Unknown operator");
	}
}

//------------------------------------------------------------------------------
AssignmentRule::Operator toOperatorEnum(const QString& str)
{
	if (str == "Begins With")
		return AssignmentRule::BeginsWith;
	else if (str == "Ends With")
		return AssignmentRule::EndsWith;
	else if (str == "Equals (String)")
		return AssignmentRule::StringEquals;
	else if (str == "Contains")
		return AssignmentRule::Contains;
	else if (str == "Before")
		return AssignmentRule::Before;
	else if (str == "After")
		return AssignmentRule::After;
	else if (str == "Equals (Date)")
		return AssignmentRule::DateEquals;
	else if (str == "Less Than")
		return AssignmentRule::LessThan;
	else if (str == "Less Than Or Equal")
		return AssignmentRule::LessThanOrEqual;
	else if (str == "Greater Than")
		return AssignmentRule::GreaterThan;
	else if (str == "Greater Than Or Equal")
		return AssignmentRule::GreaterThanOrEqual;
	else if (str == "Equals (Money)")
		return AssignmentRule::AmountEquals;
	else
		return AssignmentRule::OperatorNotDefined;
}

//------------------------------------------------------------------------------
QStringList operatorsFor(AssignmentRule::Field field)
{
	QStringList list;

	switch (field)
	{
	case ub::AssignmentRule::Date:
		list << toString(AssignmentRule::DateEquals);
		list << toString(AssignmentRule::Before);
		list << toString(AssignmentRule::After);
		break;
	case ub::AssignmentRule::Amount:
		list << toString(AssignmentRule::AmountEquals);
		list << toString(AssignmentRule::LessThan);
		list << toString(AssignmentRule::LessThanOrEqual);
		list << toString(AssignmentRule::GreaterThan);
		list << toString(AssignmentRule::GreaterThanOrEqual);
		break;
	case ub::AssignmentRule::Payee:
	case ub::AssignmentRule::Memo:
	case ub::AssignmentRule::DepositAccount:
	case ub::AssignmentRule::WithdrawalAccount:
		list << toString(AssignmentRule::StringEquals);
		list << toString(AssignmentRule::Contains);
		list << toString(AssignmentRule::BeginsWith);
		list << toString(AssignmentRule::EndsWith);
		break;
	default:
		list << toString(AssignmentRule::OperatorNotDefined);
	}

	return list;
}

}

