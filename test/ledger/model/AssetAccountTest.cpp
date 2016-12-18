// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include <ledger/model/AssetAccount.hpp>
#include <ledger/model/Currency.hpp>
#include <ledger/model/Money.hpp>
#include "AssetAccountTest.hpp"

//--------------------------------------------------------------------------------------------------
QTEST_MAIN(ub::ledger::AssetAccountTest)

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
void AssetAccountTest::setCurrentBalanceShouldThrowIfCurrencyDoesNotMatch() {
    AssetAccount acct;
    acct.setCurrency(Currency("USD"));
    Money balance(10.0, Currency("UAH"));
    QVERIFY_EXCEPTION_THROWN(acct.setCurrentBalance(balance), std::invalid_argument);
}

//--------------------------------------------------------------------------------------------------
void AssetAccountTest::setReconciledBalanceShouldThrowIfCurrencyDoesNotMatch() {
    AssetAccount acct;
    acct.setCurrency(Currency("USD"));
    Money balance(10.0, Currency("UAH"));
    QVERIFY_EXCEPTION_THROWN(acct.setReconciledBalance(balance), std::invalid_argument);
}

} // ledger namespace
} // ub namespace
