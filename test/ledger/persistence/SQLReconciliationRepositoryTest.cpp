// Standard include(s)
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
#include <ledger/model/Money.hpp>
#include <ledger/model/Reconciliation.hpp>
#include <ledger/model/ReconciliationRepository.hpp>
#include <ledger/persistence/SQLReconciliationRepository.hpp>

namespace ub {
namespace ledger {

/** Mock account repository */
class MockAccountRepository : public AccountRepository {
public:
    MOCK_METHOD2(create, int(const Account &, const Account &));
    MOCK_METHOD1(getAccount, Account(int));
    MOCK_METHOD0(getLeafAccounts, std::vector<Account>());
    MOCK_METHOD0(getRoot, Account());
    MOCK_CONST_METHOD0(lastError, QString());
    MOCK_METHOD2(move, bool(const Account &, const Account &));
    MOCK_METHOD1(remove, bool(const Account &));
    MOCK_METHOD1(update, bool(const Account &));
};

/** Test fixture */
class SQLReconciliationRepositoryTest : public ::testing::Test {
protected:
    /** In-memory SQL database */
    QSqlDatabase db;
    /** Mock account repository */
    std::shared_ptr<MockAccountRepository> accounts;
    /** Reconciliation repository */
    std::unique_ptr<SQLReconciliationRepository> repo;

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

    /** Sets up the reconciliation repository */
    void createRepo() {
        try {
            repo.reset(new SQLReconciliationRepository(db, accounts));
        } catch (std::exception & e) {
            FAIL() << "Exception thrown: " << e.what();
        }
    }

    /** Populates the database with several reconciliations. */
    void populate() {
        Reconciliation r1;
        r1.setAccount(Account(4));
        r1.setBeginningBalance(Money(100.00));
        r1.setBeginningDate(QDate(2016, 12, 1));
        r1.setEndingBalance(Money(6.33));
        r1.setEndingDate(QDate(2016, 12, 31));
        EXPECT_EQ(1, repo->create(r1)) << repo->lastError().toStdString();

        Reconciliation r2;
        r2.setAccount(Account(2));
        r2.setBeginningBalance(Money(67.38));
        r2.setBeginningDate(QDate(2017, 1, 1));
        r2.setEndingBalance(Money(67.38));
        r2.setEndingDate(QDate(2017, 1, 31));
        EXPECT_EQ(2, repo->create(r2)) << repo->lastError().toStdString();

        Reconciliation r3;
        r3.setAccount(Account(4));
        r3.setBeginningBalance(Money(6.33));
        r3.setBeginningDate(QDate(2017, 1, 1));
        r3.setEndingBalance(Money(204.07));
        r3.setEndingDate(QDate(2017, 1, 31));
        EXPECT_EQ(3, repo->create(r3)) << repo->lastError().toStdString();

        Reconciliation r4;
        r4.setAccount(Account(4));
        r4.setBeginningBalance(Money(204.07));
        r4.setBeginningDate(QDate(2017, 2, 1));
        r4.setEndingBalance(Money(117.48));
        r4.setEndingDate(QDate(2017, 2, 28));
        EXPECT_EQ(4, repo->create(r4)) << repo->lastError().toStdString();
    }
};

/** Verifies that the reconciliation table is created when it does not already exist. */
TEST_F(SQLReconciliationRepositoryTest, ShouldCreateTableWhenItDoesNotAlreadyExist) {
    auto tables = db.tables();
    ASSERT_FALSE(tables.contains("reconciliation"));

    createRepo();

    tables = db.tables();
    ASSERT_TRUE(tables.contains("reconciliation"));
}

/** Verifies that reconciliations require an account to exist. */
TEST_F(SQLReconciliationRepositoryTest, ShouldNotInsertReconciliationWhenAccountDoesNotExist) {
    createRepo();
    Reconciliation reconciliation;
    reconciliation.setAccount(Account(3));

    EXPECT_EQ(-1, repo->create(reconciliation));

    QSqlQuery query(db);
    query.exec("select count(id) from reconciliation;");
    query.first();
    auto record = query.record();
    EXPECT_EQ(0, record.value(0).toInt());
}

/** Verifies that an invalid reconciliation is returned for an invalid ID. */
TEST_F(SQLReconciliationRepositoryTest, ShouldReturnInvalidReconciliationWhenIDDoesNotExist) {
    createRepo();
    populate();
    Reconciliation record = repo->getReconciliation(42);
    EXPECT_EQ(-1, record.id());
}

/** Verifies that reconciliation can be retrieved by ID. */
TEST_F(SQLReconciliationRepositoryTest, ShouldReturnValidReconciliationWhenIDExists) {
    createRepo();
    populate();
    Reconciliation record = repo->getReconciliation(3);
    EXPECT_EQ(3, record.id());
    EXPECT_EQ(4, record.account().id());
    EXPECT_EQ(Money(6.33, Currency(1, "USD")), record.beginningBalance());
    EXPECT_EQ(QDate(2017, 1, 1), record.beginningDate());
    EXPECT_EQ(Money(204.07, Currency(1, "USD")), record.endingBalance());
    EXPECT_EQ(QDate(2017, 1, 31), record.endingDate());
}

/** Verifies that an empty reconciliation list is returned for an invalid account. */
TEST_F(SQLReconciliationRepositoryTest, ShouldReturnEmptyListWhenAccountIsInvalid) {
    createRepo();
    populate();
    auto list = repo->getReconciliations(Account(42));
    EXPECT_TRUE(list.empty());
}

/** Verifies that reconciliations are returned for an account in chronological order. */
TEST_F(SQLReconciliationRepositoryTest, ShouldReturnReconciliationsAgainstAccountSortedByDate) {
    createRepo();
    populate();
    auto list = repo->getReconciliations(Account(4));
    ASSERT_EQ(3u, list.size()) << repo->lastError().toStdString();
    EXPECT_EQ(1, list.at(0).id());
    EXPECT_EQ(QDate(2016, 12, 1), list.at(0).beginningDate());
    EXPECT_EQ(3, list.at(1).id());
    EXPECT_EQ(QDate(2017, 1, 1), list.at(1).beginningDate());
    EXPECT_EQ(4, list.at(2).id());
    EXPECT_EQ(QDate(2017, 2, 1), list.at(2).beginningDate());
}

/** Verifies that reconciliations can be removed. */
TEST_F(SQLReconciliationRepositoryTest, ShouldRemoveReconciliationFromDatabase) {
    createRepo();
    populate();
    ASSERT_TRUE(repo->remove(Reconciliation(3))) << repo->lastError().toStdString();
    EXPECT_EQ(-1, repo->getReconciliation(3).id());
    EXPECT_EQ(2u, repo->getReconciliations(Account(4)).size());
}

/** Verifies that reconciliations can be modified. */
TEST_F(SQLReconciliationRepositoryTest, ShouldUpdateReconciliationsInDatabase) {
    createRepo();
    populate();
    Reconciliation record(3);
    record.setAccount(Account(2));
    record.setBeginningBalance(Money(67.38));
    record.setBeginningDate(QDate(2017, 1, 1));
    record.setEndingBalance(Money(67.38));
    record.setEndingDate(QDate(2017, 1, 31));
    ASSERT_TRUE(repo->update(record)) << repo->lastError().toStdString();

    auto mod = repo->getReconciliation(3);
    EXPECT_EQ(2, mod.account().id());
    EXPECT_EQ(Money(67.38, Currency(1, "USD")), mod.beginningBalance());
}

} // ledger namespace
} // ub namespace
