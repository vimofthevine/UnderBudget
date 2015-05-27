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

#ifndef PROJECTEDBALANCETEST_HPP
#define PROJECTEDBALANCETEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

namespace ub {

/**
 * Unit tests for the ProjectedBalance class.
 */
class ProjectedBalanceTest : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Tests the net increase to a projected balance.
	 */
	void netIncrease();

	/**
	 * Tests the net decrease to a projected balance.
	 */
	void netDecrease();

	/**
	 * Tests the net change to a projected balance.
	 */
	void netChange();

	/**
	 * Tests the resulting projected balance with an initial balance.
	 */
	void balance();
};

}

#endif //PROJECTEDBALANCETEST_HPP
