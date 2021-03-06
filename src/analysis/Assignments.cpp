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

// UnderBudget include(s)
#include "analysis/Assignments.hpp"

namespace ub {

//------------------------------------------------------------------------------
Assignments::Assignments(QObject* parent)
	: QObject(parent)
{ }

//------------------------------------------------------------------------------
void Assignments::clear()
{
	transactionToEstimate.clear();
	transactionToRule.clear();
	emit assignmentsChanged();
}

//------------------------------------------------------------------------------
void Assignments::record(uint trnId, uint estId, uint ruleId)
{
	transactionToEstimate[trnId] = estId;
	transactionToRule[trnId] = ruleId;
	emit assignmentsChanged();
}

//------------------------------------------------------------------------------
int Assignments::numberOfAssignments() const
{
	return transactionToEstimate.size();
}

//------------------------------------------------------------------------------
uint Assignments::estimate(uint trnId) const
{
	return transactionToEstimate.value(trnId, 0);
}

//------------------------------------------------------------------------------
uint Assignments::rule(uint trnId) const
{
	return transactionToRule.value(trnId, 0);
}

}

