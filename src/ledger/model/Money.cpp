/*
 * Copyright 2016 Kyle Treubig
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
#include <iostream>
#include <stdexcept>

// UnderBudget include(s)
#include "Money.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
Money::Money(int scaled, const Currency & currency) : amount_(scaled), currency_(currency) {}

//--------------------------------------------------------------------------------------------------
Money::Money(double amount, const Currency & currency)
        : amount_(scale(amount)), currency_(currency) {}

//--------------------------------------------------------------------------------------------------
double Money::amount() const {
    return humanize(amount_);
}

//--------------------------------------------------------------------------------------------------
const Currency & Money::currency() const {
    return currency_;
}

//--------------------------------------------------------------------------------------------------
bool Money::isNegative() const {
    return (amount_ < 0);
}

//--------------------------------------------------------------------------------------------------
bool Money::isZero() const {
    return (amount_ == 0);
}

//--------------------------------------------------------------------------------------------------
int Money::scaled() const {
    return amount_;
}

//--------------------------------------------------------------------------------------------------
QString Money::toString() const {
    return currency_.format(humanize(amount_));
}

//--------------------------------------------------------------------------------------------------
const Money Money::operator-() const {
    return Money(humanize(-amount_), currency_);
}

//--------------------------------------------------------------------------------------------------
const Money & Money::operator+=(const Money & addend) {
    assertCurrency(addend);
    amount_ += addend.amount_;
    return *this;
}

//--------------------------------------------------------------------------------------------------
const Money & Money::operator-=(const Money & subtrahend) {
    assertCurrency(subtrahend);
    amount_ -= subtrahend.amount_;
    return *this;
}

//--------------------------------------------------------------------------------------------------
Money Money::operator*(double factor) const {
    return Money(humanize(amount_ * factor), currency_);
}

//--------------------------------------------------------------------------------------------------
double Money::operator/(const Money & divisor) const {
    assertCurrency(divisor);
    return ((double) amount_ / (double) divisor.amount_);
}

//--------------------------------------------------------------------------------------------------
Money Money::operator/(double divisor) const {
    return Money(humanize((double) amount_ / divisor), currency_);
}

//--------------------------------------------------------------------------------------------------
void Money::assertCurrency(const Money & that) const {
    if (this->currency_ != that.currency_) {
        throw std::invalid_argument("");
    }
}

//--------------------------------------------------------------------------------------------------
int Money::scale(double value) {
    return round(value * 10000);
}

//--------------------------------------------------------------------------------------------------
double Money::humanize(int value) {
    return (double) value / 10000;
}

//--------------------------------------------------------------------------------------------------
const Money operator+(const Money & addend1, const Money & addend2) {
    Money sum(addend1);
    sum += addend2;
    return sum;
}

//--------------------------------------------------------------------------------------------------
const Money operator-(const Money & minuend, const Money & subtrahend) {
    Money difference(minuend);
    difference -= subtrahend;
    return difference;
}

//--------------------------------------------------------------------------------------------------
const Money operator*(double scalar, const Money & money) {
    return money * scalar;
}

//--------------------------------------------------------------------------------------------------
bool operator!=(const Money & lhs, const Money & rhs) {
    return !(lhs == rhs);
}

//--------------------------------------------------------------------------------------------------
bool operator==(const Money & lhs, const Money & rhs) {
    return (lhs.amount_ == rhs.amount_) && (lhs.currency_ == rhs.currency_);
}

//--------------------------------------------------------------------------------------------------
bool operator>(const Money & lhs, const Money & rhs) {
    lhs.assertCurrency(rhs);
    return (lhs.amount_ > rhs.amount_);
}

//--------------------------------------------------------------------------------------------------
bool operator>=(const Money & lhs, const Money & rhs) {
    lhs.assertCurrency(rhs);
    return (lhs.amount_ >= rhs.amount_);
}

//--------------------------------------------------------------------------------------------------
bool operator<(const Money & lhs, const Money & rhs) {
    lhs.assertCurrency(rhs);
    return (lhs.amount_ < rhs.amount_);
}

//--------------------------------------------------------------------------------------------------
bool operator<=(const Money & lhs, const Money & rhs) {
    lhs.assertCurrency(rhs);
    return (lhs.amount_ <= rhs.amount_);
}

//--------------------------------------------------------------------------------------------------
QDataStream & operator<<(QDataStream & os, const Money & money) {
    return os << money.toString();
}

//--------------------------------------------------------------------------------------------------
std::ostream & operator<<(std::ostream & os, const Money & money) {
    return os << money.toString().toStdString();
}

} // ledger namespace
} // ub namespace
