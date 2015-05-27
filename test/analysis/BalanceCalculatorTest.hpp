/*
 * Copyright 2014 Kyle Treubig
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
#include <QtTest/QtTest>

namespace ub {

/**
 * Unit tests for the BalanceCalculator class.
 */
class BalanceCalculatorTest : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Tests the calculation of the estimated ending balance.
	 */
	void estimatedBalance();

	/**
	 * Tests the calculation of the actual ending balance.
	 */
	void actualBalance();

	/**
	 * Tests the calculation of the expected ending balance.
	 */
	void expectedBalance();

	/**
	 * Tests the recording of over-budget estimates.
	 */
	void overBudgetEstimates();

	/**
	 * Tests the recording of under-budget estimates.
	 */
	void underBudgetEstimates();
};

}

