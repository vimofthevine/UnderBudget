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

#ifndef MONEY_HPP
#define MONEY_HPP

// Qt include(s)
#include <QObject>

// UnderBudget include(s)
#include "accounting/Currency.hpp"

namespace ub {

/**
 * A discrete amount of money in a specific currency.
 *
 * Money values that are created without a specified currency
 * (e.g., `Money(123.45)` rather than `Money(123.45, "USD")`) will use the
 * default currency for the current locale.
 *
 * All simple arithmetic operations (addition, subtraction, multiplication, and
 * division) between money values are supported. Additionally,
 * operations with money values and scalar values (i.e., int, double, etc.) are
 * supported via implicit conversion. For example, all three of the following
 * would produce the same result (assuming the two moneys in the first case
 * are of the same currency):
 *
 *     Money(123.45) + Money(23.50)
 *     Money(123.45) + 23.50
 *     23.50 + Money(123.45)
 *
 * The only exception would be division operations. Division of a money value
 * by a scalar will produce another money value (e.g., `Money(100) / 4 ==
 * Money(25)`) while division of a money value by another money value will
 * produce the ratio between the two (e.g., `Money(75) / Money(100) == 0.75`).
 *
 * During an operation on two money values, if they are not of the same
 * currency, the amount of the second money value will be converted to the
 * currency of the first money value. The conversion rate used is acquired from
 * the `ConversionRates` * returned by `ConversionRates::factory()`.
 *
 * It should be noted that this class is not necessarily designed with the
 * intention of being used as a generic, all-purpose money library, although
 * it could be (perhaps with further testing and precious handling). It was
 * specifically created to be used in UnderBudget and to support the operations
 * that are known to be used within the UnderBudget application. For example,
 * the only arithmetic operations used as part of UnderBudget are addition and
 * subtraction. These are the easiest operations to implement with money values
 * and retain the right amount of precision. The multiplication and division
 * operations, although implemented, may not maintain the correct amount of
 * accuracy and precision to be used in a banking application. The division
 * operation is used within UnderBudget as a fuzzy percentage calculation,
 * rather than an exact, accurate mathematical operation.
 *
 * @ingroup accounting
 */
class Money : public QObject
{
	Q_OBJECT

public:
	/**
	 * Constructs a money value of the given amount in the
	 * given currency.
	 *
	 * @param[in] amount   monetary amount
	 * @param[in] currency monetary currency
	 */
	Money(double amount = 0.0, const Currency& currency = Currency());

	/**
	 * Constructs a money value as a copy of the given money value.
	 *
	 * @param[in] orig money value to be copied
	 */
	Money(const Money& orig);

	/**
	 * Assigns the given money value to this money value.
	 *
	 * @param[in] other new money value
	 */
	Money& operator=(const Money& other);

	/**
	 * Creates a string representation of this money value.
	 *
	 * @return string representation of this money value
	 */
	const QString toString() const;

	/**
	 * Returns the amount of this money value.
	 *
	 * @return amount of this money value
	 */
	double amount() const;

	/**
	 * Returns the currency of this money value.
	 *
	 * @return currency of this money value
	 */
	const Currency& currency() const;

	/**
	 * Checks if this money value is equal in value to zero.
	 *
	 * @return `true` if this money value is equal to zero
	 */
	bool isZero() const;

	/**
	 * Checks if this money value is negative.
	 *
	 * @return `true` if this money value is negative
	 */
	bool isNegative() const;

	/**
	 * Creates a negative copy of this money value.
	 *
	 * @return negative copy of this money value
	 */
	const Money operator-() const;

	/**
	 * Adds the given money value to this money value, converting
	 * the given value into this value's currency if they differ.
	 *
	 * @param[in] addend money value to be added to this money value
	 * @return this money value
	 */
	const Money& operator+=(const Money& addend);

	/**
	 * Subtracts the given money value from this money value, converting
	 * the given value into this value's currency if they differ.
	 *
	 * @param[in] subtrahend money value to be subtracted from this money value
	 * @return this money value
	 */
	const Money& operator-=(const Money& subtrahend);

	/**
	 * Creates a money value representing the product of this
	 * money value and the given scalar factor.
	 *
	 * @param[in] factor scalar factor
	 * @return product of this money value and the scalar value
	 */
	const Money operator*(double factor) const;

	/**
	 * Returns the ratio between this money value and the given money value.
	 *
	 * @param[in] divisor money value to divide into this money value
	 * @return ratio between the money values
	 */
	const double operator/(const Money& divisor) const;

	/**
	 * Creates a money value representing a portion of this money value
	 * as a result of dividing by the given scalar divisor.
	 *
	 * @param[in] divisor scalar value to divide into this money value
	 * @return result of this money value divided by the given scalar
	 */
	const Money operator/(double divisor) const;

	/**
	 * Checks if the given money values are not equal to each other
	 * in amount or currency.
	 *
	 * @param[in] lhs first money value to be compared
	 * @param[in] rhs second money value to be compared
	 * @return `true` if the given values are not equal to each other
	 */
	friend bool operator!=(const Money& lhs, const Money& rhs);

	/**
	 * Checks if the given money values are equal to each other
	 * in amount and currency.
	 *
	 * @param[in] lhs first money value to be compared
	 * @param[in] rhs second money value to be compared
	 * @return `true` if the given values are equal to each other
	 */
	friend bool operator==(const Money& lhs, const Money& rhs);

	/**
	 * Checks if the first given money value is greater than
	 * the second given money value. If the second value is not of the same
	 * currency as the first value, it is automatically converted.
	 *
	 * @param[in] lhs first money value to be compared
	 * @param[in] rhs second money value to be compared
	 * @return `true` if the first given value is greater than the second value
	 */
	friend bool operator>(const Money& lhs, const Money& rhs);

	/**
	 * Checks if the first given money value is greater than or equal to
	 * the second given money value. If the second value is not of the same
	 * currency as the first value, it is automatically converted.
	 *
	 * @param[in] lhs first money value to be compared
	 * @param[in] rhs second money value to be compared
	 * @return `true` if the first given value is greater than or equal
	 *         to the second value
	 */
	friend bool operator>=(const Money& lhs, const Money& rhs);

	/**
	 * Checks if the first given money value is lesser than
	 * the second given money value. If the second value is not of the same
	 * currency as the first value, it is automatically converted.
	 *
	 * @param[in] lhs first money value to be compared
	 * @param[in] rhs second money value to be compared
	 * @return `true` if the first given value is lesser than the second value
	 */
	friend bool operator<(const Money& lhs, const Money& rhs);

	/**
	 * Checks if the first given money value is lesser than or equal to
	 * the second given money value. If the second value is not of the same
	 * currency as the first value, it is automatically converted.
	 *
	 * @param[in] lhs first money value to be compared
	 * @param[in] rhs second money value to be compared
	 * @return `true` if the first given value is lesser than or equal
	 *         to second value
	 */
	friend bool operator<=(const Money& lhs, const Money& rhs);

private:
	/** Scaled amount of money represented by this value */
	double scaledAmount;
	/** Currency of this money value */
	Currency currencyUnit;

	/**
	 * Converts the scaled amount of the given money value
	 * to a scaled amount in this money value's currency.
	 *
	 * @param[in] other other money value to be converted
	 * @return scaled amount of other money value in this value's currency
	 */
	double convert(const Money& other) const;

	/**
	 * Scales the given value by a constant factor to ensure that
	 * precision is not lost. Additionally, excess digits are
	 * stripped away. A total of 4 decimal digits are preserved.
	 * For example, the value 12.34 is stored as 123400.
	 *
	 * @param[in] value original, unscaled monetary amount
	 * @return scaled monetary amount
	 */
	static double scale(double value);

	/**
	 * De-scales the given value by a constant factor back to
	 * a human-readable form.
	 *
	 * @param[in] value scaled monetary amount
	 * @return unscaled monetary amount
	 */
	static double humanize(double value);
};

/**
 * Creates a money value representing the sum of the given money values.
 *
 * @param[in] addend1 first money value to be added
 * @param[in] addend2 second money value to be added
 * @return sum of the money values
 */
const Money operator+(const Money& addend1, const Money& addend2);

/**
 * Creates a money value representing the difference of the given money values.
 *
 * @param[in] minuend money value from which to subtract
 * @param[in] subtrahend money value to be subtracted from the minuend
 * @return difference of the money values
 */
const Money operator-(const Money& minuend, const Money& subtrahend);

/**
 * Creates a money value representing the product of a given scalar factor
 * and money value.
 *
 * @param[in] scalar scalar factor
 * @param[in] money money factor
 * @return product of the given scalar and  money value
 */
const Money operator*(double scalar, const Money& money);

}

// Make Money known to QMetaType
Q_DECLARE_METATYPE(ub::Money)

#endif // MONEY_HPP
