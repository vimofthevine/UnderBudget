// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include <ledger/model/Currency.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/model/Money.hpp>
#include "EnvelopeTest.hpp"

//--------------------------------------------------------------------------------------------------
QTEST_MAIN(ub::ledger::EnvelopeTest)

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
void EnvelopeTest::setCurrentBalanceShouldThrowIfCurrencyDoesNotMatch() {
    Envelope envelope;
    envelope.setCurrency(Currency("USD"));
    Money balance(10.0, Currency("UAH"));
    QVERIFY_EXCEPTION_THROWN(envelope.setCurrentBalance(balance), std::invalid_argument);
}

} // ledger namespace
} // ub namespace
