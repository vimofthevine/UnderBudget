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
