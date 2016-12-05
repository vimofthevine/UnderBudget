#pragma once

// Qt include(s)
#include <QtTest/QtTest>

namespace ub {
namespace ledger {

/** Unit test for the Currency class. */
class CurrencyTest : public QObject {
    Q_OBJECT

private slots:
    /** Verifies that the default constructor creates a currency for the current locale */
    void defaultConstructorShouldUseCurrentLocale();

    /** Sets up test data for the default constructor test */
    void defaultConstructorShouldUseCurrentLocale_data();

    /** Verifies that values are formatted correctly according to the currency type */
    void shouldFormatValuesWithAppropriateCurrencySymbol();

    /** Sets up test data for the format test */
    void shouldFormatValuesWithAppropriateCurrencySymbol_data();
};

} // ledger namespace
} // ub namespace
