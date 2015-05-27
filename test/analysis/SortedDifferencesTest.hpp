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

#ifndef SORTEDDIFFERENCESTEST_HPP
#define SORTEDDIFFERENCESTEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

namespace ub {

/**
 * Unit tests for the SortedDifferences class.
 */
class SortedDifferencesTest : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Tests the sorting of differences by value.
	 */
	void sort();

	/**
	 * Test data for testing sorting of the list.
	 */
	void sort_data();

	/**
	 * Tests out-of-bounds access.
	 */
	void outOfBounds();

	/**
	 * Test data for testing out-of-bounds.
	 */
	void outOfBounds_data();
};

}

#endif //SORTEDDIFFERENCESTEST_HPP
