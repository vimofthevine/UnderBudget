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

#ifndef ACTUALSTEST_HPP
#define ACTUALSTEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

namespace ub {

/**
 * Unit tests for the Actuals class.
 */
class ActualsTest : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Tests the storing of initial actual values of differing
	 * currencies, to verify that the currencies are retained.
	 */
	void initializingOfMultipleCurrencies();

	/**
	 * Tests the storing of additional, contributing actual values
	 * to the same estimate.
	 */
	void sumOfContributingActuals();
};

}


#endif //ACTUALSTEST_HPP
