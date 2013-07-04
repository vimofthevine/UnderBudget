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
#include "budget/AssignmentRule.hpp"
#include "budget/AssignmentRules.hpp"
#include "budget/InsertRuleCommand.hpp"
#include "budget/MoveRuleCommand.hpp"
#include "budget/RemoveRuleCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
QSharedPointer<AssignmentRules> AssignmentRules::create()
{
	return QSharedPointer<AssignmentRules>(new AssignmentRules);
}

//------------------------------------------------------------------------------
AssignmentRules::AssignmentRules()
{ }

//------------------------------------------------------------------------------
AssignmentRules::~AssignmentRules()
{
	qDeleteAll(rules);
}

//------------------------------------------------------------------------------
AssignmentRule* AssignmentRules::createRule(uint ruleId, uint estimateId,
	const QList<AssignmentRule::Condition>& conditions)
{
	return insert(rules.size(), ruleId, estimateId, conditions);
}

//------------------------------------------------------------------------------
void AssignmentRules::reindex()
{
	ridToIndex.clear();
	eidToIndex.clear();

	for (int i=0; i<rules.size(); ++i)
	{
		AssignmentRule* rule = rules.at(i);
		ridToIndex.insert(rule->ruleId(), i);
		eidToIndex.insert(rule->estimateId(), i);
	}
}

//------------------------------------------------------------------------------
int AssignmentRules::size() const
{
	return rules.size();
}

//------------------------------------------------------------------------------
int AssignmentRules::indexOf(uint ruleId) const
{
	return ridToIndex.value(ruleId, -1);
}

//------------------------------------------------------------------------------
AssignmentRule* AssignmentRules::at(int index) const
{
	if (index < 0 || index >= rules.size())
		return 0;
	return rules.at(index);
}

//------------------------------------------------------------------------------
AssignmentRule* AssignmentRules::find(uint ruleId) const
{
	if (ridToIndex.contains(ruleId))
	{
		int index = ridToIndex.value(ruleId);
		if (index >=0 && index < rules.size())
		{
			return rules.at(index);
		}
	}

	return 0;
}

//------------------------------------------------------------------------------
QList<AssignmentRule*> AssignmentRules::findFor(uint estimateId) const
{
	QList<AssignmentRule*> foundRules;

	// According to the QMultiHash docs, this STL-style iterator is more
	// efficient than QMultiHash::values()
	QMultiHash<uint,int>::const_iterator iter = eidToIndex.find(estimateId);
	while (iter != eidToIndex.end() && iter.key() == estimateId)
	{
		int index = iter.value();
		if (index >= 0 && index < rules.size())
		{
			foundRules << rules.at(index);
		}
		++iter;
	}

	return foundRules;
}

//------------------------------------------------------------------------------
QUndoCommand* AssignmentRules::addRule(uint estimateId, QUndoCommand* cmd)
{
	uint ruleId = QDateTime::currentDateTime().toTime_t();
	return new InsertRuleCommand(this, rules.size(), ruleId, estimateId,
		QList<AssignmentRule::Condition>(), cmd);
}

//------------------------------------------------------------------------------
QUndoCommand* AssignmentRules::cloneRule(uint ruleId, QUndoCommand* cmd)
{
	AssignmentRule* rule = find(ruleId);
	if (rule)
	{
		uint cloneId = QDateTime::currentDateTime().toTime_t();
		int index = ridToIndex.value(ruleId) + 1; // Insert after original
		return new InsertRuleCommand(this, index, cloneId, rule->estimateId(),
			rule->conditions, cmd); // Private variable access
	}
	else
		return new QUndoCommand(cmd);
}

//------------------------------------------------------------------------------
AssignmentRule* AssignmentRules::insert(int index, uint ruleId, uint estimateId,
	const QList<AssignmentRule::Condition>& conditions)
{
	AssignmentRule* rule = new AssignmentRule(ruleId, estimateId, conditions, this);
	rules.insert(index, rule);
	reindex();
	emit ruleAdded(rule, index);
	return rule;
}

//------------------------------------------------------------------------------
QUndoCommand* AssignmentRules::removeAll(uint estimateId, QUndoCommand* cmd)
{
	QUndoCommand* removes = new QUndoCommand(cmd);

	// According to the QMultiHash docs, this STL-style iterator is more
	// efficient than QMultiHash::values()
	QMultiHash<uint,int>::const_iterator iter = eidToIndex.find(estimateId);
	while (iter != eidToIndex.end() && iter.key() == estimateId)
	{
		int index = iter.value();
		removeAt(index, removes);
		++iter;
	}

	return removes;
}

//------------------------------------------------------------------------------
QUndoCommand* AssignmentRules::removeRule(uint ruleId, QUndoCommand* cmd)
{
	if (ridToIndex.contains(ruleId))
	{
		return removeAt(ridToIndex.value(ruleId), cmd);
	}
	else
		return new QUndoCommand(cmd);
}

//------------------------------------------------------------------------------
QUndoCommand* AssignmentRules::removeAt(int index, QUndoCommand* cmd)
{
	if (index >= 0 && index < rules.size())
	{
		AssignmentRule* rule = rules.at(index);
		return new RemoveRuleCommand(this, index, rule->ruleId(),
			rule->estimateId(), rule->conditions, cmd); // Private variable access
	}
	else
		return new QUndoCommand(cmd);
}

//------------------------------------------------------------------------------
void AssignmentRules::remove(int index)
{
	if (index >= 0 && index < rules.size())
	{
		AssignmentRule* rule = rules.takeAt(index);
		reindex();
		emit ruleRemoved(rule, index);
		delete rule;
	}
}

//------------------------------------------------------------------------------
QUndoCommand* AssignmentRules::move(int from, int to, QUndoCommand* cmd)
{
	return new MoveRuleCommand(this, from, to, cmd);
}

//------------------------------------------------------------------------------
void AssignmentRules::moveRule(int from, int to)
{
	// Make sure new index is within the bounds of the rules list
	if (to < 0)
		to = 0;
	if (to >= rules.size())
		to = rules.size() - 1;

	AssignmentRule* rule = at(from);
	rules.move(from, to);
	reindex();
	emit ruleMoved(rule, from, to);
}

}

