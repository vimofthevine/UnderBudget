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

#ifndef ASSIGNMENTRULETEST_HPP
#define ASSIGNMENTRULETEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

// UnderBudget include(s)
#include "budget/AssignmentRule.hpp"
#include "budget/AssignmentRules.hpp"

namespace ub {

/**
 * Unit tests for the AssignmentRule class.
 */
class AssignmentRuleTest : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Creates a set of test assignment rules.
	 */
	void init();

	/**
	 * Tests condition counts of rules.
	 */
	void conditionCount();

	/**
	 * Tests condition retrieval of rules.
	 */
	void conditionAt();

private:
	// Test rules
	QSharedPointer<AssignmentRules> rules;
	AssignmentRule* ruleWithNoConditions;
	AssignmentRule* ruleWithOneCondition;
	AssignmentRule* ruleWithMultipleConditions;
};

}

#endif //ASSIGNMENTRULETEST_HPP
