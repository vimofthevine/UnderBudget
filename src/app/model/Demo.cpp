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
#include <ledger/model/EnvelopeRepository.hpp>
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

        // --- Accounts

        Account acct1;
        acct1.setName("Credit Cards");
        auto a1 = accounts->create(acct1, accounts->getRoot());

        Account sub_acct1b;
        sub_acct1b.setName("Blue Card");
        auto a1a = accounts->create(sub_acct1b, accounts->getAccount(a1));

        Account sub_acct1a;
        sub_acct1a.setName("Red Card");
        auto a1b = accounts->create(sub_acct1a, accounts->getAccount(a1));

        Account acct2;
        acct2.setName("Bank");
        auto a2 = accounts->create(acct2, accounts->getRoot());

        Account acct3;
        acct3.setName("Cash");
        auto a3 = accounts->create(acct3, accounts->getRoot());

        qInfo() << "Account insertion error (if any):" << accounts->lastError();

        // --- Envelopes

        auto envelopes = repos->envelopes();

        Envelope env1;
        env1.setName("Food");
        auto e1 = envelopes->create(env1, envelopes->getRoot());

        Envelope env2;
        env2.setName("Utilities");
        auto e2 = envelopes->create(env2, envelopes->getRoot());

        Envelope sub_env2a;
        sub_env2a.setName("Rent");
        auto e2a = envelopes->create(sub_env2a, envelopes->getEnvelope(e2));

        Envelope sub_env2b;
        sub_env2b.setName("Electric");
        auto e2b = envelopes->create(sub_env2b, envelopes->getEnvelope(e2));

        Envelope env3;
        env3.setName("Gifts");
        auto e3 = envelopes->create(env3, envelopes->getRoot());

        Envelope env4;
        env4.setName("Unallocated");
        auto e4 = envelopes->create(env4, envelopes->getRoot());

        qInfo() << "Envelope insertion error (if any):" << envelopes->lastError();

        // --- Transactions

        auto transactions = repos->transactions();

        {
            AccountTransaction at1;
            at1.setAccount(accounts->getAccount(a1a));
            at1.setAmount(Money(-52.33));

            EnvelopeTransaction et1;
            et1.setAmount(-52.33);
            et1.setEnvelope(envelopes->getEnvelope(e2b));

            Transaction t1;
            t1.setDate(QDate(2017, 2, 25));
            t1.setPayee("electric bill");

            LedgerEntry entry1(transactions);
            entry1.updateTransaction(t1);
            entry1.addSplit(at1);
            entry1.addSplit(et1);
            if (not entry1.save()) {
                qWarning() << "Transaction insertion error:" << entry1.lastError();
            }
        }

        {
            AccountTransaction at2;
            at2.setAccount(accounts->getAccount(a3));
            at2.setAmount(-100.00);

            EnvelopeTransaction et2a;
            et2a.setAmount(-20.00);
            et2a.setEnvelope(envelopes->getEnvelope(e3));
            et2a.setMemo("Flowers");

            EnvelopeTransaction et2b;
            et2b.setAmount(-80.00);
            et2b.setEnvelope(envelopes->getEnvelope(e1));

            Transaction t2;
            t2.setDate(QDate(2017, 2, 24));
            t2.setPayee("Grocer");

            LedgerEntry entry2(transactions);
            entry2.updateTransaction(t2);
            entry2.addSplit(at2);
            entry2.addSplit(et2a);
            entry2.addSplit(et2b);
            if (not entry2.save()) {
                qWarning() << "Transaction insertion error:" << entry2.lastError();
            }
        }

        {
            AccountTransaction at3a;
            at3a.setAccount(accounts->getAccount(a2));
            at3a.setAmount(-150.00);

            AccountTransaction at3b;
            at3b.setAccount(accounts->getAccount(a3));
            at3b.setAmount(150.00);

            Transaction t3;
            t3.setDate(QDate(2017, 2, 24));
            t3.setPayee("Cash withdrawal");

            LedgerEntry entry3(transactions);
            entry3.updateTransaction(t3);
            entry3.addSplit(at3a);
            entry3.addSplit(at3b);
            if (not entry3.save()) {
                qWarning() << "Transaction insertion error:" << entry3.lastError();
            }
        }

        {
            AccountTransaction at4;
            at4.setAccount(accounts->getAccount(a2));
            at4.setAmount(350.00);

            EnvelopeTransaction et4a;
            et4a.setAmount(100.00);
            et4a.setEnvelope(envelopes->getEnvelope(e1));

            EnvelopeTransaction et4b;
            et4b.setAmount(25.00);
            et4b.setEnvelope(envelopes->getEnvelope(e3));

            EnvelopeTransaction et4c;
            et4c.setAmount(55.00);
            et4c.setEnvelope(envelopes->getEnvelope(e2b));

            EnvelopeTransaction et4d;
            et4d.setAmount(120.00);
            et4d.setEnvelope(envelopes->getEnvelope(e2a));

            EnvelopeTransaction et4e;
            et4e.setAmount(50.00);
            et4e.setEnvelope(envelopes->getEnvelope(e4));

            Transaction t4;
            t4.setDate(QDate(2017, 2, 23));
            t4.setPayee("payday");

            LedgerEntry entry4(transactions);
            entry4.updateTransaction(t4);
            entry4.addSplit(at4);
            entry4.addSplit(et4a);
            entry4.addSplit(et4b);
            entry4.addSplit(et4c);
            entry4.addSplit(et4d);
            entry4.addSplit(et4e);
            if (not entry4.save()) {
                qWarning() << "Transaction insertion error:" << entry4.lastError();
            }
        }
    }
}

} // ub namespace
