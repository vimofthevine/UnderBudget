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
#include "analysis/Actuals.hpp"
#include "ActualsTest.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::ActualsTest)

namespace ub {

//------------------------------------------------------------------------------
void ActualsTest::initializingOfMultipleCurrencies()
{
	Actuals actuals;

	actuals.record(23, Money(23.4, "USD"));
	actuals.record(46, Money(123.4, "UAH"));

	QCOMPARE(actuals.forEstimate(46), Money(123.4, "UAH"));
	QCOMPARE(actuals.forEstimate(23), Money(23.4, "USD"));
}

//------------------------------------------------------------------------------
void ActualsTest::sumOfContributingActuals()
{
	Actuals actuals;

	actuals.record(4, Money(10, "USD"));
	actuals.record(4, Money(45.25, "USD"));
	actuals.record(4, Money(-5, "USD"));

	QCOMPARE(actuals.forEstimate(4), Money(50.25, "USD"));
}

}

