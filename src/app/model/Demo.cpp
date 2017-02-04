// Standard include(s)
#include <memory>

// Qt include(s)
#include <QDebug>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountRepository.hpp>
#include <ledger/model/Currency.hpp>
#include <ledger/model/CurrencyRepository.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/model/LedgerEntry.hpp>
#include <ledger/model/Transaction.hpp>
#include <ledger/model/TransactionRepository.hpp>
#include "Demo.hpp"

namespace ub {

//--------------------------------------------------------------------------------------------------
void Demo::populate(std::shared_ptr<Repositories> repos) {
    using namespace ub::ledger;

    auto accounts = repos->accounts();
    // Don't populate in case real data was accidentially given
    if (accounts->getRoot().children().size() == 0u) {
        qDebug() << "Populating DB with demo data";

        {
            Account acct1;
            acct1.setName("Credit Cards");
            auto a1 = accounts->create(acct1, accounts->getRoot());

            Account sub_acct1b;
            sub_acct1b.setName("Blue Card");
            accounts->create(sub_acct1b, accounts->getAccount(a1));

            Account sub_acct1a;
            sub_acct1a.setName("Red Card");
            accounts->create(sub_acct1a, accounts->getAccount(a1));
        }

        {
            Account acct2;
            acct2.setName("Bank");
            accounts->create(acct2, accounts->getRoot());
        }

        {
            Account acct3;
            acct3.setName("Cash");
            accounts->create(acct3, accounts->getRoot());
        }
    }
}

} // ub namespace
