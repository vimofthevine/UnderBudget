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
#include "analysis/SortedDifferences.hpp"
#include "SortedDifferencesTest.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::SortedDifferencesTest)

namespace ub {

//------------------------------------------------------------------------------
void SortedDifferencesTest::sort()
{
	SortedDifferences diffs;

	diffs.record(111, Money(67.24));
	diffs.record(222, Money(7.50));
	diffs.record(333, Money(84));
	diffs.record(444, Money(7.51));
	diffs.sort();

	QFETCH(int, index);
	QFETCH(uint, estimate);
	QFETCH(Money, amount);

	QCOMPARE(diffs.size() >= index, true);
	QCOMPARE(diffs.estimate(index), estimate);
	QCOMPARE(diffs.difference(index), amount);

	diffs.clear();
	QCOMPARE(diffs.size(), 0);
}

//------------------------------------------------------------------------------
void SortedDifferencesTest::sort_data()
{
	QTest::addColumn<int>("index");
	QTest::addColumn<uint>("estimate");
	QTest::addColumn<Money>("amount");

	QTest::newRow("0") << 0 << (uint) 333 << Money(84);
	QTest::newRow("1") << 1 << (uint) 111 << Money(67.24);
	QTest::newRow("2") << 2 << (uint) 444 << Money(7.51);
	QTest::newRow("3") << 3 << (uint) 222 << Money(7.50);
}

//------------------------------------------------------------------------------
void SortedDifferencesTest::outOfBounds()
{
	SortedDifferences diffs;

	diffs.record(111, Money(67.24));

	QFETCH(int, index);
	QFETCH(uint, estimate);
	QFETCH(Money, amount);

	QCOMPARE(diffs.estimate(index), estimate);
	QCOMPARE(diffs.difference(index), amount);
}

//------------------------------------------------------------------------------
void SortedDifferencesTest::outOfBounds_data()
{
	QTest::addColumn<int>("index");
	QTest::addColumn<uint>("estimate");
	QTest::addColumn<Money>("amount");

	QTest::newRow("below-zero") << -1 << (uint) 0 << Money();
	QTest::newRow("valid") << 0 << (uint) 111 << Money(67.24);
	QTest::newRow("larger") << 1 << (uint) 0 << Money();
}

}

