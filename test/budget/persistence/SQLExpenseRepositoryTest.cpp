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
#include <budget/model/Expense.hpp>
#include <budget/model/ExpenseRepository.hpp>
#include <budget/persistence/SQLExpenseRepository.hpp>
#include <ledger/model/Currency.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/model/EnvelopeRepository.hpp>
#include <ledger/model/Money.hpp>

namespace ub {
namespace budget {

using ledger::Currency;
using ledger::Envelope;
using ledger::Money;

/** Mock envelope repository */
class MockEnvelopeRepository : public ledger::EnvelopeRepository {
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
class SQLExpenseRepositoryTest : public ::testing::Test {
protected:
    /** In-memory SQL database */
    QSqlDatabase db;
    /** Mock envelope repository */
    std::shared_ptr<MockEnvelopeRepository> envelopes;
    /** Expense repository */
    std::unique_ptr<SQLExpenseRepository> repo;

    /** Sets up the in-memory database */
    void SetUp() override {
        TearDown();
        db = QSqlDatabase::addDatabase("QSQLITE", "UT");
        db.setDatabaseName(":memory:");
        ASSERT_TRUE(db.open()) << "Unable to open database";

        QSqlQuery query(db);
        query.exec("create table envelope(id integer primary key);");
        query.exec("insert into envelope(id) values(2);");
        query.exec("insert into envelope(id) values(4);");

        envelopes.reset(new ::testing::NiceMock<MockEnvelopeRepository>());

        Envelope env2(2);
        env2.setName("Env 2");
        env2.setCurrency(Currency(1, "USD"));
        ON_CALL(*envelopes.get(), getEnvelope(2)).WillByDefault(::testing::Return(env2));

        Envelope env4(4);
        env4.setName("Env 4");
        env4.setCurrency(Currency(1, "USD"));
        ON_CALL(*envelopes.get(), getEnvelope(4)).WillByDefault(::testing::Return(env4));
    }

    /** Clears the in-memory database */
    void TearDown() override {
        repo.reset();
        db.close();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase("UT");
    }

    /** Sets up the expense repository */
    void createRepo() {
        try {
            repo.reset(new SQLExpenseRepository(db, envelopes));
        } catch (std::exception & e) {
            FAIL() << "Exception thrown: " << e.what();
        }
    }

    /** Populates the database with several expenses. */
    void populate() {
        Expense e1;
        e1.setAmount(Money(100.00));
        e1.setDescription("Rent");
        e1.setEnvelope(Envelope(4));
        e1.setBeginningDate(QDate(2016, 2, 1));
        e1.setEndingDate(QDate(2017, 1, 11));
        Recurrence r1;
        r1.setDay(5);
        r1.setPeriodicity(1);
        r1.setScope(Recurrence::Monthly);
        e1.setRecurrence(r1);
        EXPECT_EQ(1, repo->create(e1)) << repo->lastError().toStdString();

        Expense e2;
        e2.setAmount(Money(1200.00));
        e2.setDescription("Bill");
        e2.setEnvelope(Envelope(2));
        e2.setBeginningDate(QDate(2016, 7, 1));
        e2.setEndingDate(QDate(2016, 7, 31));
        EXPECT_EQ(2, repo->create(e2)) << repo->lastError().toStdString();

        Expense e3;
        e3.setAmount(Money(110.00));
        e3.setDescription("Rent");
        e3.setEnvelope(Envelope(4));
        e3.setBeginningDate(QDate(2017, 1, 11));
        e3.setRecurrence(r1);
        EXPECT_EQ(3, repo->create(e3)) << repo->lastError().toStdString();
    }
};

/** Verifies that the expense table is created when it does not already exist. */
TEST_F(SQLExpenseRepositoryTest, ShouldCreateTableWhenItDoesNotAlreadyExist) {
    auto tables = db.tables();
    ASSERT_FALSE(tables.contains("budget_expense"));

    createRepo();

    tables = db.tables();
    ASSERT_TRUE(tables.contains("budget_expense"));
}

/** Verifies that expenses require an envelope to exist. */
TEST_F(SQLExpenseRepositoryTest, ShouldNotInsertExpenseWhenEnvelopeDoesNotExist) {
    createRepo();
    Expense expense;
    expense.setEnvelope(Envelope(3));

    EXPECT_EQ(-1, repo->create(expense));

    QSqlQuery query(db);
    query.exec("select count(id) from budget_expense;");
    query.first();
    auto record = query.record();
    EXPECT_EQ(0, record.value(0).toInt());
}

/** Verifies that an invalid expense is returned for an invalid ID. */
TEST_F(SQLExpenseRepositoryTest, ShouldReturnInvalidExpenseWhenIDDoesNotExist) {
    createRepo();
    populate();
    Expense record = repo->expense(42);
    EXPECT_EQ(-1, record.id());
}

/** Verifies that expense can be retrieved by ID. */
TEST_F(SQLExpenseRepositoryTest, ShouldReturnValidExpenseWhenIDExists) {
    createRepo();
    populate();
    Expense record = repo->expense(3);
    EXPECT_EQ(3, record.id()) << repo->lastError().toStdString();
    EXPECT_EQ(Money(110.00, Currency(1, "USD")), record.amount());
    EXPECT_EQ(QString("Rent"), record.description());
    EXPECT_EQ(4, record.envelope().id());
    EXPECT_EQ(QDate(2017, 1, 11), record.beginningDate());
    EXPECT_FALSE(record.endingDate().isValid());

    Recurrence rec = record.recurrence();
    EXPECT_EQ(5, rec.day());
    EXPECT_EQ(0, rec.month());
    EXPECT_EQ(1, rec.periodicity());
    EXPECT_EQ(Recurrence::Monthly, rec.scope());
    EXPECT_EQ(0, rec.week());
}

/** Verifies that one-time expense can be retrieved. */
TEST_F(SQLExpenseRepositoryTest, ShouldReturnValidOneTimeExpense) {
    createRepo();
    populate();
    Expense record = repo->expense(2);
    EXPECT_EQ(2, record.id());
    EXPECT_EQ(Money(1200.00, Currency(1, "USD")), record.amount());
    EXPECT_EQ(QString("Bill"), record.description());
    EXPECT_EQ(2, record.envelope().id());
    EXPECT_EQ(QDate(2016, 7, 1), record.beginningDate());
    EXPECT_EQ(QDate(2016, 7, 31), record.endingDate());

    Recurrence rec = record.recurrence();
    EXPECT_EQ(0, rec.day());
    EXPECT_EQ(0, rec.month());
    EXPECT_EQ(0, rec.periodicity());
    EXPECT_EQ(Recurrence::Yearly, rec.scope());
    EXPECT_EQ(0, rec.week());
}

/** Verifies that an empty expense list is returned for an invalid envelope. */
TEST_F(SQLExpenseRepositoryTest, ShouldReturnEmptyListWhenEnvelopeIsInvalid) {
    createRepo();
    populate();
    auto list = repo->expenses(Envelope(42));
    EXPECT_TRUE(list.empty());
}

/** Verifies that expenses are returned for an envelope in chronological order. */
TEST_F(SQLExpenseRepositoryTest, ShouldReturnExpensesAgainstEnvelopeSortedByDate) {
    createRepo();
    populate();
    auto list = repo->expenses(Envelope(4));
    ASSERT_EQ(2u, list.size()) << repo->lastError().toStdString();
    EXPECT_EQ(1, list.at(0).id());
    EXPECT_EQ(QDate(2016, 2, 1), list.at(0).beginningDate());
    EXPECT_EQ(3, list.at(1).id());
    EXPECT_EQ(QDate(2017, 1, 11), list.at(1).beginningDate());
}

/** Verifies that expenses can be removed. */
TEST_F(SQLExpenseRepositoryTest, ShouldRemoveExpenseFromDatabase) {
    createRepo();
    populate();
    ASSERT_TRUE(repo->remove(Expense(3))) << repo->lastError().toStdString();
    EXPECT_EQ(-1, repo->expense(3).id());
    EXPECT_EQ(1u, repo->expenses(Envelope(4)).size());
}

/** Verifies that expenses can be modified. */
TEST_F(SQLExpenseRepositoryTest, ShouldUpdateExpensesInDatabase) {
    createRepo();
    populate();
    Expense record(3);
    record.setEnvelope(Envelope(2));
    record.setAmount(Money(999.99));
    record.setBeginningDate(QDate(2017, 1, 1));
    record.setEndingDate(QDate(2017, 1, 31));
    Recurrence rec;
    rec.setPeriodicity(2);
    rec.setScope(Recurrence::Weekly);
    record.setRecurrence(rec);
    ASSERT_TRUE(repo->update(record)) << repo->lastError().toStdString();

    auto mod = repo->expense(3);
    EXPECT_EQ(2, mod.envelope().id());
    EXPECT_EQ(Money(999.99, Currency(1, "USD")), mod.amount());
    EXPECT_EQ(QDate(2017, 1, 1), mod.beginningDate());
    EXPECT_EQ(QDate(2017, 1, 31), mod.endingDate());
    EXPECT_EQ(Recurrence::Weekly, mod.recurrence().scope());
}

} // budget namespace
} // ub namespace
