/*
 * Copyright 2016 Kyle Treubig
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

// Qt include(s)
#include <QtCore>
#include <QtSql>

// Google include(s)
#include <gtest/gtest.h>

// UnderBudget include(s)
#include <ledger/model/Currency.hpp>
#include <ledger/persistence/SQLCurrencyRepository.hpp>

namespace ub {
namespace ledger {

/** Test fixture */
class SQLCurrencyRepositoryTest : public ::testing::Test {
protected:
    /** In-memory SQL database */
    QSqlDatabase db;

    /** Sets up the in-memory database */
    void SetUp() override {
        if (QSqlDatabase::contains()) {
            db = QSqlDatabase::database();
        } else {
            db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName(":memory:");
            ASSERT_TRUE(db.open()) << "Unable to open database";
        }

        dropAllTables();

        QLocale::setDefault(QLocale(QLocale::German, QLocale::Germany));
    }

    /** Clears the in-memory database */
    void TearDown() override {
        dropAllTables();
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

/** Verifies that the currency table is created when it does not already exist. */
TEST_F(SQLCurrencyRepositoryTest, ShouldCreateTableWhenItDoesNotAlreadyExist) {
    auto tables = db.tables();
    ASSERT_FALSE(tables.contains("currency"));

    SQLCurrencyRepository repo(db);

    tables = db.tables();
    ASSERT_TRUE(tables.contains("currency"));
}

/** Verifies that the default currency entry is created if it does not already exist. */
TEST_F(SQLCurrencyRepositoryTest, ShouldCreateDefaultCurrencyEntryWhenItDoesNotAlreadyExist) {
    {
        SQLCurrencyRepository repo(db);
        QSqlQuery query;
        query.exec("select count(id) from currency;");
        query.first();
        QSqlRecord record = query.record();
        EXPECT_EQ(record.value(0).toInt(), 1);
    }

    {
        SQLCurrencyRepository repo(db);
        QSqlQuery query;
        query.exec("select count(id) from currency;");
        query.first();
        QSqlRecord record = query.record();
        EXPECT_EQ(record.value(0).toInt(), 1);

        query.exec("select * from currency where id=1;");
        query.first();
        record = query.record();
        EXPECT_EQ(record.value("code"), QVariant("EUR"));
    }
}

/** Verifies that new currencies are inserted into the table. */
TEST_F(SQLCurrencyRepositoryTest, ShouldInsertNewCurrencyIntoTable) {
    SQLCurrencyRepository repo(db);
    Currency currency(999, "USD");
    ASSERT_GT(repo.create(currency), 0);

    QSqlQuery query;
    query.exec("select count(id) from currency;");
    query.first();
    QSqlRecord record = query.record();
    EXPECT_EQ(record.value(0).toInt(), 2);
}

/** Verifies that the currency ID is returned after insertion. */
TEST_F(SQLCurrencyRepositoryTest, ShouldReturnNewIDAfterInsertion) {
    SQLCurrencyRepository repo(db);
    Currency currency(999, "USD");
    EXPECT_EQ(repo.create(currency), 2);
}

/** Verifies that existing ID is returned after insertion attempt. */
TEST_F(SQLCurrencyRepositoryTest, ShouldReturnExistingIDIfCodeAlreadyExists) {
    SQLCurrencyRepository repo(db);

    QSqlQuery query;
    query.exec("insert into currency(id, code) values(7, 'UAH');");

    Currency currency(999, "UAH");
    EXPECT_EQ(repo.create(currency), 7);
}

/** Verifies that all fields of the currency are inserted into the table. */
TEST_F(SQLCurrencyRepositoryTest, ShouldSetAllFieldsInTableOnInsertion) {
    SQLCurrencyRepository repo(db);
    Currency currency(999, "USD");
    EXPECT_EQ(repo.create(currency), 2);

    QSqlQuery query("select * from currency where id=2;");
    query.first();
    ASSERT_TRUE(query.isValid());
    QSqlRecord record = query.record();
    EXPECT_EQ(record.value("id"), QVariant(2));
    EXPECT_EQ(record.value("code"), QVariant("USD"));
}

/** Verifies that all fields of the currency are retrieved. */
TEST_F(SQLCurrencyRepositoryTest, ShouldRetrieveAllFieldsFromTable) {
    SQLCurrencyRepository repo(db);
    QSqlQuery query("insert into currency(id, code) values(42, 'UAH');");

    Currency currency = repo.getCurrency(42);
    EXPECT_EQ(currency.id(), 42);
    EXPECT_EQ(currency.code(), QString("UAH"));
}

/** Verifies that exception is thrown when modifying a non-default currency. */
TEST_F(SQLCurrencyRepositoryTest, ShouldThrowWhenUpdatingNonDefaultCurrency) {
    QSqlQuery query("insert into currency(id, code) values(42, 'UAH');");
    SQLCurrencyRepository repo(db);
    Currency currency(42, "USD");
    EXPECT_THROW(repo.update(currency), std::invalid_argument);
}

/** Verifies that all fields of the currency are updated in the table after modification. */
TEST_F(SQLCurrencyRepositoryTest, ShouldSetModifiedFieldsInTable) {
    SQLCurrencyRepository repo(db);
    Currency currency(1, "USD");
    ASSERT_TRUE(repo.update(currency));

    QSqlQuery query("select * from currency where id=1;");
    query.first();
    ASSERT_TRUE(query.isValid());
    QSqlRecord record = query.record();
    EXPECT_EQ(record.value("code"), QVariant("USD"));
}

/** Verifies that the row is removed from the table after removal. */
TEST_F(SQLCurrencyRepositoryTest, ShouldRemoveRowFromTable) {
    QSqlQuery query("insert into currency(id, code) values(42, 'UAH');");
    SQLCurrencyRepository repo(db);
    Currency currency(42, "EUR");
    ASSERT_TRUE(repo.remove(currency));

    query.exec("select * from currency where id=42;");
    query.first();
    ASSERT_FALSE(query.isValid());
}

} // ledger namespace
} // ub namespace
