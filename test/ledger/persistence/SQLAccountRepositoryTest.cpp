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
#include <ledger/model/Currency.hpp>
#include <ledger/model/CurrencyRepository.hpp>
#include <ledger/persistence/SQLAccountRepository.hpp>

namespace ub {
namespace ledger {

/** Mock currency repository */
class MockCurrencyRepository : public CurrencyRepository {
public:
    MOCK_METHOD1(create, int(const Currency &));
    MOCK_METHOD1(getCurrency, Currency(int));
    MOCK_METHOD0(lastError, QString());
    MOCK_METHOD1(remove, bool(const Currency &));
    MOCK_METHOD1(update, bool(const Currency &));
};

/** Test fixture */
class SQLAccountRepositoryTest : public ::testing::Test {
protected:
    /** In-memory SQL database */
    QSqlDatabase db;
    /** Mock currency repository */
    std::shared_ptr<MockCurrencyRepository> currencies;

    /** Initializes the test fixture. */
    SQLAccountRepositoryTest() : currencies(new ::testing::NiceMock<MockCurrencyRepository>()) {}

    /** Sets up the in-memory database */
    void SetUp() override {
        ON_CALL(*currencies.get(),
                getCurrency(1)).WillByDefault(::testing::Return(Currency(1, "USD")));
        ON_CALL(*currencies.get(),
                getCurrency(2)).WillByDefault(::testing::Return(Currency(2, "UAH")));

        if (QSqlDatabase::contains()) {
            db = QSqlDatabase::database();
        } else {
            db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName(":memory:");
            ASSERT_TRUE(db.open()) << "Unable to open database";
        }

        dropAllTables();
    }

    /** Clears the in-memory database */
    void TearDown() override {
        dropAllTables();
    }

    /** Creates a family tree of accounts */
    void createTree() {
        SQLAccountRepository repo(db, currencies);
        Account newAcct;
        newAcct.setName("Child");
        EXPECT_EQ(2, repo.create(newAcct, repo.getRoot()));

        newAcct.setName("Grandchild 1");
        EXPECT_EQ(3, repo.create(newAcct, repo.getAccount(2)));
        newAcct.setName("Grandchild 2");
        EXPECT_EQ(4, repo.create(newAcct, repo.getAccount(2)));

        newAcct.setName("Great-grandchild 1");
        EXPECT_EQ(5, repo.create(newAcct, repo.getAccount(4)));

        newAcct.setName("Great-grandchild 2");
        EXPECT_EQ(6, repo.create(newAcct, repo.getAccount(3)));
    }

    /**
     * Verifies that an account has the specified children relationships.
     *
     * @param repo     Account repository
     * @param id       ID of the parent account
     * @param children List of expected children IDs
     */
    void verifyChildren(SQLAccountRepository & repo, int id, const std::vector<int> & children) {
        Account account = repo.getAccount(id);
        ASSERT_EQ(children.size(), account.children().size())
                << "Wrong number of children under ID " << id;
        for (size_t i = 0; i < children.size(); ++i) {
            EXPECT_EQ(children.at(i), account.children().at(i));
            EXPECT_EQ(id, repo.getAccount(children.at(i)).parent());
        }
    }

private:
    /** Drops all tables in the database */
    void dropAllTables() {
        auto tables = db.tables();
        QSqlQuery query(db);
        for (auto table : tables) {
            if (not query.exec("drop table " + table)) {
                qWarning() << query.lastError();
            }
        }
    }
};

/** Verifies that the account table is created when it does not already exist. */
TEST_F(SQLAccountRepositoryTest, ShouldCreateTableWhenItDoesNotAlreadyExist) {
    auto tables = db.tables();
    ASSERT_FALSE(tables.contains("account"));

    SQLAccountRepository repo(db, currencies);

    tables = db.tables();
    ASSERT_TRUE(tables.contains("account"));
}

/** Verifies that the root account is created when it does not already exist. */
TEST_F(SQLAccountRepositoryTest, ShouldCreateRootAccountWhenItDoesNotAlreadyExist) {
    {
        SQLAccountRepository repo(db, currencies);
        QSqlQuery query(db);
        query.exec("select count(id) from account;");
        query.first();
        QSqlRecord record = query.record();
        EXPECT_EQ(1, record.value(0).toInt());
    }

    {
        SQLAccountRepository repo(db, currencies);
        QSqlQuery query(db);
        query.exec("select count(id) from account;");
        query.first();
        QSqlRecord record = query.record();
        EXPECT_EQ(1, record.value(0).toInt()) << "No account should have been added the second time";

        query.exec("select * from account where id=1;");
        query.first();
        record = query.record();
        EXPECT_EQ(1, record.value("currency_id").toInt());
    }
}

/** Verifies that accounts can be created under the root account. */
TEST_F(SQLAccountRepositoryTest, ShouldCreateAccountUnderRoot) {
    {
        SQLAccountRepository repo(db, currencies);
        Account newAcct;
        newAcct.setName("First Child");
        EXPECT_EQ(2, repo.create(newAcct, repo.getRoot()));
    }

    {
        SQLAccountRepository repo(db, currencies);
        Account root = repo.getRoot();
        ASSERT_EQ(1u, root.children().size());
        EXPECT_EQ(2, root.children().at(0));
        EXPECT_EQ(1, repo.getAccount(2).parent());
    }
}

/** Verifies that accounts can be created under non-root accounts. */
TEST_F(SQLAccountRepositoryTest, ShouldCreateAccountsUnderNonRoot) {
    createTree();

    SQLAccountRepository repo(db, currencies);

    verifyChildren(repo, 1, {2});
    verifyChildren(repo, 2, {3, 4});
    verifyChildren(repo, 3, {6});
    verifyChildren(repo, 4, {5});
    verifyChildren(repo, 5, {});
    verifyChildren(repo, 6, {});
}

/** Verifies that an invalid account is returned for an invalid ID */
TEST_F(SQLAccountRepositoryTest, ShouldReturnInvalidAccountWhenNoAccountExistsForRequestedID) {
    SQLAccountRepository repo(db, currencies);
    EXPECT_EQ(-1, repo.getAccount(42).id());
}

/** Verifies that leaf nodes can be moved towards the back of the tree */
TEST_F(SQLAccountRepositoryTest, ShouldMoveLeafTowardsBackOfTree) {
    createTree();

    SQLAccountRepository repo(db, currencies);
    ASSERT_TRUE(repo.move(repo.getAccount(6), repo.getAccount(4)))
            << repo.lastError().toStdString();

    verifyChildren(repo, 1, {2});
    verifyChildren(repo, 2, {3, 4});
    verifyChildren(repo, 3, {});
    verifyChildren(repo, 4, {5, 6});
    verifyChildren(repo, 5, {});
    verifyChildren(repo, 6, {});
}

/** Verifies that leaf nodes can be moved towards the front of the tree */
TEST_F(SQLAccountRepositoryTest, ShouldMoveLeafTowardsFrontOfTree) {
    createTree();

    SQLAccountRepository repo(db, currencies);
    ASSERT_TRUE(repo.move(repo.getAccount(5), repo.getAccount(3)))
            << repo.lastError().toStdString();

    verifyChildren(repo, 1, {2});
    verifyChildren(repo, 2, {3, 4});
    verifyChildren(repo, 3, {6, 5});
    verifyChildren(repo, 4, {});
    verifyChildren(repo, 5, {});
    verifyChildren(repo, 6, {});
}

/** Verifies that subtrees can be moved towards the back of the tree */
TEST_F(SQLAccountRepositoryTest, ShouldMoveSubtreeTowardsBackOfTree) {
    createTree();

    SQLAccountRepository repo(db, currencies);
    ASSERT_TRUE(repo.move(repo.getAccount(3), repo.getAccount(1)))
            << repo.lastError().toStdString();

    verifyChildren(repo, 1, {2, 3});
    verifyChildren(repo, 2, {4});
    verifyChildren(repo, 3, {6});
    verifyChildren(repo, 4, {5});
    verifyChildren(repo, 5, {});
    verifyChildren(repo, 6, {});
}

/** Verifies that subtrees an be moved towards the front of the tree */
TEST_F(SQLAccountRepositoryTest, ShouldMoveSubtreeTowardsFrontOfTree) {
    createTree();

    SQLAccountRepository repo(db, currencies);
    ASSERT_TRUE(repo.move(repo.getAccount(4), repo.getAccount(3)))
            << repo.lastError().toStdString();

    verifyChildren(repo, 1, {2});
    verifyChildren(repo, 2, {3});
    verifyChildren(repo, 3, {6, 4});
    verifyChildren(repo, 4, {5});
    verifyChildren(repo, 5, {});
    verifyChildren(repo, 6, {});
}

/** Verifies that leaf nodes can be removed from the tree */
TEST_F(SQLAccountRepositoryTest, ShouldRemoveLeafFromTree) {
    createTree();

    SQLAccountRepository repo(db, currencies);
    ASSERT_TRUE(repo.remove(repo.getAccount(5))) << repo.lastError().toStdString();

    verifyChildren(repo, 1, {2});
    verifyChildren(repo, 2, {3, 4});
    verifyChildren(repo, 3, {6});
    verifyChildren(repo, 4, {});
    verifyChildren(repo, 6, {});
}

/** Verifies that subtrees can be removed from the tree */
TEST_F(SQLAccountRepositoryTest, ShouldRemoveSubtreeFromTree) {
    createTree();

    SQLAccountRepository repo(db, currencies);
    ASSERT_TRUE(repo.remove(repo.getAccount(3))) << repo.lastError().toStdString();

    verifyChildren(repo, 1, {2});
    verifyChildren(repo, 2, {4});
    verifyChildren(repo, 4, {5});
    verifyChildren(repo, 5, {});
}

/** Verifies that accounts can be updated */
TEST_F(SQLAccountRepositoryTest, ShouldUpdateAccountInDatabase) {
    createTree();

    SQLAccountRepository repo(db, currencies);
    Account account = repo.getAccount(4);
    account.setCurrency(Currency(2, "UAH"));
    account.setName("Fred");
    ASSERT_TRUE(repo.update(account));

    Account updated = repo.getAccount(4);
    EXPECT_EQ(2, updated.currency().id());
    EXPECT_EQ("Fred", updated.name().toStdString());
}

} // ledger namespace
} // ub namespace
