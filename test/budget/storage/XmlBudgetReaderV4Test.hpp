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

#ifndef XMLBUDGETREADERV4TEST_HPP
#define XMLBUDGETREADERV4TEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

// UnderBudget include(s)
#include "budget/AssignmentRule.hpp"
#include "budget/Estimate.hpp"

// Forward declaration(s)
class QIODevice;

namespace ub {

/**
 * Unit test for the XmlBudgetReader class when used on version 4.0 budgets.
 */
class XmlBudgetReaderV4Test : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Creates test fixtures.
	 */
	void init();

	/**
	 * Destroys test fixtures.
	 */
	void cleanup();

	/**
	 * Tests reading of a full valid version 4.0 XML budget.
	 */
	void readFullBudget();

	/**
	 * Tests reading of an incomplete, yet valid version 4.0 budget.
	 */
	void readIncompleteBudget();

	/**
	 * Tests reading of budgeting periods.
	 */
	void readBudgetingPeriods();

	/**
	 * Test data for reading of budgeting periods.
	 */
	void readBudgetingPeriods_data();

	/**
	 * Tests reading of rule conditions.
	 */
	void readRuleConditions();

	/**
	 * Test data for reading of rule conditions.
	 */
	void readRuleConditions_data();

private:
	/** Well-formed version 4.0 XML budget */
	QIODevice* fullBudget;
	/** Incomplete, yet valid version 4.0 budget */
	QIODevice* incompleteBudget;
};

}

#endif //XMLBUDGETREADERV4TEST_HPP
