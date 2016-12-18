#pragma once

// Qt include(s)
#include <QtTest/QtTest>

namespace ub {
namespace ledger {

/** Unit test for the AssetAccount class. */
class AssetAccountTest : public QObject {
    Q_OBJECT

private slots:
    /**
     * Verifies that setting the current balance throws an exception if the currency does not match
     * the account currency
     */
    void setCurrentBalanceShouldThrowIfCurrencyDoesNotMatch();

    /**
     * Verifies that setting the reconciled balance throws an exception if the currency does not match
     * the account currency
     */
    void setReconciledBalanceShouldThrowIfCurrencyDoesNotMatch();
};

} // ledger namespace
} // ub namespace
