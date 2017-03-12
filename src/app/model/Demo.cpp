/*
 * Copyright 2017 Kyle Treubig
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Standard include(s)
#include <memory>

// Qt include(s)
#include <QDebug>

// UnderBudget include(s)
#include <app/model/Repositories.hpp>
#include <budget/model/Expense.hpp>
#include <budget/model/Income.hpp>
#include <budget/model/Recurrence.hpp>
#include <ledger/model/Account.hpp>
#include <ledger/model/Currency.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/model/JournalEntry.hpp>
#include <ledger/model/Transaction.hpp>
#include "Demo.hpp"

namespace ub {

//--------------------------------------------------------------------------------------------------
void Demo::populate(std::shared_ptr<Repositories> repos) {
    using namespace ub::budget;
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

        qDebug() << "Account insertion error (if any):" << accounts->lastError();

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

        qDebug() << "Envelope insertion error (if any):" << envelopes->lastError();

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

            JournalEntry entry1(transactions);
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

            JournalEntry entry2(transactions);
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

            JournalEntry entry3(transactions);
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

            JournalEntry entry4(transactions);
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

        // -- Budgeted expenses

        auto expenses = repos->expenses();

        Recurrence r1;
        r1.setPeriodicity(1);
        r1.setScope(Recurrence::Weekly);
        Expense exp1;
        exp1.setAmount(100.0);
        exp1.setBeginningDate(QDate(2016, 1, 1));
        exp1.setDescription("Groceries");
        exp1.setEnvelope(envelopes->getEnvelope(e1));
        exp1.setRecurrence(r1);
        expenses->create(exp1);

        Recurrence r2;
        r2.setMonth(6);
        r2.setPeriodicity(1);
        r2.setScope(Recurrence::Yearly);
        Expense exp2;
        exp2.setAmount(25.0);
        exp2.setBeginningDate(QDate(2014, 6, 1));
        exp2.setDescription("John's birthday");
        exp2.setEnvelope(envelopes->getEnvelope(e3));
        exp2.setRecurrence(r2);
        expenses->create(exp2);

        Recurrence r3;
        r3.setDay(1);
        r3.setPeriodicity(1);
        r3.setScope(Recurrence::Monthly);
        Expense exp3;
        exp3.setAmount(110.0);
        exp3.setBeginningDate(QDate(2014, 1, 1));
        exp3.setEndingDate(QDate(2016, 12, 1));
        exp3.setDescription("Rent");
        exp3.setEnvelope(envelopes->getEnvelope(e2a));
        exp3.setRecurrence(r3);
        expenses->create(exp3);

        Recurrence r4;
        r4.setDay(1);
        r4.setPeriodicity(1);
        r4.setScope(Recurrence::Monthly);
        Expense exp4;
        exp4.setAmount(120.0);
        exp4.setBeginningDate(QDate(2017, 1, 1));
        exp4.setDescription("Rent");
        exp4.setEnvelope(envelopes->getEnvelope(e2a));
        exp4.setRecurrence(r4);
        expenses->create(exp4);

        for (int i = 1; i < 6; ++i) {
            Recurrence r5;
            r5.setMonth(i);
            r5.setPeriodicity(1);
            r5.setScope(Recurrence::Yearly);
            Expense exp5;
            exp5.setAmount(50.0);
            exp5.setBeginningDate(QDate(2014, 1, 1));
            exp5.setDescription("Winter electric bill");
            exp5.setEnvelope(envelopes->getEnvelope(e2b));
            exp5.setRecurrence(r5);
            expenses->create(exp5);
        }
        for (int i = 6; i < 9; ++i) {
            Recurrence r5;
            r5.setMonth(i);
            r5.setPeriodicity(1);
            r5.setScope(Recurrence::Yearly);
            Expense exp5;
            exp5.setAmount(100.0);
            exp5.setBeginningDate(QDate(2014, 1, 1));
            exp5.setDescription("Summer electric bill");
            exp5.setEnvelope(envelopes->getEnvelope(e2b));
            exp5.setRecurrence(r5);
            expenses->create(exp5);
        }
        for (int i = 9; i < 13; ++i) {
            Recurrence r5;
            r5.setMonth(i);
            r5.setPeriodicity(1);
            r5.setScope(Recurrence::Yearly);
            Expense exp5;
            exp5.setAmount(70.0);
            exp5.setBeginningDate(QDate(2014, 1, 1));
            exp5.setDescription("Fall electric bill");
            exp5.setEnvelope(envelopes->getEnvelope(e2b));
            exp5.setRecurrence(r5);
            expenses->create(exp5);
        }

        Expense exp6;
        exp6.setAmount(250.0);
        exp6.setBeginningDate(QDate(2016, 8, 1));
        exp6.setDescription("Parent's 10th anniversary");
        exp6.setEndingDate(QDate(2016, 8, 31));
        exp6.setEnvelope(envelopes->getEnvelope(e3));
        expenses->create(exp6);

        Recurrence r7;
        r7.setPeriodicity(2);
        r7.setScope(Recurrence::Weekly);
        Expense exp7;
        exp7.setAmount(15.0);
        exp7.setBeginningDate(QDate(2016, 11, 10));
        exp7.setDescription("Dinner with friends");
        exp7.setEnvelope(envelopes->getEnvelope(e1));
        exp7.setRecurrence(r7);
        expenses->create(exp7);

        // -- Budgeted incomes

        auto incomes = repos->incomes();

        Recurrence r8;
        r8.setPeriodicity(2);
        r8.setScope(Recurrence::Weekly);
        r8.setDay(5);
        Income inc1;
        inc1.setAccount(accounts->getAccount(a2));
        inc1.setAmount(350.0);
        inc1.setBeginningDate(QDate(2012, 7, 21));
        inc1.setDescription("Payday");
        inc1.setRecurrence(r8);
        incomes->create(inc1);
    }
}

} // ub namespace
