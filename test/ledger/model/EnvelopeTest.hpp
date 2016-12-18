#pragma once

// Qt include(s)
#include <QtTest/QtTest>

namespace ub {
namespace ledger {

/** Unit test for the Envelope class. */
class EnvelopeTest : public QObject {
    Q_OBJECT

private slots:
    /**
     * Verifies that setting the current balance throws an exception if the currency does not match
     * the envelope currency
     */
    void setCurrentBalanceShouldThrowIfCurrencyDoesNotMatch();
};

} // ledger namespace
} // ub namespace
