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
#include <ledger/model/Currency.hpp>
#include <ledger/model/CurrencyRepository.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/persistence/SQLEnvelopeRepository.hpp>

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
class SQLEnvelopeRepositoryTest : public ::testing::Test {
protected:
    /** In-memory SQL database */
    QSqlDatabase db;
    /** Mock currency repository */
    std::shared_ptr<MockCurrencyRepository> currencies;

    /** Initializes the test fixture. */
    SQLEnvelopeRepositoryTest() : currencies(new ::testing::NiceMock<MockCurrencyRepository>()) {}

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

    /** Creates a family tree of envelopes */
    void createTree() {
        SQLEnvelopeRepository repo(db, currencies);
        Envelope envelope;
        envelope.setName("Child");
        EXPECT_EQ(2, repo.create(envelope, repo.getRoot()));

        envelope.setName("Grandchild 1");
        EXPECT_EQ(3, repo.create(envelope, repo.getEnvelope(2)));
        envelope.setName("Grandchild 2");
        EXPECT_EQ(4, repo.create(envelope, repo.getEnvelope(2)));

        envelope.setName("Great-grandchild 1");
        EXPECT_EQ(5, repo.create(envelope, repo.getEnvelope(4)));

        envelope.setName("Great-grandchild 2");
        EXPECT_EQ(6, repo.create(envelope, repo.getEnvelope(3)));
    }

    /**
     * Verifies that an envelope has the specified children relationships.
     *
     * @param repo     Envelope repository
     * @param id       ID of the parent envelope
     * @param children List of expected children IDs
     */
    void verifyChildren(SQLEnvelopeRepository & repo, int id, const std::vector<int> & children) {
        Envelope envelope = repo.getEnvelope(id);
        ASSERT_EQ(children.size(), envelope.children().size())
                << "Wrong number of children under ID " << id;
        for (size_t i = 0; i < children.size(); ++i) {
            EXPECT_EQ(children.at(i), envelope.children().at(i));
            EXPECT_EQ(id, repo.getEnvelope(children.at(i)).parent());
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

/** Verifies that the envelope table is created when it does not already exist. */
TEST_F(SQLEnvelopeRepositoryTest, ShouldCreateTableWhenItDoesNotAlreadyExist) {
    auto tables = db.tables();
    ASSERT_FALSE(tables.contains("envelope"));

    SQLEnvelopeRepository repo(db, currencies);

    tables = db.tables();
    ASSERT_TRUE(tables.contains("envelope"));
}

/** Verifies that the root envelope is created when it does not already exist. */
TEST_F(SQLEnvelopeRepositoryTest, ShouldCreateRootEnvelopeWhenItDoesNotAlreadyExist) {
    {
        SQLEnvelopeRepository repo(db, currencies);
        QSqlQuery query(db);
        query.exec("select count(id) from envelope;");
        query.first();
        QSqlRecord record = query.record();
        EXPECT_EQ(1, record.value(0).toInt());
    }

    {
        SQLEnvelopeRepository repo(db, currencies);
        QSqlQuery query(db);
        query.exec("select count(id) from envelope;");
        query.first();
        QSqlRecord record = query.record();
        EXPECT_EQ(1, record.value(0).toInt()) << "No envelope should have been added the second time";

        query.exec("select * from envelope where id=1;");
        query.first();
        record = query.record();
        EXPECT_EQ(1, record.value("currency_id").toInt());
    }
}

/** Verifies that envelopes can be created under the root envelope. */
TEST_F(SQLEnvelopeRepositoryTest, ShouldCreateEnvelopeUnderRoot) {
    {
        SQLEnvelopeRepository repo(db, currencies);
        Envelope envelope;
        envelope.setName("First Child");
        EXPECT_EQ(2, repo.create(envelope, repo.getRoot()));
    }

    {
        SQLEnvelopeRepository repo(db, currencies);
        Envelope root = repo.getRoot();
        ASSERT_EQ(1u, root.children().size());
        EXPECT_EQ(2, root.children().at(0));
        EXPECT_EQ(1, repo.getEnvelope(2).parent());
    }
}

/** Verifies that envelopes can be created under non-root envelopes. */
TEST_F(SQLEnvelopeRepositoryTest, ShouldCreateEnvelopesUnderNonRoot) {
    createTree();

    SQLEnvelopeRepository repo(db, currencies);

    verifyChildren(repo, 1, {2});
    verifyChildren(repo, 2, {3, 4});
    verifyChildren(repo, 3, {6});
    verifyChildren(repo, 4, {5});
    verifyChildren(repo, 5, {});
    verifyChildren(repo, 6, {});
}

/** Verifies that an invalid envelope is returned for an invalid ID */
TEST_F(SQLEnvelopeRepositoryTest, ShouldReturnInvalidEnvelopeWhenNoEnvelopeExistsForRequestedID) {
    SQLEnvelopeRepository repo(db, currencies);
    EXPECT_EQ(-1, repo.getEnvelope(42).id());
}

/** Verifies that leaf nodes can be retrieved */
TEST_F(SQLEnvelopeRepositoryTest, ShouldFindLeafNodes) {
    createTree();

    SQLEnvelopeRepository repo(db, currencies);
    auto leafs = repo.getLeafEnvelopes();
    ASSERT_EQ(2u, leafs.size());
    EXPECT_EQ(5, leafs.at(0).id());
    EXPECT_EQ(6, leafs.at(1).id());
}

/** Verifies that leaf nodes can be moved towards the back of the tree */
TEST_F(SQLEnvelopeRepositoryTest, ShouldMoveLeafTowardsBackOfTree) {
    createTree();

    SQLEnvelopeRepository repo(db, currencies);
    ASSERT_TRUE(repo.move(repo.getEnvelope(6), repo.getEnvelope(4)))
            << repo.lastError().toStdString();

    verifyChildren(repo, 1, {2});
    verifyChildren(repo, 2, {3, 4});
    verifyChildren(repo, 3, {});
    verifyChildren(repo, 4, {5, 6});
    verifyChildren(repo, 5, {});
    verifyChildren(repo, 6, {});
}

/** Verifies that leaf nodes can be moved towards the front of the tree */
TEST_F(SQLEnvelopeRepositoryTest, ShouldMoveLeafTowardsFrontOfTree) {
    createTree();

    SQLEnvelopeRepository repo(db, currencies);
    ASSERT_TRUE(repo.move(repo.getEnvelope(5), repo.getEnvelope(3)))
            << repo.lastError().toStdString();

    verifyChildren(repo, 1, {2});
    verifyChildren(repo, 2, {3, 4});
    verifyChildren(repo, 3, {6, 5});
    verifyChildren(repo, 4, {});
    verifyChildren(repo, 5, {});
    verifyChildren(repo, 6, {});
}

/** Verifies that subtrees can be moved towards the back of the tree */
TEST_F(SQLEnvelopeRepositoryTest, ShouldMoveSubtreeTowardsBackOfTree) {
    createTree();

    SQLEnvelopeRepository repo(db, currencies);
    ASSERT_TRUE(repo.move(repo.getEnvelope(3), repo.getEnvelope(1)))
            << repo.lastError().toStdString();

    verifyChildren(repo, 1, {2, 3});
    verifyChildren(repo, 2, {4});
    verifyChildren(repo, 3, {6});
    verifyChildren(repo, 4, {5});
    verifyChildren(repo, 5, {});
    verifyChildren(repo, 6, {});
}

/** Verifies that subtrees an be moved towards the front of the tree */
TEST_F(SQLEnvelopeRepositoryTest, ShouldMoveSubtreeTowardsFrontOfTree) {
    createTree();

    SQLEnvelopeRepository repo(db, currencies);
    ASSERT_TRUE(repo.move(repo.getEnvelope(4), repo.getEnvelope(3)))
            << repo.lastError().toStdString();

    verifyChildren(repo, 1, {2});
    verifyChildren(repo, 2, {3});
    verifyChildren(repo, 3, {6, 4});
    verifyChildren(repo, 4, {5});
    verifyChildren(repo, 5, {});
    verifyChildren(repo, 6, {});
}

/** Verifies that leaf nodes can be removed from the tree */
TEST_F(SQLEnvelopeRepositoryTest, ShouldRemoveLeafFromTree) {
    createTree();

    SQLEnvelopeRepository repo(db, currencies);
    ASSERT_TRUE(repo.remove(repo.getEnvelope(5))) << repo.lastError().toStdString();

    verifyChildren(repo, 1, {2});
    verifyChildren(repo, 2, {3, 4});
    verifyChildren(repo, 3, {6});
    verifyChildren(repo, 4, {});
    verifyChildren(repo, 6, {});
}

/** Verifies that subtrees can be removed from the tree */
TEST_F(SQLEnvelopeRepositoryTest, ShouldRemoveSubtreeFromTree) {
    createTree();

    SQLEnvelopeRepository repo(db, currencies);
    ASSERT_TRUE(repo.remove(repo.getEnvelope(3))) << repo.lastError().toStdString();

    verifyChildren(repo, 1, {2});
    verifyChildren(repo, 2, {4});
    verifyChildren(repo, 4, {5});
    verifyChildren(repo, 5, {});
}

/** Verifies that envelopes can be updated */
TEST_F(SQLEnvelopeRepositoryTest, ShouldUpdateEnvelopeInDatabase) {
    createTree();

    SQLEnvelopeRepository repo(db, currencies);
    Envelope envelope = repo.getEnvelope(4);
    envelope.setCurrency(Currency(2, "UAH"));
    envelope.setName("Fred");
    ASSERT_TRUE(repo.update(envelope));

    Envelope updated = repo.getEnvelope(4);
    EXPECT_EQ(2, updated.currency().id());
    EXPECT_EQ("Fred", updated.name().toStdString());
}

} // ledger namespace
} // ub namespace
