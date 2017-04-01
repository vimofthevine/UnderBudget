/*
 * UnderBudget
 * Copyright 2017 Kyle Treubig
 *
 * UnderBudget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UnderBudget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UnderBudget.  If not, see <http://www.gnu.org/licenses/>.
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
#include <budget/model/Income.hpp>
#include <budget/model/IncomeRepository.hpp>
#include <budget/persistence/SQLIncomeRepository.hpp>
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountRepository.hpp>
#include <ledger/model/Currency.hpp>
#include <ledger/model/Money.hpp>

namespace ub {
namespace budget {

using ledger::Account;
using ledger::Currency;
using ledger::Money;

/** Mock account repository */
class MockAccountRepository : public ledger::AccountRepository {
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

/** Test fixture */
class SQLIncomeRepositoryTest : public ::testing::Test {
protected:
    /** In-memory SQL database */
    QSqlDatabase db;
    /** Mock account repository */
    std::shared_ptr<MockAccountRepository> accounts;
    /** Income repository */
    std::unique_ptr<SQLIncomeRepository> repo;

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

        accounts.reset(new ::testing::NiceMock<MockAccountRepository>());

        Account acct2(2);
        acct2.setName("Acct 2");
        acct2.setCurrency(Currency(1, "USD"));
        ON_CALL(*accounts.get(), getAccount(2)).WillByDefault(::testing::Return(acct2));

        Account acct4(4);
        acct4.setName("Acct 4");
        acct4.setCurrency(Currency(1, "USD"));
        ON_CALL(*accounts.get(), getAccount(4)).WillByDefault(::testing::Return(acct4));
    }

    /** Clears the in-memory database */
    void TearDown() override {
        repo.reset();
        db.close();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase("UT");
    }

    /** Sets up the income repository */
    void createRepo() {
        try {
            repo.reset(new SQLIncomeRepository(db, accounts));
        } catch (std::exception & e) {
            FAIL() << "Exception thrown: " << e.what();
        }
    }

    /** Populates the database with several incomes. */
    void populate() {
        Income e1;
        e1.setAccount(Account(4));
        e1.setAmount(Money(100.00));
        e1.setDescription("Rent");
        e1.setBeginningDate(QDate(2016, 2, 1));
        e1.setEndingDate(QDate(2017, 1, 9));
        Recurrence r1;
        r1.setDay(5);
        r1.setPeriodicity(1);
        r1.setScope(Recurrence::Monthly);
        e1.setRecurrence(r1);
        EXPECT_EQ(1, repo->create(e1)) << repo->lastError().toStdString();

        Income e2;
        e2.setAccount(Account(2));
        e2.setAmount(Money(1200.00));
        e2.setDescription("Bill");
        e2.setBeginningDate(QDate(2016, 7, 1));
        e2.setEndingDate(QDate(2016, 7, 31));
        EXPECT_EQ(2, repo->create(e2)) << repo->lastError().toStdString();

        Income e3;
        e3.setAccount(Account(4));
        e3.setAmount(Money(110.00));
        e3.setDescription("Rent");
        e3.setBeginningDate(QDate(2017, 1, 11));
        e3.setRecurrence(r1);
        EXPECT_EQ(3, repo->create(e3)) << repo->lastError().toStdString();
    }
};

/** Verifies that the income table is created when it does not already exist. */
TEST_F(SQLIncomeRepositoryTest, ShouldCreateTableWhenItDoesNotAlreadyExist) {
    auto tables = db.tables();
    ASSERT_FALSE(tables.contains("budget_income"));

    createRepo();

    tables = db.tables();
    ASSERT_TRUE(tables.contains("budget_income"));
}

/** Verifies that incomes require an account to exist. */
TEST_F(SQLIncomeRepositoryTest, ShouldNotInsertIncomeWhenAccountDoesNotExist) {
    createRepo();
    Income income;
    income.setAccount(Account(3));

    EXPECT_EQ(-1, repo->create(income));

    QSqlQuery query(db);
    query.exec("select count(id) from budget_income;");
    query.first();
    auto record = query.record();
    EXPECT_EQ(0, record.value(0).toInt());
}

/** Verifies that an invalid income is returned for an invalid ID. */
TEST_F(SQLIncomeRepositoryTest, ShouldReturnInvalidIncomeWhenIDDoesNotExist) {
    createRepo();
    populate();
    Income record = repo->income(42);
    EXPECT_EQ(-1, record.id());
}

/** Verifies that income can be retrieved by ID. */
TEST_F(SQLIncomeRepositoryTest, ShouldReturnValidIncomeWhenIDExists) {
    createRepo();
    populate();
    Income record = repo->income(3);
    EXPECT_EQ(3, record.id()) << repo->lastError().toStdString();
    EXPECT_EQ(Money(110.00, Currency(1, "USD")), record.amount());
    EXPECT_EQ(QString("Rent"), record.description());
    EXPECT_EQ(4, record.account().id());
    EXPECT_EQ(QDate(2017, 1, 11), record.beginningDate());
    EXPECT_FALSE(record.endingDate().isValid());

    Recurrence rec = record.recurrence();
    EXPECT_EQ(5, rec.day());
    EXPECT_EQ(0, rec.month());
    EXPECT_EQ(1, rec.periodicity());
    EXPECT_EQ(Recurrence::Monthly, rec.scope());
    EXPECT_EQ(0, rec.week());
}

/** Verifies that one-time income can be retrieved. */
TEST_F(SQLIncomeRepositoryTest, ShouldReturnValidOneTimeIncome) {
    createRepo();
    populate();
    Income record = repo->income(2);
    EXPECT_EQ(2, record.id());
    EXPECT_EQ(Money(1200.00, Currency(1, "USD")), record.amount());
    EXPECT_EQ(QString("Bill"), record.description());
    EXPECT_EQ(2, record.account().id());
    EXPECT_EQ(QDate(2016, 7, 1), record.beginningDate());
    EXPECT_EQ(QDate(2016, 7, 31), record.endingDate());

    Recurrence rec = record.recurrence();
    EXPECT_EQ(0, rec.day());
    EXPECT_EQ(0, rec.month());
    EXPECT_EQ(0, rec.periodicity());
    EXPECT_EQ(Recurrence::Yearly, rec.scope());
    EXPECT_EQ(0, rec.week());
}

/** Verifies that an empty income list is returned for an invalid account. */
TEST_F(SQLIncomeRepositoryTest, ShouldReturnEmptyListWhenAccountIsInvalid) {
    createRepo();
    populate();
    auto list = repo->incomes(Account(42));
    EXPECT_TRUE(list.empty());
}

/** Verifies that incomes are returned for an account in chronological order. */
TEST_F(SQLIncomeRepositoryTest, ShouldReturnIncomesAgainstAccountSortedByDate) {
    createRepo();
    populate();
    auto list = repo->incomes(Account(4));
    ASSERT_EQ(2u, list.size()) << repo->lastError().toStdString();
    EXPECT_EQ(1, list.at(0).id());
    EXPECT_EQ(QDate(2016, 2, 1), list.at(0).beginningDate());
    EXPECT_EQ(3, list.at(1).id());
    EXPECT_EQ(QDate(2017, 1, 11), list.at(1).beginningDate());
}

/** Verifies that incomes can be removed. */
TEST_F(SQLIncomeRepositoryTest, ShouldRemoveIncomeFromDatabase) {
    createRepo();
    populate();
    ASSERT_TRUE(repo->remove(Income(3))) << repo->lastError().toStdString();
    EXPECT_EQ(-1, repo->income(3).id());
    EXPECT_EQ(1u, repo->incomes(Account(4)).size());
}

/** Verifies that incomes can be modified. */
TEST_F(SQLIncomeRepositoryTest, ShouldUpdateIncomesInDatabase) {
    createRepo();
    populate();
    Income record(3);
    record.setAccount(Account(2));
    record.setAmount(Money(999.99));
    record.setBeginningDate(QDate(2017, 1, 1));
    record.setEndingDate(QDate(2017, 1, 31));
    Recurrence rec;
    rec.setPeriodicity(2);
    rec.setScope(Recurrence::Weekly);
    record.setRecurrence(rec);
    ASSERT_TRUE(repo->update(record)) << repo->lastError().toStdString();

    auto mod = repo->income(3);
    EXPECT_EQ(2, mod.account().id());
    EXPECT_EQ(Money(999.99, Currency(1, "USD")), mod.amount());
    EXPECT_EQ(QDate(2017, 1, 1), mod.beginningDate());
    EXPECT_EQ(QDate(2017, 1, 31), mod.endingDate());
    EXPECT_EQ(Recurrence::Weekly, mod.recurrence().scope());
}

/** Verifies that an empty list is returned when no incomes match the given date range */
TEST_F(SQLIncomeRepositoryTest, ShouldReturnEmptyListWhenNoIncomesOccurInDateRange) {
    createRepo();
    populate();
    auto list = repo->incomes(QDate(2010, 1, 1), QDate(2011, 1, 1));
    EXPECT_TRUE(list.empty());
}

/** Verifies that incomes are returned that begin before the date range */
TEST_F(SQLIncomeRepositoryTest, ShouldReturnIncomesThatBeginBeforeDateRange) {
    createRepo();
    populate();
    auto list = repo->incomes(QDate(2016, 4, 1), QDate(2016, 5, 1));
    EXPECT_EQ(1u, list.size());
    EXPECT_EQ(1, list[0].id());
}

/** Verifies that incomes are returned that begin and end within the date range */
TEST_F(SQLIncomeRepositoryTest, ShouldReturnIncomesThatBeginAndEndWithinDateRange) {
    createRepo();
    populate();
    auto list = repo->incomes(QDate(2016, 6, 1), QDate(2016, 8, 1));
    EXPECT_EQ(2u, list.size());
    EXPECT_EQ(1, list[0].id());
    EXPECT_EQ(2, list[1].id());
}

/** Verifies that incomes are returned that have no ending date */
TEST_F(SQLIncomeRepositoryTest, ShouldReturnIncomesThatHaveNoEndingDate) {
    createRepo();
    populate();
    auto list = repo->incomes(QDate(2017, 1, 10), QDate(2017, 2, 1));
    EXPECT_EQ(1u, list.size());
    EXPECT_EQ(3, list[0].id());
}

} // budget namespace
} // ub namespace
