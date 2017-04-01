/*
 * UnderBudget
 * Copyright 2017 Kyle Treubig
 *
 * UnderBudget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UnderBudget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UnderBudget.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// Standard include(s)
#include <cstdint>
#include <iostream>
#include <stdexcept>

// Qt include(s)
#include <QMetaType>
#include <QString>

// UnderBudget include(s)
#include "Currency.hpp"

namespace ub {
namespace ledger {

/**
 * A discrete amount of money in a specific currency.
 *
 * Money values that are created without a specified currency
 * (e.g., `Money(123.45)` rather than `Money(123.45, "USD")`) will use the
 * default currency for the current locale.
 *
 * All simple arithmetic operations (addition, subtraction, multiplication, and
 * division) between money values of the same currency are supported. Additionally,
 * operations with money values and scalar values (i.e., int, double, etc.) are
 * supported via implicit conversion. For example, all three of the following
 * would produce the same result (assuming the two moneys in the First case
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
 * currency, an exception will be thrown.
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
 * @ingroup ledger
 */
class Money {
public:
    /**
     * Initializes a money value of the given amount in the given currency.
     *
     * @param[in] scaled   Monetary amount
     * @param[in] currency Monetary currency
     */
    explicit Money(int64_t scaled = 0, const Currency & currency = Currency());

    /**
     * Initializes a money value of the given amount in the given currency.
     *
     * @param[in] amount   Monetary amount
     * @param[in] currency Monetary currency
     */
    Money(double amount, const Currency & currency = Currency());

    // Copy constructor
    Money(const Money & orig) = default;

    // Assignment operator
    Money & operator=(const Money & orig) = default;

    /**
     * Returns the numerical amount of this money value.
     *
     * @return Amount of this money value
     */
    double amount() const;

    /**
     * Returns the currency of this money value.
     *
     * @return Currency of this money value
     */
    const Currency & currency() const;

    /**
     * Checks if this money value is negative.
     *
     * @return `true` if this money value is negative
     */
    bool isNegative() const;

    /**
     * Checks if this money value is equal in value to zero.
     *
     * @return `true` if this money value is equal to zero
     */
    bool isZero() const;

    /**
     * Returns the scaled integer amount of this money value.
     *
     * @return Scaled amount of this money value
     */
    int64_t scaled() const;

    /**
     * Creates a string representation of this money value.
     *
     * @return String representation of this money value
     */
    QString toString() const;

    /**
     * Creates a negative copy of this money value.
     *
     * @return Negative copy of this money value
     */
    const Money operator-() const;

    /**
     * Adds the given money value to this money value.
     *
     * @param[in] addend Money value to be added to this money value
     * @return This money value
     * @throw std::invalid_argument if the addend is of a different currency
     */
    const Money & operator+=(const Money & addend);

    /**
     * Subtracts the given money value from this money value.
     *
     * @param[in] subtrahend Money value to be subtracted from this money value
     * @return This money value
     * @throw std::invalid_argument if the subtrahend is of a different currency
     */
    const Money & operator-=(const Money & subtrahend);

    /**
     * Creates a money value representing the product of this
     * money value and the given scalar factor.
     *
     * @param[in] factor Scalar factor
     * @return Product of this money value and the scalar value
     */
    Money operator*(double factor) const;

    /**
     * Returns the ratio between this money value and the given money value.
     *
     * @param[in] divisor money value to divide into this money value
     * @return ratio between the money values
     * @throw std::invalid_argument if the divisor is of a different currency
     */
    double operator/(const Money & divisor) const;

    /**
     * Creates a money value representing a portion of this money value
     * as a result of dividing by the given scalar divisor.
     *
     * @param[in] divisor Scalar value to divide into this money value
     * @return Result of this money value divided by the given scalar
     */
    Money operator/(double divisor) const;

    /**
     * Checks if the given money values are not equal to each other
     * in amount or currency.
     *
     * @param[in] lhs First money value to be compared
     * @param[in] rhs Second money value to be compared
     * @return `true` if the given values are not equal to each other
     */
    friend bool operator!=(const Money & lhs, const Money & rhs);

    /**
     * Checks if the given money values are equal to each other
     * in amount and currency.
     *
     * @param[in] lhs First money value to be compared
     * @param[in] rhs Second money value to be compared
     * @return `true` if the given values are equal to each other
     */
    friend bool operator==(const Money & lhs, const Money & rhs);

    /**
     * Checks if the First given money value is greater than
     * the Second given money value.
     *
     * @param[in] lhs First money value to be compared
     * @param[in] rhs Second money value to be compared
     * @return `true` if the First given value is greater than the Second value
     * @throw std::invalid_argument if the currencies do not match
     */
    friend bool operator>(const Money & lhs, const Money & rhs);

    /**
     * Checks if the First given money value is greater than or equal to
     * the Second given money value.
     *
     * @param[in] lhs First money value to be compared
     * @param[in] rhs Second money value to be compared
     * @return `true` if the First given value is greater than or equal
     *         to the Second value
     * @throw std::invalid_argument if the currencies do not match
     */
    friend bool operator>=(const Money & lhs, const Money & rhs);

    /**
     * Checks if the First given money value is lesser than
     * the Second given money value.
     *
     * @param[in] lhs First money value to be compared
     * @param[in] rhs Second money value to be compared
     * @return `true` if the First given value is lesser than the Second value
     * @throw std::invalid_argument if the currencies do not match
     */
    friend bool operator<(const Money & lhs, const Money & rhs);

    /**
     * Checks if the First given money value is lesser than or equal to
     * the Second given money value.
     *
     * @param[in] lhs First money value to be compared
     * @param[in] rhs Second money value to be compared
     * @return `true` if the First given value is lesser than or equal
     *         to Second value
     * @throw std::invalid_argument if the currencies do not match
     */
    friend bool operator<=(const Money & lhs, const Money & rhs);

private:
    /** Scaled amount of money represented by this value */
    int64_t amount_;
    /** Currency of this money value */
    Currency currency_;

    /**
     * Verifies that the currency of the given money matches this money.
     *
     * @param[in] that Money whose currency is to be compared
     * @throw std::invalid_argument if the value is of a different currency
     */
    void assertCurrency(const Money & that) const;

    /**
     * Scales the given value by a constant factor to ensure that
     * precision is not lost. Additionally, excess digits are
     * stripped away. A total of 4 decimal digits are preserved.
     * For example, the value 12.34 is stored as 123400.
     *
     * @param[in] value Original, unscaled monetary amount
     * @return Scaled monetary amount
     */
    static int64_t scale(double value);

    /**
     * De-scales the given value by a constant factor back to
     * a human-readable form.
     *
     * @param[in] value Scaled monetary amount
     * @return Unscaled monetary amount
     */
    static double humanize(int64_t value);
};

/**
 * Creates a money value representing the sum of the given money values.
 *
 * @param[in] addend1 First money value to be added
 * @param[in] addend2 Second money value to be added
 * @return Sum of the money values
 * @throw std::invalid_argument if the currencies do not match
 */
const Money operator+(const Money & addend1, const Money & addend2);

/**
 * Creates a money value representing the difference of the given money values.
 *
 * @param[in] minuend money value from which to subtract
 * @param[in] subtrahend money value to be subtracted from the minuend
 * @return Difference of the money values
 * @throw std::invalid_argument if the currencies do not match
 */
const Money operator-(const Money & minuend, const Money & subtrahend);

/**
 * Creates a money value representing the product of a given scalar factor
 * and money value.
 *
 * @param[in] scalar Scalar factor
 * @param[in] money  Money factor
 * @return Product of the given scalar and  money value
 */
const Money operator*(double scalar, const Money & money);

/**
 * Prints this money value to the output stream.
 *
 * @param[in] os    Output stream
 * @param[in] money Money value
 * @return Output stream
 */
QDataStream & operator<<(QDataStream & os, const Money & money);

/**
 * Prints this money value to the output stream.
 *
 * @param[in] os    Output stream
 * @param[in] money Money value
 * @return Output stream
 */
std::ostream & operator<<(std::ostream & os, const Money & money);

} // ledger namespace
} // ub namespace

// Make Money known to QMetaType
Q_DECLARE_METATYPE(ub::ledger::Money)
