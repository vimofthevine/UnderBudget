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

#ifndef ASSIGNMENTRULESTEST_HPP
#define ASSIGNMENTRULESTEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

// UnderBudget include(s)

namespace ub {

// Forward declaration(s)
class AssignmentRule;
class AssignmentRules;

/**
 * Unit tests for the AssignmentRules class.
 */
class AssignmentRulesTest : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Creates a test assignment rules list.
	 */
	void init();

	/**
	 * Tests retrieval of rule indices.
	 */
	void indexOf();

	/**
	 * Tests retrieval of rules by index.
	 */
	void atIndexRetrieval();

	/**
	 * Tests finding of rules by rule ID.
	 */
	void find();

	/**
	 * Tests finding of rules by estimate ID.
	 */
	void findFor();

	/**
	 * Tests append of rules to the list.
	 */
	void addRule();

	/**
	 * Tests removal of rule from beginning of the list.
	 */
	void removeFirstRule();

	/**
	 * Tests removal of rule from end of the list.
	 */
	void removeLastRule();

	/**
	 * Tests removal of rule from middle of the list.
	 */
	void removeRule();

	/**
	 * Tests removal of rule by ID.
	 */
	void removeByRuleId();

	/**
	 * Tests removal of rules by estimate ID.
	 */
	void removeByEstimate();

	/**
	 * Tests moving of rule to beginning of the list.
	 */
	void moveToBeginning();

	/**
	 * Tests moving of rule to end of the list.
	 */
	void moveToEnd();

	/**
	 * Tests moving of rule to middle of the list.
	 */
	void moveToMiddle();

private:
	// Test rules list
	QSharedPointer<AssignmentRules> rules;
	AssignmentRule* rule1;
	AssignmentRule* rule2;
	AssignmentRule* rule3;
};

}

#endif //ASSIGNMENTRULESTEST_HPP
