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

#ifndef MONEYTEST_HPP
#define MONEYTEST_HPP

// Qt include(s)
#include <QtTest/QtTest>

// UnderBudget include(s)

namespace ub {

/**
 * Unit tests for the Money class.
 */
class MoneyTest : public QObject
{
	Q_OBJECT

private slots:
	/**
	 * Sets up a stub conversion rates source to be used for testing
	 * automatic conversion during arithmetic.
	 */
	void init();

	/**
	 * Tests the construction of money values from scalar.
	 */
	void constructionFromScalar();

	/**
	 * Test data for construction from scalar.
	 */
	void constructionFromScalar_data();

	/**
	 * Tests the precision of money values.
	 */
	void precision();

	/**
	 * Test data for testing precision.
	 */
	void precision_data();

	/**
	 * Tests the string representation of a money value.
	 */
	void toString();

	/**
	 * Test data for string represetations.
	 */
	void toString_data();

	/**
	 * Tests comparison with zero of a money value.
	 */
	void zero();

	/**
	 * Test data for zero.
	 */
	void zero_data();

	/**
	 * Tests comparison as negative of a money value.
	 */
	void negative();

	/**
	 * Test data for negative.
	 */
	void negative_data();

	/**
	 * Tests negation of a money value.
	 */
	void negation();

	/**
	 * Test data for negation.
	 */
	void negation_data();

	/**
	 * Tests addition of two money values.
	 */
	void addition();

	/**
	 * Test data for addition.
	 */
	void addition_data();

	/**
	 * Tests addition of money and scalar values.
	 */
	void additionWithScalars();

	/**
	 * Tests subtraction of two money values.
	 */
	void subtraction();

	/**
	 * Test data for subtraction.
	 */
	void subtraction_data();

	/**
	 * Tests subtraction of money and scalar values.
	 */
	void subtractionWithScalars();

	/**
	 * Tests multiplication of a money value and a scalar value.
	 */
	void multiplication();

	/**
	 * Test data for multiplication.
	 */
	void multiplication_data();

	/**
	 * Tests multiplication of money and scalar values.
	 */
	void multiplicationWithScalars();

	/**
	 * Tests division of two money values.
	 */
	void divisionWithMoney();

	/**
	 * Test data for division with money.
	 */
	void divisionWithMoney_data();

	/**
	 * Tests division of a money value with a scalar value.
	 */
	void divisionWithScalar();

	/**
	 * Test data for division with scalars.
	 */
	void divisionWithScalar_data();

	/**
	 * Tests inequality comparison of two money values.
	 */
	void inequality();

	/**
	 * Test data for inequality.
	 */
	void inequality_data();

	/**
	 * Tests inequality comparison of a money value and a scalar value.
	 */
	void inequalityWithScalars();

	/**
	 * Tests equality comparison of two money values.
	 */
	void equality();

	/**
	 * Tests equality comparison of a money value and a scalar value.
	 */
	void equalityWithScalars();

	/**
	 * Test data for equality.
	 */
	void equality_data();

	/**
	 * Tests greater-than comparison of two money values.
	 */
	void greaterThan();

	/**
	 * Test data for greater-than.
	 */
	void greaterThan_data();

	/**
	 * Tests greater-than-or-equal comparison of two money values.
	 */
	void greaterThanOrEqual();

	/**
	 * Test data for greater-than-or-equal.
	 */
	void greaterThanOrEqual_data();

	/**
	 * Tests greater-than (or-equal) comparison of a money value and a scalar.
	 */
	void greaterThanWithScalars();

	/**
	 * Tests lesser-than comparison of two money values.
	 */
	void lesserThan();

	/**
	 * Test data for lesser-than.
	 */
	void lesserThan_data();

	/**
	 * Tests lesser-than-or-equal comparison of two money values.
	 */
	void lesserThanOrEqual();

	/**
	 * Test data for lesser-than-or-equal.
	 */
	void lesserThanOrEqual_data();

	/**
	 * Tests lesser-than (or-equal) comparison of a money value and a scalar.
	 */
	void lesserThanWithScalars();

	/**
	 * Tests assignment operator.
	 */
	void assignment();

	/**
	 * Tests explicit conversion.
	 */
	void conversion();
};

}

#endif // MONEYTEST_HPP
