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
AssignmentRule* AssignmentRules::createRule(uint ruleId, uint estimateId,
	const QList<AssignmentRule::Condition>& conditions)
{
	AssignmentRule* rule = new AssignmentRule(ruleId, estimateId, conditions, this);
	int index = rules.size();
	rules.append(rule);
	ridToIndex.insert(ruleId, index);
	eidToIndex.insert(estimateId, index);
	return rule;
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

}

