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
#include <cstdint>
#include <memory>
#include <vector>

// Qt include(s)
#include <QtCore>
#include <QtSql>

// Google include(s)
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountRepository.hpp>
#include <ledger/model/Currency.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/model/EnvelopeRepository.hpp>
#include <ledger/model/Money.hpp>
#include <ledger/persistence/SQLTransactionRepository.hpp>

namespace ub {
namespace ledger {

/** Mock account repository */
class MockAccountRepository : public AccountRepository {
public:
    MOCK_METHOD2(create, int64_t(const Account &, const Account &));
    MOCK_METHOD1(getAccount, Account(int64_t));
    MOCK_METHOD0(getLeafAccounts, std::vector<Account>());
    MOCK_METHOD0(getRoot, Account());
    MOCK_CONST_METHOD0(lastError, QString());
    MOCK_METHOD2(move, bool(const Account &, const Account &));
    MOCK_METHOD1(remove, bool(const Account &));
    MOCK_METHOD1(update, bool(const Account &));
};

/** Mock envelope repository */
class MockEnvelopeRepository : public EnvelopeRepository {
public:
    MOCK_METHOD2(create, int64_t(const Envelope &, const Envelope &));
    MOCK_METHOD1(getEnvelope, Envelope(int64_t));
    MOCK_METHOD0(getLeafEnvelopes, std::vector<Envelope>());
    MOCK_METHOD0(getRoot, Envelope());
    MOCK_METHOD0(lastError, QString());
    MOCK_METHOD2(move, bool(const Envelope &, const Envelope &));
    MOCK_METHOD1(remove, bool(const Envelope &));
    MOCK_METHOD1(update, bool(const Envelope &));
};

/** Test fixture */
class SQLTransactionRepositoryTest : public ::testing::Test {
protected:
    /** In-memory SQL database */
    QSqlDatabase db;
    /** Mock account repository */
    std::shared_ptr<MockAccountRepository> accounts;
    /** Mock envelope repository */
    std::shared_ptr<MockEnvelopeRepository> envelopes;
    /** Transaction repository */
    std::unique_ptr<SQLTransactionRepository> repo;

    /** Sets up the in-memory database */
    void SetUp() override {
        TearDown();
        db = QSqlDatabase::addDatabase("QSQLITE", "UT");
        db.setDatabaseName(":memory:");
        ASSERT_TRUE(db.open()) << "Unable to open database";

        QSqlQuery query(db);
        query.exec("create table account(id integer primary key);");
        query.exec("insert into account(id) values(2);");
        query.exec("insert into account(id) values(4);");
        query.exec("create table envelope(id integer primary key);");
        query.exec("insert into envelope(id) values(3);");
        query.exec("insert into envelope(id) values(5);");
        query.exec("create table reconciliation(id integer primary key);");
        query.exec("insert into reconciliation(id) values(1);");
        query.exec("insert into reconciliation(id) values(2);");

        accounts.reset(new ::testing::NiceMock<MockAccountRepository>());
        envelopes.reset(new ::testing::NiceMock<MockEnvelopeRepository>());

        Account acct2(2);
        acct2.setName("Acct 2");
        acct2.setCurrency(Currency(1, "USD"));
        ON_CALL(*accounts.get(), getAccount(2)).WillByDefault(::testing::Return(acct2));

        Account acct4(4);
        acct4.setName("Acct 4");
        acct4.setCurrency(Currency(1, "USD"));
        ON_CALL(*accounts.get(), getAccount(4)).WillByDefault(::testing::Return(acct4));

        Envelope env3(3);
        env3.setName("Envelope 3");
        env3.setCurrency(Currency(1, "USD"));
        ON_CALL(*envelopes.get(), getEnvelope(3)).WillByDefault(::testing::Return(env3));

        Envelope env5(5);
        env5.setName("Envelope 5");
        env5.setCurrency(Currency(1, "USD"));
        ON_CALL(*envelopes.get(), getEnvelope(5)).WillByDefault(::testing::Return(env5));
    }

    /** Clears the in-memory database */
    void TearDown() override {
        repo.reset();
        db.close();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase("UT");
    }

    /** Sets up the transaction repository */
    void createRepo() {
        try {
            repo.reset(new SQLTransactionRepository(db, accounts, envelopes));
        } catch (std::exception & e) {
            FAIL() << "Exception thrown: " << e.what();
        }
    }

    /** Populates the database with several transactions. */
    void populate() {
        // income

        Transaction income;
        income.setDate(QDate(2016, 11, 2));
        income.setPayee("Income");
        EXPECT_EQ(1, repo->create(income)) << repo->lastError().toStdString();

        AccountTransaction income1;
        income1.setAccount(Account(2));
        income1.setAmount(Money(100.00));
        income1.setCleared(true);
        income1.setReconciliation(1);
        income1.setTransaction(Transaction(1));
        EXPECT_EQ(1, repo->create(income1)) << repo->lastError().toStdString();

        EnvelopeTransaction income2;
        income2.setEnvelope(Envelope(5));
        income2.setAmount(Money(100.00));
        income2.setTransaction(Transaction(1));
        EXPECT_EQ(1, repo->create(income2)) << repo->lastError().toStdString();

        // simple expense

        Transaction simple_expense;
        simple_expense.setDate(QDate(2016, 12, 24));
        simple_expense.setPayee("Expense");
        EXPECT_EQ(2, repo->create(simple_expense)) << repo->lastError().toStdString();

        AccountTransaction expense1;
        expense1.setAccount(Account(4));
        expense1.setAmount(Money(-24.75));
        expense1.setCleared(true);
        expense1.setReconciliation(2);
        expense1.setTransaction(Transaction(2));
        EXPECT_EQ(2, repo->create(expense1)) << repo->lastError().toStdString();

        EnvelopeTransaction expense2;
        expense2.setEnvelope(Envelope(5));
        expense2.setAmount(Money(-24.75));
        expense2.setTransaction(Transaction(2));
        EXPECT_EQ(2, repo->create(expense2)) << repo->lastError().toStdString();

        // transfer

        Transaction transfer;
        transfer.setDate(QDate(2017, 1, 15));
        transfer.setPayee("Transfer");
        EXPECT_EQ(3, repo->create(transfer)) << repo->lastError().toStdString();

        AccountTransaction transfer1;
        transfer1.setAccount(Account(2));
        transfer1.setAmount(Money(-24.75));
        transfer1.setCleared(true);
        transfer1.setTransaction(Transaction(3));
        EXPECT_EQ(3, repo->create(transfer1)) << repo->lastError().toStdString();

        AccountTransaction transfer2;
        transfer2.setAccount(Account(4));
        transfer2.setAmount(Money(24.75));
        transfer2.setCleared(true);
        transfer2.setTransaction(Transaction(3));
        EXPECT_EQ(4, repo->create(transfer2)) << repo->lastError().toStdString();

        // complex expense

        Transaction complex_expense;
        complex_expense.setDate(QDate(2017, 2, 28));
        complex_expense.setPayee("Complex");
        EXPECT_EQ(4, repo->create(complex_expense)) << repo->lastError().toStdString();

        AccountTransaction complex1;
        complex1.setAccount(Account(4));
        complex1.setAmount(Money(-32.53));
        complex1.setTransaction(Transaction(4));
        EXPECT_EQ(5, repo->create(complex1)) << repo->lastError().toStdString();

        EnvelopeTransaction complex2;
        complex2.setEnvelope(Envelope(3));
        complex2.setAmount(Money(-10.00));
        complex2.setTransaction(Transaction(4));
        EXPECT_EQ(3, repo->create(complex2)) << repo->lastError().toStdString();

        EnvelopeTransaction complex3;
        complex3.setEnvelope(Envelope(5));
        complex3.setAmount(Money(-22.53));
        complex3.setTransaction(Transaction(4));
        EXPECT_EQ(4, repo->create(complex3)) << repo->lastError().toStdString();

        // refund

        Transaction refund;
        refund.setDate(QDate(2017, 12, 7));
        refund.setPayee("Refund");
        EXPECT_EQ(5, repo->create(refund)) << repo->lastError().toStdString();

        AccountTransaction refund1;
        refund1.setAccount(Account(4));
        refund1.setAmount(Money(5.00));
        refund1.setTransaction(Transaction(5));
        EXPECT_EQ(6, repo->create(refund1)) << repo->lastError().toStdString();

        EnvelopeTransaction refund2;
        refund2.setEnvelope(Envelope(5));
        refund2.setAmount(Money(5.00));
        refund2.setTransaction(Transaction(5));
        EXPECT_EQ(5, repo->create(refund2)) << repo->lastError().toStdString();

        EXPECT_TRUE(repo->save());
    }
};

/** Verifies that the transaction tables are created when they do not already exist. */
TEST_F(SQLTransactionRepositoryTest, ShouldCreateTablesWhenTheyDoNotAlreadyExist) {
    auto tables = db.tables();
    EXPECT_FALSE(tables.contains("transaction_entry"));
    EXPECT_FALSE(tables.contains("account_transaction"));
    EXPECT_FALSE(tables.contains("envelope_transaction"));

    createRepo();

    tables = db.tables();
    EXPECT_TRUE(tables.contains("transaction_entry"));
    EXPECT_TRUE(tables.contains("account_transaction"));
    EXPECT_TRUE(tables.contains("envelope_transaction"));
}

/** Verifies that account transactions require a transaction to exist. */
TEST_F(SQLTransactionRepositoryTest,
       ShouldNotInsertAccountTransactionWhenTransactionEntryDoesNotExist) {
    createRepo();
    AccountTransaction transaction;
    transaction.setAccount(Account(2));
    transaction.setAmount(Money(13.75));
    transaction.setTransaction(Transaction(7));

    EXPECT_EQ(-1, repo->create(transaction));

    QSqlQuery query(db);
    query.exec("select count(id) from account_transaction;");
    query.first();
    auto record = query.record();
    EXPECT_EQ(0, record.value(0).toInt());
}

/** Verifies that envelope transactions require a transaction to exist. */
TEST_F(SQLTransactionRepositoryTest,
       ShouldNotInsertEnvelopeTransactionWhenTransactionEntryDoesNotExist) {
    createRepo();
    EnvelopeTransaction transaction;
    transaction.setEnvelope(Envelope(3));
    transaction.setAmount(Money(13.75));
    transaction.setTransaction(Transaction(7));

    EXPECT_EQ(-1, repo->create(transaction));

    QSqlQuery query(db);
    query.exec("select count(id) from envelope_transaction;");
    query.first();
    auto record = query.record();
    EXPECT_EQ(0, record.value(0).toInt());
}

/** Verifies that an invalid account transaction is returned for an invalid ID. */
TEST_F(SQLTransactionRepositoryTest, ShouldReturnInvalidAccountTransactionWhenIDDoesNotExist) {
    createRepo();
    populate();
    AccountTransaction transaction = repo->getAccountTransaction(42);
    EXPECT_EQ(-1, transaction.id());
}

/** Verifies that reconciled account transactions can be retrieved. */
TEST_F(SQLTransactionRepositoryTest, ShouldRetrieveReconciledAccountTransaction) {
    createRepo();
    populate();
    AccountTransaction transaction = repo->getAccountTransaction(2);
    EXPECT_EQ(2, transaction.id()) << repo->lastError().toStdString();
    EXPECT_EQ(4, transaction.account().id());
    EXPECT_EQ(Money(-24.75, Currency(1, "USD")), transaction.amount());
    EXPECT_TRUE(transaction.isCleared());
    EXPECT_EQ(2, transaction.reconciliation());
    EXPECT_EQ(2, transaction.transaction().id());
    EXPECT_EQ(QDate(2016, 12, 24), transaction.transaction().date());
}

/** Verifies that unreconciled account transactions can be retrieved. */
TEST_F(SQLTransactionRepositoryTest, ShouldRetrieveUnreconciledAccountTransaction) {
    createRepo();
    populate();
    AccountTransaction transaction = repo->getAccountTransaction(5);
    EXPECT_EQ(5, transaction.id()) << repo->lastError().toStdString();
    EXPECT_EQ(4, transaction.account().id());
    EXPECT_EQ(Money(-32.53, Currency(1, "USD")), transaction.amount());
    EXPECT_FALSE(transaction.isCleared());
    EXPECT_EQ(-1, transaction.reconciliation());
    EXPECT_EQ(4, transaction.transaction().id());
    EXPECT_EQ(QDate(2017, 2, 28), transaction.transaction().date());
}

/** Verifies that an invalid envelope transaction is returned for an invalid ID. */
TEST_F(SQLTransactionRepositoryTest, ShouldReturnInvalidEnvelopeTransactionWhenIDDoesNotExist) {
    createRepo();
    populate();
    EnvelopeTransaction transaction = repo->getEnvelopeTransaction(42);
    EXPECT_EQ(-1, transaction.id());
}

/** Verifies that envelope transactions can be retrieved. */
TEST_F(SQLTransactionRepositoryTest, ShouldRetrieveEnvelopeTransaction) {
    createRepo();
    populate();
    EnvelopeTransaction transaction = repo->getEnvelopeTransaction(3);
    EXPECT_EQ(3, transaction.id()) << repo->lastError().toStdString();
    EXPECT_EQ(3, transaction.envelope().id());
    EXPECT_EQ(Money(-10.00, Currency(1, "USD")), transaction.amount());
    EXPECT_EQ(4, transaction.transaction().id());
    EXPECT_EQ(QDate(2017, 2, 28), transaction.transaction().date());
}

/** Verifies that an invalid double-entry transaction is returned for an invalid ID. */
TEST_F(SQLTransactionRepositoryTest, ShouldReturnInvalidTransactionWhenIDDoesNotExist) {
    createRepo();
    populate();
    Transaction transaction = repo->getTransaction(42);
    EXPECT_EQ(-1, transaction.id());
}

/** Verifies that double-entry transactions can be retrieved. */
TEST_F(SQLTransactionRepositoryTest, ShouldRetrieveTransaction) {
    createRepo();
    populate();
    Transaction transaction = repo->getTransaction(5);
    EXPECT_EQ(5, transaction.id()) << repo->lastError().toStdString();
    EXPECT_EQ(QDate(2017, 12, 7), transaction.date());
    EXPECT_EQ(QString("Refund"), transaction.payee());
}

/** Verifies that an empty transaction list is returned for an invalid account. */
TEST_F(SQLTransactionRepositoryTest, ShouldReturnEmptyListWhenAccountIsInvalid) {
    createRepo();
    populate();
    auto list = repo->getTransactions(Account(42));
    EXPECT_TRUE(list.empty());
}

/** Verifies that all transactions are returned for an account. */
TEST_F(SQLTransactionRepositoryTest, ShouldReturnAllTransactionsAgainstAccount) {
    createRepo();
    populate();
    EXPECT_EQ(2u, repo->getTransactions(Account(2)).size()) << repo->lastError().toStdString();
    EXPECT_EQ(4u, repo->getTransactions(Account(4)).size()) << repo->lastError().toStdString();
}

/** Verifies that transactions are returned in chronological order for an account. */
TEST_F(SQLTransactionRepositoryTest, ShouldReturnTransactionsAgainstAccountSortedByDate) {
    createRepo();
    populate();
    auto list = repo->getTransactions(Account(4));
    ASSERT_EQ(4u, list.size()) << repo->lastError().toStdString();
    EXPECT_EQ(2, list.at(0).id());
    EXPECT_EQ(QDate(2016, 12, 24), list.at(0).transaction().date());
    EXPECT_EQ(4, list.at(1).id());
    EXPECT_EQ(QDate(2017, 1, 15), list.at(1).transaction().date());
    EXPECT_EQ(5, list.at(2).id());
    EXPECT_EQ(QDate(2017, 2, 28), list.at(2).transaction().date());
    EXPECT_EQ(6, list.at(3).id());
    EXPECT_EQ(QDate(2017, 12, 7), list.at(3).transaction().date());
}

/** Verifies that transactions contain the appropriate balances. */
TEST_F(SQLTransactionRepositoryTest, ShouldReturnTransactionsWithBalances) {
    createRepo();
    populate();
    auto list = repo->getTransactions(Account(4));
    ASSERT_EQ(4u, list.size()) << repo->lastError().toStdString();
    EXPECT_EQ(2, list.at(0).id());
    EXPECT_EQ(Money(-24.75), list.at(0).balance());
    EXPECT_EQ(4, list.at(1).id());
    EXPECT_EQ(Money(), list.at(1).balance());
    EXPECT_EQ(5, list.at(2).id());
    EXPECT_EQ(Money(-32.53), list.at(2).balance());
    EXPECT_EQ(6, list.at(3).id());
    EXPECT_EQ(Money(-27.53), list.at(3).balance());
}

/** Verifies that an empty transaction list is returned for an invalid envelope. */
TEST_F(SQLTransactionRepositoryTest, ShouldReturnEmptyListWhenEnvelopeIsInvalid) {
    createRepo();
    populate();
    auto list = repo->getTransactions(Envelope(42));
    EXPECT_TRUE(list.empty());
}

/** Verifies that all transactions are returned for an envelope. */
TEST_F(SQLTransactionRepositoryTest, ShouldReturnAllTransactionsAgainstEnvelope) {
    createRepo();
    populate();
    EXPECT_EQ(1u, repo->getTransactions(Envelope(3)).size()) << repo->lastError().toStdString();
    EXPECT_EQ(4u, repo->getTransactions(Envelope(5)).size()) << repo->lastError().toStdString();
}

/** Verifies that transactions are returned in reverse-chronological order for an envelope. */
TEST_F(SQLTransactionRepositoryTest, ShouldReturnTransactionsAgainstEnvelopeSortedByDate) {
    createRepo();
    populate();
    auto list = repo->getTransactions(Envelope(5));
    ASSERT_EQ(4u, list.size()) << repo->lastError().toStdString();
    EXPECT_EQ(1, list.at(0).id());
    EXPECT_EQ(QDate(2016, 11, 2), list.at(0).transaction().date());
    EXPECT_EQ(2, list.at(1).id());
    EXPECT_EQ(QDate(2016, 12, 24), list.at(1).transaction().date());
    EXPECT_EQ(4, list.at(2).id());
    EXPECT_EQ(QDate(2017, 2, 28), list.at(2).transaction().date());
    EXPECT_EQ(5, list.at(3).id());
    EXPECT_EQ(QDate(2017, 12, 7), list.at(3).transaction().date());
}

/** Verifies that envelope balances can be retrieved. */
TEST_F(SQLTransactionRepositoryTest, ShouldCalculateEnvelopeBalances) {
    createRepo();
    populate();
    auto list = repo->getTransactions(Envelope(5));
    ASSERT_EQ(4u, list.size()) << repo->lastError().toStdString();
    EXPECT_EQ(1, list.at(0).id());
    EXPECT_EQ(Money(100.00), list.at(0).balance());
    EXPECT_EQ(2, list.at(1).id());
    EXPECT_EQ(Money(75.25), list.at(1).balance());
    EXPECT_EQ(4, list.at(2).id());
    EXPECT_EQ(Money(52.72), list.at(2).balance());
    EXPECT_EQ(5, list.at(3).id());
    EXPECT_EQ(Money(57.72), list.at(3).balance());
}

/** Verifies that double-entry transactions can be removed. */
TEST_F(SQLTransactionRepositoryTest, ShouldRemoveTransaction) {
    createRepo();
    populate();
    ASSERT_TRUE(repo->remove(Transaction(2))) << repo->lastError().toStdString();
    EXPECT_EQ(-1, repo->getTransaction(2).id());
    EXPECT_EQ(3u, repo->getTransactions(Account(4)).size());
    EXPECT_EQ(3u, repo->getTransactions(Envelope(5)).size());
}

/** Verifies that account transactions can be removed. */
TEST_F(SQLTransactionRepositoryTest, ShouldRemoveAccountTransaction) {
    createRepo();
    populate();
    ASSERT_TRUE(repo->remove(AccountTransaction(2))) << repo->lastError().toStdString();
    EXPECT_EQ(-1, repo->getAccountTransaction(2).id());
}

/** Verifies that envelope transactions can be removed. */
TEST_F(SQLTransactionRepositoryTest, ShouldRemoveEnvelopeTransaction) {
    createRepo();
    populate();
    ASSERT_TRUE(repo->remove(EnvelopeTransaction(2))) << repo->lastError().toStdString();
    EXPECT_EQ(-1, repo->getEnvelopeTransaction(2).id());
}

/** Verifies that double-entry transactions can be modified. */
TEST_F(SQLTransactionRepositoryTest, ShouldUpdateTransaction) {
    createRepo();
    populate();
    Transaction modified(5);
    modified.setDate(QDate(2017, 4, 7));
    modified.setPayee("Modified");
    ASSERT_TRUE(repo->update(modified)) << repo->lastError().toStdString();

    auto mod = repo->getTransaction(5);
    EXPECT_EQ(QDate(2017, 4, 7), mod.date());
    EXPECT_EQ(QString("Modified"), mod.payee());
}

/** Verifies that account transactions can be modified. */
TEST_F(SQLTransactionRepositoryTest, ShouldUpdateAccountTransaction) {
    createRepo();
    populate();
    AccountTransaction modified(6);
    modified.setAccount(Account(2));
    modified.setAmount(Money(6.50));
    modified.setCleared(true);
    modified.setMemo("Modified");
    modified.setReconciliation(2);
    modified.setTransaction(Transaction(42));
    ASSERT_TRUE(repo->update(modified)) << repo->lastError().toStdString();

    auto mod = repo->getAccountTransaction(6);
    EXPECT_EQ(2, mod.account().id());
    EXPECT_EQ(Money(6.50, Currency(1, "USD")), mod.amount());
    EXPECT_TRUE(mod.isCleared());
    EXPECT_EQ(QString("Modified"), mod.memo());
    EXPECT_EQ(2, mod.reconciliation());
    EXPECT_EQ(5, mod.transaction().id());
}

/** Verifies that envelope transactions can be modified. */
TEST_F(SQLTransactionRepositoryTest, ShouldUpdateEnvelopeTransaction) {
    createRepo();
    populate();
    EnvelopeTransaction modified(5);
    modified.setAmount(Money(6.50));
    modified.setEnvelope(Envelope(3));
    modified.setMemo("Modified");
    modified.setTransaction(Transaction(42));
    ASSERT_TRUE(repo->update(modified)) << repo->lastError().toStdString();

    auto mod = repo->getEnvelopeTransaction(5);
    EXPECT_EQ(3, mod.envelope().id());
    EXPECT_EQ(Money(6.50, Currency(1, "USD")), mod.amount());
    EXPECT_EQ(QString("Modified"), mod.memo());
    EXPECT_EQ(5, mod.transaction().id());
}

/** Verifies that account balances can be retrieved. */
TEST_F(SQLTransactionRepositoryTest, ShouldCalculateAccountBalances) {
    createRepo();
    populate();

    Currency c(1, "USD");
    Account a2(2);
    a2.setCurrency(c);
    Account a4(4);
    a4.setCurrency(c);

    EXPECT_EQ(Money(0.0, c), repo->getBalance(QDate(2016, 11, 1), a2));
    EXPECT_EQ(Money(0.0, c), repo->getBalance(QDate(2016, 11, 1), a4));

    EXPECT_EQ(Money(100.0, c), repo->getBalance(QDate(2016, 11, 2), a2));
    EXPECT_EQ(Money(0.0, c), repo->getBalance(QDate(2016, 11, 2), a4));

    EXPECT_EQ(Money(100.0, c), repo->getBalance(QDate(2016, 12, 24), a2));
    EXPECT_EQ(Money(-24.75, c), repo->getBalance(QDate(2016, 12, 24), a4));

    EXPECT_EQ(Money(75.25, c), repo->getBalance(QDate(2017, 1, 15), a2));
    EXPECT_EQ(Money(0.0, c), repo->getBalance(QDate(2017, 1, 15), a4));

    EXPECT_EQ(Money(75.25, c), repo->getBalance(QDate(2017, 2, 28), a2));
    EXPECT_EQ(Money(-32.53, c), repo->getBalance(QDate(2017, 2, 28), a4));

    EXPECT_EQ(Money(75.25, c), repo->getBalance(QDate(2017, 12, 7), a2));
    EXPECT_EQ(Money(-27.53, c), repo->getBalance(QDate(2017, 12, 7), a4));

    EXPECT_EQ(Money(75.25, c), repo->getBalance(QDate(2017, 12, 8), a2));
    EXPECT_EQ(Money(-27.53, c), repo->getBalance(QDate(2017, 12, 8), a4));
}

/** Verifies that envelope balances can be retrieved. */
TEST_F(SQLTransactionRepositoryTest, ShouldCalculateEnvelopeBalance) {
    createRepo();
    populate();

    Currency c(1, "USD");
    Envelope e3(3);
    e3.setCurrency(c);
    Envelope e5(5);
    e5.setCurrency(c);

    EXPECT_EQ(Money(0.0, c), repo->getBalance(QDate(2016, 11, 1), e3));
    EXPECT_EQ(Money(0.0, c), repo->getBalance(QDate(2016, 11, 1), e5));

    EXPECT_EQ(Money(0.0, c), repo->getBalance(QDate(2016, 11, 2), e3));
    EXPECT_EQ(Money(100.0, c), repo->getBalance(QDate(2016, 11, 2), e5));

    EXPECT_EQ(Money(0.0, c), repo->getBalance(QDate(2016, 12, 24), e3));
    EXPECT_EQ(Money(75.25, c), repo->getBalance(QDate(2016, 12, 24), e5));

    EXPECT_EQ(Money(0.0, c), repo->getBalance(QDate(2017, 1, 15), e3));
    EXPECT_EQ(Money(75.25, c), repo->getBalance(QDate(2017, 1, 15), e5));

    EXPECT_EQ(Money(-10.0, c), repo->getBalance(QDate(2017, 2, 28), e3));
    EXPECT_EQ(Money(52.72, c), repo->getBalance(QDate(2017, 2, 28), e5));

    EXPECT_EQ(Money(-10.0, c), repo->getBalance(QDate(2017, 12, 7), e3));
    EXPECT_EQ(Money(57.72, c), repo->getBalance(QDate(2017, 12, 7), e5));

    EXPECT_EQ(Money(-10.0, c), repo->getBalance(QDate(2017, 12, 8), e3));
    EXPECT_EQ(Money(57.72, c), repo->getBalance(QDate(2017, 12, 8), e5));
}

/** Verifies that reconciled account balances can be retrieved. */
TEST_F(SQLTransactionRepositoryTest, ShouldCalculateReconciledAccountBalance) {
    createRepo();
    populate();

    Currency c(1, "USD");
    Account a2(2);
    a2.setCurrency(c);
    Account a4(4);
    a4.setCurrency(c);

    EXPECT_EQ(Money(0.0, c), repo->getReconciledBalance(QDate(2016, 11, 1), a2));
    EXPECT_EQ(Money(0.0, c), repo->getReconciledBalance(QDate(2016, 11, 1), a4));

    EXPECT_EQ(Money(100.0, c), repo->getReconciledBalance(QDate(2016, 11, 2), a2));
    EXPECT_EQ(Money(0.0, c), repo->getReconciledBalance(QDate(2016, 11, 2), a4));

    EXPECT_EQ(Money(100.0, c), repo->getReconciledBalance(QDate(2016, 12, 24), a2));
    EXPECT_EQ(Money(-24.75, c), repo->getReconciledBalance(QDate(2016, 12, 24), a4));

    EXPECT_EQ(Money(100.0, c), repo->getReconciledBalance(QDate(2017, 1, 15), a2));
    EXPECT_EQ(Money(-24.75, c), repo->getReconciledBalance(QDate(2017, 1, 15), a4));

    EXPECT_EQ(Money(100.0, c), repo->getReconciledBalance(QDate(2017, 2, 28), a2));
    EXPECT_EQ(Money(-24.75, c), repo->getReconciledBalance(QDate(2017, 2, 28), a4));

    EXPECT_EQ(Money(100.0, c), repo->getReconciledBalance(QDate(2017, 12, 7), a2));
    EXPECT_EQ(Money(-24.75, c), repo->getReconciledBalance(QDate(2017, 12, 7), a4));

    EXPECT_EQ(Money(100.0, c), repo->getReconciledBalance(QDate(2017, 12, 8), a2));
    EXPECT_EQ(Money(-24.75, c), repo->getReconciledBalance(QDate(2017, 12, 8), a4));
}

/** Verifies that account transactions can be retrieved for a transaction. */
TEST_F(SQLTransactionRepositoryTest, ShouldRetrieveAccountSplitsForATransaction) {
    createRepo();
    populate();

    auto splits = repo->getAccountTransactions(Transaction(3));
    ASSERT_EQ(2u, splits.size());
    EXPECT_EQ(3, splits.at(0).id());
    EXPECT_EQ(4, splits.at(1).id());
}

/** Verifies that envelope transactions can be retrieved for a transaction. */
TEST_F(SQLTransactionRepositoryTest, ShouldRetrieveEnvelopeSplitsForATransaction) {
    createRepo();
    populate();

    auto splits = repo->getEnvelopeTransactions(Transaction(4));
    ASSERT_EQ(2u, splits.size());
    EXPECT_EQ(3, splits.at(0).id());
    EXPECT_EQ(4, splits.at(1).id());
}

} // ledger namespace
} // ub namespace
