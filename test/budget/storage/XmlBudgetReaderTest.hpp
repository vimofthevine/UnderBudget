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

namespace ub {

/**
 * Unit test for the XmlBudgetReader class when used on invalid or
 * unsupported budgets.
 */
class XmlBudgetReaderTest : public QObject
{
	Q_OBJECT

private slots:
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
};

}

#endif //XMLBUDGETREADERTEST_HPP
