// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/Currency.hpp>
#include <ledger/model/Money.hpp>
#include "AccountTest.hpp"

//--------------------------------------------------------------------------------------------------
QTEST_MAIN(ub::ledger::AccountTest)

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
void AccountTest::setCurrentBalanceShouldThrowIfCurrencyDoesNotMatch() {
    Account acct;
    acct.setCurrency(Currency("USD"));
    Money balance(10.0, Currency("UAH"));
    QVERIFY_EXCEPTION_THROWN(acct.setCurrentBalance(balance), std::invalid_argument);
}

//--------------------------------------------------------------------------------------------------
void AccountTest::setReconciledBalanceShouldThrowIfCurrencyDoesNotMatch() {
    Account acct;
    acct.setCurrency(Currency("USD"));
    Money balance(10.0, Currency("UAH"));
    QVERIFY_EXCEPTION_THROWN(acct.setReconciledBalance(balance), std::invalid_argument);
}

} // ledger namespace
} // ub namespace
