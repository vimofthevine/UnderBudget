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

#ifndef XMLBUDGETREADERTEST_HPP
#define XMLBUDGETREADERTEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

// UnderBudget include(s)
#include "budget/AssignmentRule.hpp"
#include "budget/Estimate.hpp"

// Forward declaration(s)
class QIODevice;

namespace ub {

/**
 * Unit test for the XmlBudgetReader class.
 */
class XmlBudgetReaderTest : public QObject
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
	 * Tests reading of an invalid XML budget.
	 */
	void readInvalidXml();

	/**
	 * Tests reading of valid XML that is not a budget.
	 */
	void readNonBudgetXml();

	/**
	 * Tests reading of an XML budget from an older version.
	 */
	void readOldVersion();

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
	/** Obsolete version XML budget */
	QIODevice* oldVersion;
	/** Invalid XML budget */
	QIODevice* invalidXml;
	/** Not a budget XML */
	QIODevice* otherXml;

	/**
	 * Compares the given estimate against all expected parameters.
	 *
	 * @param[in] actual   actual estimate to be compared
	 * @param[in] id       expected estimate ID
	 * @param[in] name     expected name
	 * @param[in] descrip  expected description
	 * @param[in] type     expected estimate type
	 * @param[in] amount   expected estimated amount
	 * @param[in] due      expected due date
	 * @param[in] finished expected finished state
	 * @param[in] children expected number of children
	 */
	void compare(Estimate* actual, uint id, const QString& name,
		const QString& descrip, Estimate::Type type, const Money& amount,
		const QDate& due, bool finished, int children);

	/**
	 * Compares the given assignment rule against all expected parameters.
	 *
	 * @param[in] actual     actual assignment rule to be compared
	 * @param[in] estimateId expected associated estimate ID
	 * @param[in] conditions expected number of conditions
	 */
	void compare(AssignmentRule* actual,
		uint estimateId, int conditions);

	/**
	 * Compares the given assignment rule against all expected parameters.
	 *
	 * @param[in] actual     actual assignment rule to be compared
	 * @param[in] ruleId     expected rule ID
	 * @param[in] estimateId expected associated estimate ID
	 * @param[in] conditions expected number of conditions
	 */
	void compare(AssignmentRule* actual, uint ruleId,
		uint estimateId, int conditions);

	/**
	 * Compares the given rule condition against all expected parameters.
	 *
	 * @param[in] actual    actual rule condition to be compared
	 * @param[in] field     expected transaction field
	 * @param[in] oper      expected comparison operator
	 * @param[in] sensitive expected case-sensitivity
	 * @param[in] value     expected comparison value
	 */
	void compare(const AssignmentRule::Condition actual,
		AssignmentRule::Field field, AssignmentRule::Operator oper,
		bool sensitive, const QString& value);
};

}

#endif //XMLBUDGETREADERTEST_HPP
