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

#ifndef XMLBUDGETWRITERTEST_HPP
#define XMLBUDGETWRITERTEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

namespace ub {

/**
 * Unit test for the XmlBudgetWriter class.
 */
class XmlBudgetWriterTest : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Tests writing of a full budget.
	 */
	void writeCompleteBudget();

	/**
	 * Tests writing of budgeting periods.
	 */
	void writeBudgetingPeriods();

	/**
	 * Test data for writing of budgeting periods.
	 */
	void writeBudgetingPeriods_data();
};

}

#endif //XMLBUDGETWRITERTEST_HPP
