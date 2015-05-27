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
#include "accounting/Money.hpp"
#include "analysis/ProjectedBalance.hpp"
#include "budget/Balance.hpp"
#include "ProjectedBalanceTest.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::ProjectedBalanceTest)

namespace QTest {

//------------------------------------------------------------------------------
template<>
char* toString(const ub::Money& money)
{
	QString str = QString("%1 (%2, %3)").arg(money.toString())
		.arg(QVariant(money.amount()).toString()).arg(money.currency().code());
	return toString(str);
}

}

namespace ub {

//------------------------------------------------------------------------------
void ProjectedBalanceTest::netIncrease()
{
	ProjectedBalance balance;

	balance.add(Money(12.25, "USD"));
	balance.add(Money(-2.05, "USD"));
	balance.add(Money(4.50, "USD"));
	balance.add(Money(-0.30, "USD"));

	QCOMPARE(balance.netIncrease(), Money(16.75, "USD"));
}

//------------------------------------------------------------------------------
void ProjectedBalanceTest::netDecrease()
{
	ProjectedBalance balance;

	balance.add(Money(12.25, "USD"));
	balance.add(Money(-2.05, "USD"));
	balance.add(Money(4.50, "USD"));
	balance.add(Money(-0.30, "USD"));

	QCOMPARE(balance.netDecrease(), Money(-2.35, "USD"));
}

//------------------------------------------------------------------------------
void ProjectedBalanceTest::netChange()
{
	ProjectedBalance balance;

	balance.add(Money(12.25, "USD"));
	balance.add(Money(-2.05, "USD"));
	balance.add(Money(4.50, "USD"));
	balance.add(Money(-0.30, "USD"));

	QCOMPARE(balance.netChange(), Money(14.40, "USD"));
}

//------------------------------------------------------------------------------
void ProjectedBalanceTest::balance()
{
	ProjectedBalance balance(Balance::create(Money(15.60, "USD")));

	balance.add(Money(12.25, "USD"));
	balance.add(Money(-2.05, "USD"));
	balance.add(Money(4.50, "USD"));
	balance.add(Money(-0.30, "USD"));

	QCOMPARE(balance.value(), Money(30.00, "USD"));
}

}

