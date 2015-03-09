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

#ifndef UIPREFSTEST_HPP
#define UIPREFSTEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

// UnderBudget include(s)
#include "budget/UIPrefs.hpp"

namespace ub {

/**
 * Unit tests for the UIPrefs class.
 */
class UIPrefsTest : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Tests retrieval of all keys
	 */
	void allKeys();

	/**
	 * Tests removal all entries
	 */
	void clear();

	/**
	 * Tests removal of single entry
	 */
	void remove();

	/**
	 * Tests retrieval of entries
	 */
	void value();
};

}

#endif //UIPREFSTEST_HPP
