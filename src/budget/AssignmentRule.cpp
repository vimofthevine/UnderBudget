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
	conditions.insert(index, condition);
	emit conditionUpdated(condition, index);
}

}

