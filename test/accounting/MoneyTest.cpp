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
#include "accounting/ConversionRatesSource.hpp"
#include "accounting/Money.hpp"
#include "MoneyTest.hpp"
#include "StubConversionRates.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::MoneyTest)

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
void MoneyTest::init()
{
	ConversionRatesSource::set(new StubConversionRates(0.125));
}

//------------------------------------------------------------------------------
void MoneyTest::constructionFromScalar_data()
{
	QTest::addColumn<double>("value");
	QTest::addColumn<Money>("result");

	QTest::newRow("zero") << 0.0 << Money();
	QTest::newRow("positive") << 12.34 << Money(12.34);
	QTest::newRow("negative") << -46.30 << Money(-46.30);
}

//------------------------------------------------------------------------------
void MoneyTest::constructionFromScalar()
{
	QFETCH(double, value);
	QFETCH(Money, result);

	QCOMPARE(Money(value), result);
}

//------------------------------------------------------------------------------
void MoneyTest::toString_data()
{
	QTest::addColumn<Money>("money");
	QTest::addColumn<QString>("string");

	// Important! These are only tested with the en-US locale (see CurrencyTest
	// for more exhaustive testing of formatting in other locales)
	QTest::newRow("zero") << Money(0.0, "USD") << "$0.00";
	QTest::newRow("whole") << Money(12.0, "USD") << "$12.00";
	QTest::newRow("decimal") << Money(43.72, "USD") << "$43.72";
	QTest::newRow("thousands") << Money(1322.04, "USD") << "$1,322.04";
	QTest::newRow("negative") << Money(-3.92, "USD") << "($3.92)";
	QTest::newRow("foreign") << Money(1322.04, "UAH") << QChar(8372) + QString("1,322.04");
}

//------------------------------------------------------------------------------
void MoneyTest::toString()
{
	// Make sure we're using en-US locale
	QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

	QFETCH(Money, money);
	QFETCH(QString, string);
	QCOMPARE(money.toString(), string);
}

//------------------------------------------------------------------------------
void MoneyTest::zero_data()
{
	QTest::addColumn<Money>("money");
	QTest::addColumn<bool>("isZero");

	QTest::newRow("zero") << Money() << true;
	QTest::newRow("negative") << Money(-0.01) << false;
	QTest::newRow("positive") << Money(0.01) << false;
}

//------------------------------------------------------------------------------
void MoneyTest::zero()
{
	QFETCH(Money, money);
	QFETCH(bool, isZero);
	QCOMPARE(money.isZero(), isZero);
}

//------------------------------------------------------------------------------
void MoneyTest::negative_data()
{
	QTest::addColumn<Money>("money");
	QTest::addColumn<bool>("isNegative");

	QTest::newRow("zero") << Money() << false;
	QTest::newRow("negative") << Money(-0.01) << true;
	QTest::newRow("positive") << Money(0.01) << false;
}

//------------------------------------------------------------------------------
void MoneyTest::negative()
{
	QFETCH(Money, money);
	QFETCH(bool, isNegative);
	QCOMPARE(money.isNegative(), isNegative);
}

//------------------------------------------------------------------------------
void MoneyTest::negation_data()
{
	QTest::addColumn<Money>("money");
	QTest::addColumn<Money>("negative");

	QTest::newRow("zero") << Money() << Money();
	QTest::newRow("positive") << Money(22.45) << Money(-22.45);
	QTest::newRow("negative") << Money(-52.23) << Money(52.23);
}

//------------------------------------------------------------------------------
void MoneyTest::negation()
{
	QFETCH(Money, money);
	QFETCH(Money, negative);
	QCOMPARE(-money, negative);
	// double negation
	QCOMPARE(-(-money), money);
}

//------------------------------------------------------------------------------
void MoneyTest::addition_data()
{
	QTest::addColumn<Money>("addend1");
	QTest::addColumn<Money>("addend2");
	QTest::addColumn<Money>("sum");

	QTest::newRow("zeros") << Money() << Money() << Money();
	QTest::newRow("pos_pos_pos") << Money(1.30) << Money(2.05) << Money(3.35);
	QTest::newRow("pos_pos_pos_rev") << Money(2.05) << Money(1.30) << Money(3.35);
	QTest::newRow("neg_pos_neg") << Money(-3.2) << Money(1.2) << Money(-2.0);
	QTest::newRow("pos_neg_neg") << Money(1.2) << Money(-3.2) << Money(-2.0);
	QTest::newRow("neg_pos_pos") << Money(-3.2) << Money(6.5) << Money(3.3);
	QTest::newRow("pos_neg_pos") << Money(6.5) << Money(-3.2) << Money(3.3);
	QTest::newRow("neg_neg_neg") << Money(-2.4) << Money(-1.2) << Money(-3.6);
	QTest::newRow("neg_neg_neg_rev") << Money(-2.4) << Money(-1.2) << Money(-3.6);
	QTest::newRow("conversion") << Money(1.0, "USD") << Money(8.0, "UAH") << Money(2.0, "USD");
}

//------------------------------------------------------------------------------
void MoneyTest::addition()
{
	QFETCH(Money, addend1);
	QFETCH(Money, addend2);
	QFETCH(Money, sum);
	QCOMPARE(addend1 + addend2, sum);
}

//------------------------------------------------------------------------------
void MoneyTest::additionWithScalars()
{
	Money money(1.30);
	QCOMPARE(money + 2.05, Money(3.35));
	QCOMPARE(2.05 + money, Money(3.35));
}

//------------------------------------------------------------------------------
void MoneyTest::subtraction_data()
{
	QTest::addColumn<Money>("minuend");
	QTest::addColumn<Money>("subtrahend");
	QTest::addColumn<Money>("difference");

	QTest::newRow("zeros") << Money() << Money() << Money();
	QTest::newRow("pos_pos_pos") << Money(2.05) << Money(1.30) << Money(0.75);
	QTest::newRow("pos_pos_neg") << Money(1.30) << Money(2.05) << Money(-0.75);
	QTest::newRow("neg_pos_neg") << Money(-3.2) << Money(1.2) << Money(-4.4);
	QTest::newRow("pos_neg_pos") << Money(1.2) << Money(-3.2) << Money(4.4);
	QTest::newRow("neg_neg_pos") << Money(-2.1) << Money(-3.2) << Money(1.1);
	QTest::newRow("neg_neg_neg") << Money(-3.2) << Money(-2.1) << Money(-1.1);
	QTest::newRow("conversion") << Money(3.0, "USD") << Money(8.0, "UAH") << Money(2.0, "USD");
}

//------------------------------------------------------------------------------
void MoneyTest::subtraction()
{
	QFETCH(Money, minuend);
	QFETCH(Money, subtrahend);
	QFETCH(Money, difference);

	QCOMPARE(minuend - subtrahend, difference);
}

//------------------------------------------------------------------------------
void MoneyTest::subtractionWithScalars()
{
	Money money(2.05);
	QCOMPARE(money - 1.30, Money(0.75));
	QCOMPARE(1.30 - money, Money(-0.75));
}

//------------------------------------------------------------------------------
void MoneyTest::multiplication_data()
{
	QTest::addColumn<Money>("money");
	QTest::addColumn<double>("scalar");
	QTest::addColumn<Money>("product");

	QTest::newRow("zero_factors") << Money() << 0.0 << Money();
	QTest::newRow("zero_money") << Money() << 4.3 << Money();
	QTest::newRow("zero_scalar") << Money(3.45) << 0.0 << Money();
	QTest::newRow("one_money") << Money(1) << 0.23 << Money(0.23);
	QTest::newRow("one_scalar") << Money(2.63) << 1.0 << Money(2.63);
	QTest::newRow("pos_pos") << Money(2.50) << 2.0 << Money(5.0);
	QTest::newRow("pos_neg") << Money(3.33) << -3.0 << Money(-9.99);
	QTest::newRow("neg_pos") << Money(-2.0) << 4.0 << Money(-8.0);
	QTest::newRow("neg_neg") << Money(-1.3) << -2.0 << Money(2.6);
	QTest::newRow("fraction") << Money(100.00) << 0.25 << Money(25.00);
}

//------------------------------------------------------------------------------
void MoneyTest::multiplication()
{
	QFETCH(Money, money);
	QFETCH(double, scalar);
	QFETCH(Money, product);
	QCOMPARE(money * scalar, product);
}

//------------------------------------------------------------------------------
void MoneyTest::multiplicationWithScalars()
{
	Money money(100.00);
	QCOMPARE(0.0 * money, Money());
	QCOMPARE(1.0 * money, Money(100.00));
	QCOMPARE(0.25 * money, Money(25.0));
}

//------------------------------------------------------------------------------
void MoneyTest::divisionWithMoney_data()
{
	QTest::addColumn<Money>("dividend");
	QTest::addColumn<Money>("divisor");
	QTest::addColumn<double>("quotient");

	QTest::newRow("identity") << Money(23.5) << Money(1) << 23.5;
	QTest::newRow("equal") << Money(23.5) << Money(23.5) << 1.0;
	QTest::newRow("over") << Money(110) << Money(100) << 1.1;
	QTest::newRow("under") << Money(70) << Money(100) << 0.7;
	QTest::newRow("conversion") << Money(1.0, "USD") << Money(16.0, "UAH") << 0.5;
}

//------------------------------------------------------------------------------
void MoneyTest::divisionWithMoney()
{
	QFETCH(Money, dividend);
	QFETCH(Money, divisor);
	QFETCH(double, quotient);
	QCOMPARE(dividend / divisor, quotient);
}

//------------------------------------------------------------------------------
void MoneyTest::divisionWithScalar_data()
{
	QTest::addColumn<Money>("dividend");
	QTest::addColumn<double>("divisor");
	QTest::addColumn<Money>("quotient");

	QTest::newRow("identity") << Money(23.5) << 1.0 << Money(23.5);
	QTest::newRow("equal") << Money(23.5) << 23.5 << Money(1);
	QTest::newRow("over") << Money(110) << 100.0 << Money(1.1);
	QTest::newRow("under") << Money(70) << 100.0 << Money(0.7);
}

//------------------------------------------------------------------------------
void MoneyTest::divisionWithScalar()
{
	QFETCH(Money, dividend);
	QFETCH(double, divisor);
	QFETCH(Money, quotient);
	QCOMPARE(dividend / divisor, quotient);
}

//------------------------------------------------------------------------------
void MoneyTest::inequality_data()
{
	QTest::addColumn<Money>("lhs");
	QTest::addColumn<Money>("rhs");
	QTest::addColumn<bool>("equal");

	QTest::newRow("slightly_off") << Money(1.10) << Money(1.11) << true;
	QTest::newRow("slightly_off_rev") << Money(1.11) << Money(1.10) << true;
	QTest::newRow("opposite_signs") << Money(10) << Money(-10) << true;
	QTest::newRow("negatives") << Money(-23.53) << Money(-23.53) << false;
	QTest::newRow("large_exact") << Money(1234567.89) << Money(1234567.89) << false;
	QTest::newRow("diff_curr") << Money(1.1, "USD") << Money(1.1, "UAH") << true;
}

//------------------------------------------------------------------------------
void MoneyTest::inequality()
{
	QFETCH(Money, lhs);
	QFETCH(Money, rhs);
	QFETCH(bool, equal);
	QCOMPARE(lhs != rhs, equal);
}

//------------------------------------------------------------------------------
void MoneyTest::inequalityWithScalars()
{
	Money money(1.10);
	QCOMPARE(money != 1.11, true);
	QCOMPARE(1.11 != money, true);
	QCOMPARE(1.10 != money, false);
}

//------------------------------------------------------------------------------
void MoneyTest::equality_data()
{
	QTest::addColumn<Money>("lhs");
	QTest::addColumn<Money>("rhs");
	QTest::addColumn<bool>("equal");

	QTest::newRow("zero") << Money() << Money(0.0) << true;
	QTest::newRow("slightly_off") << Money(1.10) << Money(1.11) << false;
	QTest::newRow("slightly_off_rev") << Money(1.11) << Money(1.10) << false;
	QTest::newRow("opposite_signs") << Money(10) << Money(-10) << false;
	QTest::newRow("negatives") << Money(-23.53) << Money(-23.53) << true;
	QTest::newRow("large_exact") << Money(1234567.89) << Money(1234567.89) << true;
	QTest::newRow("diff_curr") << Money(1.1, "USD") << Money(1.1, "UAH") << false;
}

//------------------------------------------------------------------------------
void MoneyTest::equality()
{
	QFETCH(Money, lhs);
	QFETCH(Money, rhs);
	QFETCH(bool, equal);
	QCOMPARE(lhs == rhs, equal);
}

//------------------------------------------------------------------------------
void MoneyTest::equalityWithScalars()
{
	Money money(1.10);
	QCOMPARE(money == 1.11, false);
	QCOMPARE(1.11 == money, false);
	QCOMPARE(1.10 == money, true);
}

//------------------------------------------------------------------------------
void MoneyTest::greaterThan_data()
{
	QTest::addColumn<Money>("lhs");
	QTest::addColumn<Money>("rhs");
	QTest::addColumn<bool>("greaterThan");

	QTest::newRow("zero") << Money() << Money() << false;
	QTest::newRow("equal") << Money(23.4) << Money(23.4) << false;
	QTest::newRow("neg_pos") << Money(-3.4) << Money(45) << false;
	QTest::newRow("pos_neg") << Money(45) << Money(-3.4) << true;
	QTest::newRow("conversion") << Money(2.0, "USD") << Money(8.0, "UAH") << true;
}

//------------------------------------------------------------------------------
void MoneyTest::greaterThan()
{
	QFETCH(Money, lhs);
	QFETCH(Money, rhs);
	QFETCH(bool, greaterThan);
	QCOMPARE(lhs > rhs, greaterThan);
}

//------------------------------------------------------------------------------
void MoneyTest::greaterThanOrEqual_data()
{
	QTest::addColumn<Money>("lhs");
	QTest::addColumn<Money>("rhs");
	QTest::addColumn<bool>("greaterThan");

	QTest::newRow("zero") << Money() << Money() << true;
	QTest::newRow("equal") << Money(23.4) << Money(23.4) << true;
	QTest::newRow("neg_pos") << Money(-3.4) << Money(45) << false;
	QTest::newRow("pos_neg") << Money(45) << Money(-3.4) << true;
	QTest::newRow("conversion") << Money(1.0, "USD") << Money(8.0, "UAH") << true;
}

//------------------------------------------------------------------------------
void MoneyTest::greaterThanOrEqual()
{
	QFETCH(Money, lhs);
	QFETCH(Money, rhs);
	QFETCH(bool, greaterThan);
	QCOMPARE(lhs >= rhs, greaterThan);
}

//------------------------------------------------------------------------------
void MoneyTest::greaterThanWithScalars()
{
	Money money(23.4);
	QCOMPARE(0 > money, false);
	QCOMPARE(23.4 > money, false);
	QCOMPARE(23.4 >= money, true);
	QCOMPARE(45 > money, true);
}

//------------------------------------------------------------------------------
void MoneyTest::lesserThan_data()
{
	QTest::addColumn<Money>("lhs");
	QTest::addColumn<Money>("rhs");
	QTest::addColumn<bool>("lesserThan");

	QTest::newRow("zero") << Money() << Money() << false;
	QTest::newRow("equal") << Money(23.4) << Money(23.4) << false;
	QTest::newRow("neg_pos") << Money(-3.4) << Money(45) << true;
	QTest::newRow("pos_neg") << Money(45) << Money(-3.4) << false;
	QTest::newRow("conversion") << Money(1.0, "USD") << Money(8.0, "UAH") << false;
}

//------------------------------------------------------------------------------
void MoneyTest::lesserThan()
{
	QFETCH(Money, lhs);
	QFETCH(Money, rhs);
	QFETCH(bool, lesserThan);
	QCOMPARE(lhs < rhs, lesserThan);
}

//------------------------------------------------------------------------------
void MoneyTest::lesserThanOrEqual_data()
{
	QTest::addColumn<Money>("lhs");
	QTest::addColumn<Money>("rhs");
	QTest::addColumn<bool>("lesserThan");

	QTest::newRow("zero") << Money() << Money() << true;
	QTest::newRow("equal") << Money(23.4) << Money(23.4) << true;
	QTest::newRow("neg_pos") << Money(-3.4) << Money(45) << true;
	QTest::newRow("pos_neg") << Money(45) << Money(-3.4) << false;
	QTest::newRow("conversion") << Money(1.0, "USD") << Money(8.0, "UAH") << true;
}

//------------------------------------------------------------------------------
void MoneyTest::lesserThanOrEqual()
{
	QFETCH(Money, lhs);
	QFETCH(Money, rhs);
	QFETCH(bool, lesserThan);
	QCOMPARE(lhs <= rhs, lesserThan);
}

//------------------------------------------------------------------------------
void MoneyTest::lesserThanWithScalars()
{
	Money money(23.4);
	QCOMPARE(0 < money, true);
	QCOMPARE(23.4 < money, false);
	QCOMPARE(23.4 <= money, true);
	QCOMPARE(45 < money, false);
}

//------------------------------------------------------------------------------
void MoneyTest::assignment()
{
	Money orig(23.5);
	Money next(-2.30);
	orig = next;
	QCOMPARE(orig, next);
}

//------------------------------------------------------------------------------
void MoneyTest::conversion()
{
	QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
	Money uah(16.0, "UAH");
	Money local(8.0);

	QCOMPARE(uah.to("USD"), Money(2.0, "USD"));
	QCOMPARE(local.to("USD"), Money(8.0, "USD"));
	QCOMPARE(uah.toLocal(), Money(2.0, "USD"));
	QCOMPARE(local.toLocal(), Money(8.0, "USD"));
}

}

