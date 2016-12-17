#pragma once

// Qt include(s)
#include <QtTest/QtTest>

namespace ub {
namespace ledger {

/** Unit test for the Money class. */
class MoneyTest : public QObject {
    Q_OBJECT

private slots:
    /** Verifies that precision of monies is maintained up to the fourth decimal */
    void shouldMaintainPrecisionToFourthDecimal();

    /** Sets up test data for the precision test */
    void shouldMaintainPrecisionToFourthDecimal_data();

    /** Verifies that monies are formatted according to their currency */
    void shouldFormatAccordingToCurrency();

    /** Sets up test data for the formatting test */
    void shouldFormatAccordingToCurrency_data();

    /** Verifies that monies can be tested for zero value */
    void shouldBeZeroIfValueIsZero();

    /** Sets up test data for the zero test */
    void shouldBeZeroIfValueIsZero_data();

    /** Verifies that monies can be tested for negative value */
    void shouldBeNegativeIfValueIsNegative();

    /** Sets up test data for the negative test */
    void shouldBeNegativeIfValueIsNegative_data();

    /** Verifies that the negation operator creates a copy with a negative value */
    void negationShouldCreateCopyWithNegatedValue();

    /** Sets up test data for the negation test */
    void negationShouldCreateCopyWithNegatedValue_data();

    /** Verifies that two monies can be added together when the same currency */
    void additionShouldAddMoniesWhenSameCurrency();

    /** Sets up test data for the addition test */
    void additionShouldAddMoniesWhenSameCurrency_data();

    /** Verifies that an exception is thrown when adding monies of different currencies */
    void additionShouldThrowWhenDifferentCurrency();

    /** Verifies that monies can be added to scalar values */
    void additionShouldAddScalarToMoney();

    /** Verifies that two monies can be subtracted from each other when the same currency */
    void subtractionShouldSubtractMoniesWhenSameCurrency();

    /** Sets up test data for the subtraction test */
    void subtractionShouldSubtractMoniesWhenSameCurrency_data();

    /** Verifies that an exception is thrown when subtracting monies of different currencies */
    void subtractionShouldThrowWhenDifferentCurrency();

    /** Verifies that monies can be subtracted with scalar values */
    void subtractionShouldSubtractScalarWithMoney();

    /** Verifies that monies can be muliplied with scalar values */
    void multiplicationShouldMultiplyScalarWithMoney();

    /** Sets up test data for the multiplication test */
    void multiplicationShouldMultiplyScalarWithMoney_data();

    /** Verifies that two monies can be divided against each other when the same currency */
    void divisionShouldDivideMoniesWhenSameCurrency();

    /** Sets up test data for the division test */
    void divisionShouldDivideMoniesWhenSameCurrency_data();

    /** Verifies that an exception is thrown when dividing monies of different currencies */
    void divisionShouldThrowWhenDifferentCurrency();

    /** Verifies that monies can be divided against scalar values */
    void divisionShouldDivideMoneyByScalar();

    /** Sets up test data for the scalar division test */
    void divisionShouldDivideMoneyByScalar_data();

    /** Verifies that monies can be compared for inequality */
    void inequalShouldCompareCurrencyAndAmount();

    /** Sets up test data for the inequality test */
    void inequalShouldCompareCurrencyAndAmount_data();

    /** Verifies that monies can be compared for inequality with a scalar value */
    void inequalShouldCompareMoneyAgainstScalar();

    /** Verifies that monies can be compared for equality */
    void equalShouldCompareCurrencyAndAmount();

    /** Sets up test data for the equality test */
    void equalShouldCompareCurrencyAndAmount_data();

    /** Verifies that monies can be compared for equality with a scalar value */
    void equalShouldCompareMoneyAgainstScalar();

    /** Verifies that greater-than operator compares monies when the same currency */
    void greaterThanShouldCompareMoniesWhenSameCurrency();

    /** Sets up test data for the greater-than test */
    void greaterThanShouldCompareMoniesWhenSameCurrency_data();

    /** Verifies that greater-than operator throws when comparing monies of different currencies */
    void greaterThanShouldThrowWhenDifferentCurrency();

    /** Verifies that greater-than operator compares monies with scalar values */
    void greaterThanShouldCompareMoneyWithScalar();

    /** Verifies that greater-than-or-equal operator compares monies when the same currency */
    void greaterThanOrEqualShouldCompareMoniesWhenSameCurrency();

    /** Sets up test data for the greater-than-or-equal test */
    void greaterThanOrEqualShouldCompareMoniesWhenSameCurrency_data();

    /**
     * Verifies that greater-than-or-equal operator throws when comparing monies of different
     * currencies
     */
    void greaterThanOrEqualShouldThrowWhenDifferentCurrency();

    /** Verifies that greater-than-or-equal operator compares monies with scalar values */
    void greaterThanOrEqualShouldCompareMoneyWithScalar();

    /** Verifies that lesser-than operator compares monies when the same currency */
    void lesserThanShouldCompareMoniesWhenSameCurrency();

    /** Sets up test data for the lesser-than test */
    void lesserThanShouldCompareMoniesWhenSameCurrency_data();

    /** Verifies that lesser-than operator throws when comparing monies of different currencies */
    void lesserThanShouldThrowWhenDifferentCurrency();

    /** Verifies that greater-than operator compares monies with scalar values */
    void lesserThanShouldCompareMoneyWithScalar();

    /** Verifies that lesser-than-or-equal operator compares monies when the same currency */
    void lesserThanOrEqualShouldCompareMoniesWhenSameCurrency();

    /** Sets up test data for the lesser-than-or-equal test */
    void lesserThanOrEqualShouldCompareMoniesWhenSameCurrency_data();

    /**
     * Verifies that lesser-than-or-equal operator throws when comparing monies of different
     * currencies
     */
    void lesserThanOrEqualShouldThrowWhenDifferentCurrency();

    /** Verifies that greater-than-or-equal operator compares monies with scalar values */
    void lesserThanOrEqualShouldCompareMoneyWithScalar();
};

} // ledger namespace
} // ub namespace
