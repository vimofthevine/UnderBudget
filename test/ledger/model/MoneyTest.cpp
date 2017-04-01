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

//
// Qt include(s)
#include <QtCore>
#include <QtTest>

// UnderBudget include(s)
#include <ledger/model/Currency.hpp>
#include <ledger/model/Money.hpp>
#include "MoneyTest.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::ledger::MoneyTest)

namespace QTest {

//------------------------------------------------------------------------------
template <> char * toString(const ub::ledger::Money & money) {
    QString str = QString("%1 (%2, %3)")
                      .arg(money.toString())
                      .arg(QVariant(money.amount()).toString())
                      .arg(money.currency().code());
    return toString(str);
}

} // QTest namespace

namespace ub {
namespace ledger {

//------------------------------------------------------------------------------
void MoneyTest::shouldMaintainPrecisionToFourthDecimal_data() {
    QTest::addColumn<double>("constructValue");
    QTest::addColumn<double>("storedValue");

    QTest::newRow("pos-0-dec") << (double) 12 << (double) 12;
    QTest::newRow("pos-1-dec") << 12.3 << 12.3;
    QTest::newRow("pos-2-dec") << 12.34 << 12.34;
    QTest::newRow("pos-3-dec") << 12.345 << 12.345;
    QTest::newRow("pos-4-dec") << 12.3456 << 12.3456;
    QTest::newRow("pos-5-up") << 12.34567 << 12.3457;
    QTest::newRow("pos-5-down") << 12.34564 << 12.3456;
    QTest::newRow("pos-5-even") << 12.34565 << 12.3456;
    QTest::newRow("pos-5-odd") << 12.34575 << 12.3458;

    QTest::newRow("neg-0-dec") << (double) -12 << (double) -12;
    QTest::newRow("neg-1-dec") << -12.3 << -12.3;
    QTest::newRow("neg-2-dec") << -12.34 << -12.34;
    QTest::newRow("neg-3-dec") << -12.345 << -12.345;
    QTest::newRow("neg-4-dec") << -12.3456 << -12.3456;
    QTest::newRow("neg-5-up") << -12.34567 << -12.3457;
    QTest::newRow("neg-5-down") << -12.34564 << -12.3456;
    QTest::newRow("neg-5-even") << -12.34565 << -12.3456;
    QTest::newRow("neg-5-odd") << -12.34575 << -12.3458;
}

//------------------------------------------------------------------------------
void MoneyTest::shouldMaintainPrecisionToFourthDecimal() {
    QFETCH(double, constructValue);
    QFETCH(double, storedValue);

    Money money(constructValue);
    QCOMPARE(money.amount(), storedValue);

    Money another(storedValue);
    QCOMPARE(money, another);
}

//------------------------------------------------------------------------------
void MoneyTest::shouldFormatAccordingToCurrency_data() {
    QTest::addColumn<Money>("money");
    QTest::addColumn<QString>("string");

    // Important! These are only tested with the en-US locale (see CurrencyTest
    // for more exhaustive testing of formatting in other locales)
    QTest::newRow("zero") << Money(0.0, Currency(0, "USD")) << "$0.00";
    QTest::newRow("whole") << Money(12.0, Currency(0, "USD")) << "$12.00";
    QTest::newRow("decimal") << Money(43.72, Currency(0, "USD")) << "$43.72";
    QTest::newRow("thousands") << Money(1322.04, Currency(0, "USD")) << "$1,322.04";
    QTest::newRow("negative") << Money(-3.92, Currency(0, "USD")) << "$-3.92";
    QTest::newRow("foreign") << Money(1322.04, Currency(0, "UAH"))
                             << QChar(8372) + QString("1,322.04");
    QTest::newRow("pos-precision") << Money(1.234567, Currency(0, "USD")) << "$1.23";
    QTest::newRow("neg-precision") << Money(-1.234567, Currency(0, "USD")) << "$-1.23";
}

//------------------------------------------------------------------------------
void MoneyTest::shouldFormatAccordingToCurrency() {
    // Make sure we're using en-US locale
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

    QFETCH(Money, money);
    QFETCH(QString, string);
    QCOMPARE(money.toString(), string);
}

//------------------------------------------------------------------------------
void MoneyTest::shouldBeZeroIfValueIsZero_data() {
    QTest::addColumn<Money>("money");
    QTest::addColumn<bool>("isZero");

    QTest::newRow("zero") << Money() << true;
    QTest::newRow("negative") << Money(-0.01) << false;
    QTest::newRow("positive") << Money(0.01) << false;
}

//------------------------------------------------------------------------------
void MoneyTest::shouldBeZeroIfValueIsZero() {
    QFETCH(Money, money);
    QFETCH(bool, isZero);
    QCOMPARE(money.isZero(), isZero);
}

//------------------------------------------------------------------------------
void MoneyTest::shouldBeNegativeIfValueIsNegative_data() {
    QTest::addColumn<Money>("money");
    QTest::addColumn<bool>("isNegative");

    QTest::newRow("zero") << Money() << false;
    QTest::newRow("negative") << Money(-0.01) << true;
    QTest::newRow("positive") << Money(0.01) << false;
}

//------------------------------------------------------------------------------
void MoneyTest::shouldBeNegativeIfValueIsNegative() {
    QFETCH(Money, money);
    QFETCH(bool, isNegative);
    QCOMPARE(money.isNegative(), isNegative);
}

//------------------------------------------------------------------------------
void MoneyTest::negationShouldCreateCopyWithNegatedValue_data() {
    QTest::addColumn<Money>("money");
    QTest::addColumn<Money>("negative");

    QTest::newRow("zero") << Money() << Money();
    QTest::newRow("positive") << Money(22.45) << Money(-22.45);
    QTest::newRow("negative") << Money(-52.23) << Money(52.23);
}

//------------------------------------------------------------------------------
void MoneyTest::negationShouldCreateCopyWithNegatedValue() {
    QFETCH(Money, money);
    QFETCH(Money, negative);
    QCOMPARE(-money, negative);
    // double negation
    QCOMPARE(-(-money), money);
}

//------------------------------------------------------------------------------
void MoneyTest::additionShouldAddMoniesWhenSameCurrency_data() {
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
}

//------------------------------------------------------------------------------
void MoneyTest::additionShouldAddMoniesWhenSameCurrency() {
    QFETCH(Money, addend1);
    QFETCH(Money, addend2);
    QFETCH(Money, sum);
    QCOMPARE(addend1 + addend2, sum);
}

//------------------------------------------------------------------------------
void MoneyTest::additionShouldThrowWhenDifferentCurrency() {
    Money usd(1.0, Currency(0, "USD"));
    Money uah(8.0, Currency(0, "UAH"));
    try {
        usd + uah;
        QFAIL("Should have thrown");
    } catch (std::invalid_argument) {
    } catch (...) {
        QFAIL("Should have thrown std::invalid_argument");
    }
}

//------------------------------------------------------------------------------
void MoneyTest::additionShouldAddScalarToMoney() {
    Money money(1.30);
    QCOMPARE(money + 2.05, Money(3.35));
    QCOMPARE(2.05 + money, Money(3.35));
}

//------------------------------------------------------------------------------
void MoneyTest::subtractionShouldSubtractMoniesWhenSameCurrency_data() {
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
}

//------------------------------------------------------------------------------
void MoneyTest::subtractionShouldSubtractMoniesWhenSameCurrency() {
    QFETCH(Money, minuend);
    QFETCH(Money, subtrahend);
    QFETCH(Money, difference);

    QCOMPARE(minuend - subtrahend, difference);
}

//------------------------------------------------------------------------------
void MoneyTest::subtractionShouldThrowWhenDifferentCurrency() {
    Money usd(2.0, Currency(0, "USD"));
    Money uah(8.0, Currency(0, "UAH"));
    try {
        usd - uah;
        QFAIL("Should have thrown");
    } catch (std::invalid_argument) {
    } catch (...) {
        QFAIL("Should have thrown std::invalid_argument");
    }
}

//------------------------------------------------------------------------------
void MoneyTest::subtractionShouldSubtractScalarWithMoney() {
    Money money(2.05);
    QCOMPARE(money - 1.30, Money(0.75));
    QCOMPARE(1.30 - money, Money(-0.75));
}

//------------------------------------------------------------------------------
void MoneyTest::multiplicationShouldMultiplyScalarWithMoney_data() {
    QTest::addColumn<Money>("money");
    QTest::addColumn<double>("scalar");
    QTest::addColumn<Money>("product");

    QTest::newRow("zero_factors") << Money() << 0.0 << Money();
    QTest::newRow("zero_money") << Money() << 4.3 << Money();
    QTest::newRow("zero_scalar") << Money(3.45) << 0.0 << Money();
    QTest::newRow("one_money") << Money(1.0) << 0.23 << Money(0.23);
    QTest::newRow("one_scalar") << Money(2.63) << 1.0 << Money(2.63);
    QTest::newRow("pos_pos") << Money(2.50) << 2.0 << Money(5.0);
    QTest::newRow("pos_neg") << Money(3.33) << -3.0 << Money(-9.99);
    QTest::newRow("neg_pos") << Money(-2.0) << 4.0 << Money(-8.0);
    QTest::newRow("neg_neg") << Money(-1.3) << -2.0 << Money(2.6);
    QTest::newRow("fraction") << Money(100.00) << 0.25 << Money(25.00);
}

//------------------------------------------------------------------------------
void MoneyTest::multiplicationShouldMultiplyScalarWithMoney() {
    QFETCH(Money, money);
    QFETCH(double, scalar);
    QFETCH(Money, product);
    QCOMPARE(money * scalar, product);
    QCOMPARE(scalar * money, product);
}

//------------------------------------------------------------------------------
void MoneyTest::divisionShouldDivideMoniesWhenSameCurrency_data() {
    QTest::addColumn<Money>("dividend");
    QTest::addColumn<Money>("divisor");
    QTest::addColumn<double>("quotient");

    QTest::newRow("identity") << Money(23.5) << Money(1.0) << 23.5;
    QTest::newRow("equal") << Money(23.5) << Money(23.5) << 1.0;
    QTest::newRow("over") << Money(110.0) << Money(100.0) << 1.1;
    QTest::newRow("under") << Money(70.0) << Money(100.0) << 0.7;
}

//------------------------------------------------------------------------------
void MoneyTest::divisionShouldDivideMoniesWhenSameCurrency() {
    QFETCH(Money, dividend);
    QFETCH(Money, divisor);
    QFETCH(double, quotient);
    QCOMPARE(dividend / divisor, quotient);
}

//------------------------------------------------------------------------------
void MoneyTest::divisionShouldThrowWhenDifferentCurrency() {
    Money usd(1.0, Currency(0, "USD"));
    Money uah(16.0, Currency(0, "UAH"));
    try {
        usd / uah;
        QFAIL("Should have thrown");
    } catch (std::invalid_argument) {
    } catch (...) {
        QFAIL("Should have thrown std::invalid_argument");
    }
}

//------------------------------------------------------------------------------
void MoneyTest::divisionShouldDivideMoneyByScalar_data() {
    QTest::addColumn<Money>("dividend");
    QTest::addColumn<double>("divisor");
    QTest::addColumn<Money>("quotient");

    QTest::newRow("identity") << Money(23.5) << 1.0 << Money(23.5);
    QTest::newRow("equal") << Money(23.5) << 23.5 << Money(1.0);
    QTest::newRow("over") << Money(110.0) << 100.0 << Money(1.1);
    QTest::newRow("under") << Money(70.0) << 100.0 << Money(0.7);
}

//------------------------------------------------------------------------------
void MoneyTest::divisionShouldDivideMoneyByScalar() {
    QFETCH(Money, dividend);
    QFETCH(double, divisor);
    QFETCH(Money, quotient);
    QCOMPARE(dividend / divisor, quotient);
}

//------------------------------------------------------------------------------
void MoneyTest::inequalShouldCompareCurrencyAndAmount_data() {
    QTest::addColumn<Money>("lhs");
    QTest::addColumn<Money>("rhs");
    QTest::addColumn<bool>("equal");

    QTest::newRow("slightly_off") << Money(1.10) << Money(1.11) << true;
    QTest::newRow("slightly_off_rev") << Money(1.11) << Money(1.10) << true;
    QTest::newRow("opposite_signs") << Money(10.0) << Money(-10.0) << true;
    QTest::newRow("negatives") << Money(-23.53) << Money(-23.53) << false;
    QTest::newRow("large_exact") << Money(1234567.89) << Money(1234567.89) << false;
    QTest::newRow("diff_curr") << Money(1.1, Currency(0, "USD")) << Money(1.1, Currency(0, "UAH"))
                               << true;
    QTest::newRow("precision-eq") << Money(1.2345678) << Money(1.2345678) << false;
    QTest::newRow("precision-round") << Money(1.2345687) << Money(1.2345678) << false;
    QTest::newRow("precision-ne") << Money(1.2345478) << Money(1.2345513) << true;
}

//------------------------------------------------------------------------------
void MoneyTest::inequalShouldCompareCurrencyAndAmount() {
    QFETCH(Money, lhs);
    QFETCH(Money, rhs);
    QFETCH(bool, equal);
    QCOMPARE(lhs != rhs, equal);
}

//------------------------------------------------------------------------------
void MoneyTest::inequalShouldCompareMoneyAgainstScalar() {
    Money money(1.10);
    QCOMPARE(money != 1.11, true);
    QCOMPARE(1.11 != money, true);
    QCOMPARE(1.10 != money, false);
}

//------------------------------------------------------------------------------
void MoneyTest::equalShouldCompareCurrencyAndAmount_data() {
    QTest::addColumn<Money>("lhs");
    QTest::addColumn<Money>("rhs");
    QTest::addColumn<bool>("equal");

    QTest::newRow("zero") << Money() << Money(0.0) << true;
    QTest::newRow("slightly_off") << Money(1.10) << Money(1.11) << false;
    QTest::newRow("slightly_off_rev") << Money(1.11) << Money(1.10) << false;
    QTest::newRow("opposite_signs") << Money(10.0) << Money(-10.0) << false;
    QTest::newRow("negatives") << Money(-23.53) << Money(-23.53) << true;
    QTest::newRow("large_exact") << Money(1234567.89) << Money(1234567.89) << true;
    QTest::newRow("diff_curr") << Money(1.1, Currency(0, "USD")) << Money(1.1, Currency(0, "UAH"))
                               << false;
    QTest::newRow("precision-eq") << Money(1.2345678) << Money(1.2345678) << true;
    QTest::newRow("precision-round") << Money(1.2345687) << Money(1.2345678) << true;
    QTest::newRow("precision-ne") << Money(1.2345478) << Money(1.2345513) << false;
}

//------------------------------------------------------------------------------
void MoneyTest::equalShouldCompareCurrencyAndAmount() {
    QFETCH(Money, lhs);
    QFETCH(Money, rhs);
    QFETCH(bool, equal);
    QCOMPARE(lhs == rhs, equal);
}

//------------------------------------------------------------------------------
void MoneyTest::equalShouldCompareMoneyAgainstScalar() {
    Money money(1.10);
    QCOMPARE(money == 1.11, false);
    QCOMPARE(1.11 == money, false);
    QCOMPARE(1.10 == money, true);
}

//------------------------------------------------------------------------------
void MoneyTest::greaterThanShouldCompareMoniesWhenSameCurrency_data() {
    QTest::addColumn<Money>("lhs");
    QTest::addColumn<Money>("rhs");
    QTest::addColumn<bool>("greaterThan");

    QTest::newRow("zero") << Money() << Money() << false;
    QTest::newRow("equal") << Money(23.4) << Money(23.4) << false;
    QTest::newRow("neg_pos") << Money(-3.4) << Money(45.0) << false;
    QTest::newRow("pos_neg") << Money(45.0) << Money(-3.4) << true;
}

//------------------------------------------------------------------------------
void MoneyTest::greaterThanShouldCompareMoniesWhenSameCurrency() {
    QFETCH(Money, lhs);
    QFETCH(Money, rhs);
    QFETCH(bool, greaterThan);
    QCOMPARE(lhs > rhs, greaterThan);
}

//------------------------------------------------------------------------------
void MoneyTest::greaterThanShouldThrowWhenDifferentCurrency() {
    Money usd(2.0, Currency(0, "USD"));
    Money uah(8.0, Currency(0, "UAH"));
    try {
        usd > uah;
        QFAIL("Should have thrown");
    } catch (std::invalid_argument) {
    } catch (...) {
        QFAIL("Should have thrown std::invalid_argument");
    }
}

//------------------------------------------------------------------------------
void MoneyTest::greaterThanShouldCompareMoneyWithScalar() {
    Money money(23.4);
    QCOMPARE(0 > money, false);
    QCOMPARE(23.4 > money, false);
    QCOMPARE(45 > money, true);
}

//------------------------------------------------------------------------------
void MoneyTest::greaterThanOrEqualShouldCompareMoniesWhenSameCurrency_data() {
    QTest::addColumn<Money>("lhs");
    QTest::addColumn<Money>("rhs");
    QTest::addColumn<bool>("greaterThan");

    QTest::newRow("zero") << Money() << Money() << true;
    QTest::newRow("equal") << Money(23.4) << Money(23.4) << true;
    QTest::newRow("neg_pos") << Money(-3.4) << Money(45.0) << false;
    QTest::newRow("pos_neg") << Money(45.0) << Money(-3.4) << true;
}

//------------------------------------------------------------------------------
void MoneyTest::greaterThanOrEqualShouldCompareMoniesWhenSameCurrency() {
    QFETCH(Money, lhs);
    QFETCH(Money, rhs);
    QFETCH(bool, greaterThan);
    QCOMPARE(lhs >= rhs, greaterThan);
}

//------------------------------------------------------------------------------
void MoneyTest::greaterThanOrEqualShouldThrowWhenDifferentCurrency() {
    Money usd(2.0, Currency(0, "USD"));
    Money uah(8.0, Currency(0, "UAH"));
    try {
        usd >= uah;
        QFAIL("Should have thrown");
    } catch (std::invalid_argument) {
    } catch (...) {
        QFAIL("Should have thrown std::invalid_argument");
    }
}

//------------------------------------------------------------------------------
void MoneyTest::greaterThanOrEqualShouldCompareMoneyWithScalar() {
    Money money(23.4);
    QCOMPARE(0 >= money, false);
    QCOMPARE(23.4 >= money, true);
    QCOMPARE(45 >= money, true);
}

//------------------------------------------------------------------------------
void MoneyTest::lesserThanShouldCompareMoniesWhenSameCurrency_data() {
    QTest::addColumn<Money>("lhs");
    QTest::addColumn<Money>("rhs");
    QTest::addColumn<bool>("lesserThan");

    QTest::newRow("zero") << Money() << Money() << false;
    QTest::newRow("equal") << Money(23.4) << Money(23.4) << false;
    QTest::newRow("neg_pos") << Money(-3.4) << Money(45.0) << true;
    QTest::newRow("pos_neg") << Money(45.0) << Money(-3.4) << false;
}

//------------------------------------------------------------------------------
void MoneyTest::lesserThanShouldCompareMoniesWhenSameCurrency() {
    QFETCH(Money, lhs);
    QFETCH(Money, rhs);
    QFETCH(bool, lesserThan);
    QCOMPARE(lhs < rhs, lesserThan);
}

//------------------------------------------------------------------------------
void MoneyTest::lesserThanShouldThrowWhenDifferentCurrency() {
    Money usd(2.0, Currency(0, "USD"));
    Money uah(8.0, Currency(0, "UAH"));
    try {
        usd < uah;
        QFAIL("Should have thrown");
    } catch (std::invalid_argument) {
    } catch (...) {
        QFAIL("Should have thrown std::invalid_argument");
    }
}

//------------------------------------------------------------------------------
void MoneyTest::lesserThanShouldCompareMoneyWithScalar() {
    Money money(23.4);
    QCOMPARE(0 < money, true);
    QCOMPARE(23.4 < money, false);
    QCOMPARE(45 < money, false);
}

//------------------------------------------------------------------------------
void MoneyTest::lesserThanOrEqualShouldCompareMoniesWhenSameCurrency_data() {
    QTest::addColumn<Money>("lhs");
    QTest::addColumn<Money>("rhs");
    QTest::addColumn<bool>("lesserThan");

    QTest::newRow("zero") << Money() << Money() << true;
    QTest::newRow("equal") << Money(23.4) << Money(23.4) << true;
    QTest::newRow("neg_pos") << Money(-3.4) << Money(45.0) << true;
    QTest::newRow("pos_neg") << Money(45.0) << Money(-3.4) << false;
}

//------------------------------------------------------------------------------
void MoneyTest::lesserThanOrEqualShouldCompareMoniesWhenSameCurrency() {
    QFETCH(Money, lhs);
    QFETCH(Money, rhs);
    QFETCH(bool, lesserThan);
    QCOMPARE(lhs <= rhs, lesserThan);
}

//------------------------------------------------------------------------------
void MoneyTest::lesserThanOrEqualShouldThrowWhenDifferentCurrency() {
    Money usd(2.0, Currency(0, "USD"));
    Money uah(8.0, Currency(0, "UAH"));
    try {
        usd <= uah;
        QFAIL("Should have thrown");
    } catch (std::invalid_argument) {
    } catch (...) {
        QFAIL("Should have thrown std::invalid_argument");
    }
}

//------------------------------------------------------------------------------
void MoneyTest::lesserThanOrEqualShouldCompareMoneyWithScalar() {
    Money money(23.4);
    QCOMPARE(0 <= money, true);
    QCOMPARE(23.4 <= money, true);
    QCOMPARE(45 <= money, false);
}

} // ledger namespace
} // ub namespace
