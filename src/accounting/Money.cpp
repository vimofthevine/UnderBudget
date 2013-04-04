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

// std include(s)
#include <cmath>
#include <limits>

// UnderBudget include(s)
#include "Money.hpp"

namespace ub {

//------------------------------------------------------------------------------
Money::Money(double amount, const Currency& currency)
	: scaledAmount(scale(amount)), currencyUnit(currency)
{ }

//------------------------------------------------------------------------------
Money::Money(const Money& orig)
	: scaledAmount(orig.scaledAmount), currencyUnit(orig.currencyUnit)
{ }

//------------------------------------------------------------------------------
Money& Money::operator=(const Money& other)
{
	scaledAmount = other.scaledAmount;
	currencyUnit = other.currencyUnit;
	return *this;
}

//------------------------------------------------------------------------------
const QString Money::toString() const
{
	return currencyUnit.format(humanize(scaledAmount));
}

//------------------------------------------------------------------------------
double Money::amount() const
{
	return humanize(scaledAmount);
}

//------------------------------------------------------------------------------
const Currency& Money::currency() const
{
	return currencyUnit;
}

//------------------------------------------------------------------------------
Money Money::to(const Currency& target) const
{
	double rate = currencyUnit.conversionRate(target);
	Money converted;
	converted.scaledAmount = scaledAmount * rate;
	converted.currencyUnit = target;
	return converted;
}

//------------------------------------------------------------------------------
Money Money::toLocal() const
{
	return to(Currency());
}

//------------------------------------------------------------------------------
bool Money::isZero() const
{
	return (scaledAmount == 0.0);
}

//------------------------------------------------------------------------------
bool Money::isNegative() const
{
	return (scaledAmount < 0.0);
}

//------------------------------------------------------------------------------
const Money Money::operator-() const
{
	Money negated;
	negated.scaledAmount = - scaledAmount;
	negated.currencyUnit = currencyUnit;
	return negated;
}

//------------------------------------------------------------------------------
const Money& Money::operator+=(const Money& addend)
{
	scaledAmount += convert(addend);
	return *this;
}

//------------------------------------------------------------------------------
const Money& Money::operator-=(const Money& subtrahend)
{
	scaledAmount -= convert(subtrahend);
	return *this;
}

//------------------------------------------------------------------------------
const Money Money::operator*(double factor) const
{
	Money product;
	product.scaledAmount = scaledAmount * factor;
	product.currencyUnit = currencyUnit;
	return product;
}

//------------------------------------------------------------------------------
const double Money::operator/(const Money& divisor) const
{
	return (scaledAmount / convert(divisor));
}

//------------------------------------------------------------------------------
const Money Money::operator/(double divisor) const
{
	Money quotient;
	quotient.scaledAmount = scaledAmount / divisor;
	quotient.currencyUnit = currencyUnit;
	return quotient;
}

//------------------------------------------------------------------------------
double Money::convert(const Money& other) const
{
	double rate = other.currencyUnit.conversionRate(currencyUnit);
	return other.scaledAmount * rate;
}

//------------------------------------------------------------------------------
double Money::scale(double value)
{
	double scaled = value * 10000;
	return (scaled > 0.0)
		? floor(scaled)
		: ceil(scaled);
}

//------------------------------------------------------------------------------
double Money::humanize(double value)
{
	return value / 10000;
}

//------------------------------------------------------------------------------
const Money operator+(const Money& addend1, const Money& addend2)
{
	Money sum(addend1);
	sum += addend2;
	return sum;
}

//------------------------------------------------------------------------------
const Money operator-(const Money& minuend, const Money& subtrahend)
{
	Money difference(minuend);
	difference -= subtrahend;
	return difference;
}

//------------------------------------------------------------------------------
const Money operator*(double scalar, const Money& money)
{
	return money * scalar;
}

//------------------------------------------------------------------------------
bool operator!=(const Money& lhs, const Money& rhs)
{
	return ! (lhs == rhs);
}

//------------------------------------------------------------------------------
bool operator==(const Money& lhs, const Money& rhs)
{
	double epsilon = std::numeric_limits<double>::epsilon();
	double diff = lhs.scaledAmount - rhs.scaledAmount;
	double rel = std::fabs(std::min(lhs.scaledAmount, rhs.scaledAmount))/1.e15;
	bool amtsEqual = (std::fabs(diff) < epsilon) || (std::fabs(diff) < rel);
	return amtsEqual && (lhs.currencyUnit == rhs.currencyUnit);
}

//------------------------------------------------------------------------------
bool operator>(const Money& lhs, const Money& rhs)
{
	return (lhs.scaledAmount > lhs.convert(rhs));
}

//------------------------------------------------------------------------------
bool operator>=(const Money& lhs, const Money& rhs)
{
	return (lhs.scaledAmount >= lhs.convert(rhs));
}

//------------------------------------------------------------------------------
bool operator<(const Money& lhs, const Money& rhs)
{
	return (lhs.scaledAmount < lhs.convert(rhs));
}

//------------------------------------------------------------------------------
bool operator<=(const Money& lhs, const Money& rhs)
{
	return (lhs.scaledAmount <= lhs.convert(rhs));
}

//------------------------------------------------------------------------------
QDataStream& operator<<(QDataStream& os, const Money& money)
{
	return os << money.toString();
}

}

