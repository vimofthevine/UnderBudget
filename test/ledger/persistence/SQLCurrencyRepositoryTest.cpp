// Qt include(s)
#include <QtCore>
#include <QtSql>

// UnderBudget include(s)
#include <ledger/model/Currency.hpp>
#include <ledger/persistence/SQLCurrencyRepository.hpp>
#include "SQLCurrencyRepositoryTest.hpp"

//--------------------------------------------------------------------------------------------------
QTEST_MAIN(ub::ledger::SQLCurrencyRepositoryTest)

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
void SQLCurrencyRepositoryTest::dropAllTables() {
    auto tables = db.tables();
    QSqlQuery query(db);
    for (auto table : tables) {
        if (not query.exec("drop table " + table)) {
            qWarning() << query.lastError();
        }
    }
}

//--------------------------------------------------------------------------------------------------
void SQLCurrencyRepositoryTest::cleanup() {
    dropAllTables();
}

//--------------------------------------------------------------------------------------------------
void SQLCurrencyRepositoryTest::init() {
    if (QSqlDatabase::contains()) {
        db = QSqlDatabase::database();
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(":memory:");
        QVERIFY2(db.open(), "Unable to open database");
    }

    dropAllTables();

    QLocale::setDefault(QLocale(QLocale::German, QLocale::Germany));
}

//--------------------------------------------------------------------------------------------------
void SQLCurrencyRepositoryTest::shouldCreateTableWhenItDoesNotAlreadyExist() {
    auto tables = db.tables();
    QVERIFY(not tables.contains("currency"));

    SQLCurrencyRepository repo(db);

    tables = db.tables();
    QVERIFY(tables.contains("currency"));
}

//--------------------------------------------------------------------------------------------------
void SQLCurrencyRepositoryTest::shouldCreateDefaultCurrencyEntryWhenItDoesNotAlreadyExist() {
    {
        SQLCurrencyRepository repo(db);
        QSqlQuery query;
        query.exec("select count(id) from currency;");
        query.first();
        QSqlRecord record = query.record();
        QCOMPARE(record.value(0).toInt(), 1);
    }

    {
        SQLCurrencyRepository repo(db);
        QSqlQuery query;
        query.exec("select count(id) from currency;");
        query.first();
        QSqlRecord record = query.record();
        QCOMPARE(record.value(0).toInt(), 1);

        query.exec("select * from currency where id=1;");
        query.first();
        record = query.record();
        QCOMPARE(record.value("code"), QVariant("EUR"));
    }
}

//--------------------------------------------------------------------------------------------------
void SQLCurrencyRepositoryTest::shouldInsertNewCurrencyIntoTable() {
    SQLCurrencyRepository repo(db);
    Currency currency(999, "USD");
    QVERIFY(repo.create(currency) > 0);

    QSqlQuery query;
    query.exec("select count(id) from currency;");
    query.first();
    QSqlRecord record = query.record();
    QCOMPARE(record.value(0).toInt(), 2);
}

//--------------------------------------------------------------------------------------------------
void SQLCurrencyRepositoryTest::shouldReturnNewIDAfterInsertion() {
    SQLCurrencyRepository repo(db);
    Currency currency(999, "USD");
    QCOMPARE(repo.create(currency), 2);
}

//--------------------------------------------------------------------------------------------------
void SQLCurrencyRepositoryTest::shouldReturnExistingIDIfCodeAlreadyExists() {
    SQLCurrencyRepository repo(db);

    QSqlQuery query;
    query.exec("insert into currency(id, code) values(7, 'UAH');");

    Currency currency(999, "UAH");
    QCOMPARE(repo.create(currency), 7);
}

//--------------------------------------------------------------------------------------------------
void SQLCurrencyRepositoryTest::shouldSetAllFieldsInTableOnInsertion() {
    SQLCurrencyRepository repo(db);
    Currency currency(999, "USD");
    QCOMPARE(repo.create(currency), 2);

    QSqlQuery query("select * from currency where id=2;");
    query.first();
    QVERIFY(query.isValid());
    QSqlRecord record = query.record();
    QCOMPARE(record.value("id"), QVariant(2));
    QCOMPARE(record.value("code"), QVariant("USD"));
}

//--------------------------------------------------------------------------------------------------
void SQLCurrencyRepositoryTest::shouldRetrieveAllFieldsFromTable() {
    SQLCurrencyRepository repo(db);
    QSqlQuery query("insert into currency(id, code) values(42, 'UAH');");

    Currency currency = repo.getCurrency(42);
    QCOMPARE(currency.id(), 42);
    QCOMPARE(currency.code(), QString("UAH"));
}

//--------------------------------------------------------------------------------------------------
void SQLCurrencyRepositoryTest::shouldThrowWhenUpdatingNonDefaultCurrency() {
    QSqlQuery query("insert into currency(id, code) values(42, 'UAH');");
    SQLCurrencyRepository repo(db);
    Currency currency(42, "USD");
    QVERIFY_EXCEPTION_THROWN(repo.update(currency), std::invalid_argument);
}

//--------------------------------------------------------------------------------------------------
void SQLCurrencyRepositoryTest::shouldSetModifiedFieldsInTable() {
    SQLCurrencyRepository repo(db);
    Currency currency(1, "USD");
    QVERIFY(repo.update(currency));

    QSqlQuery query("select * from currency where id=1;");
    query.first();
    QVERIFY(query.isValid());
    QSqlRecord record = query.record();
    QCOMPARE(record.value("code"), QVariant("USD"));
}

//--------------------------------------------------------------------------------------------------
void SQLCurrencyRepositoryTest::shouldRemoveRowFromTable() {
    QSqlQuery query("insert into currency(id, code) values(42, 'UAH');");
    SQLCurrencyRepository repo(db);
    Currency currency(42, "EUR");
    QVERIFY(repo.remove(currency));

    query.exec("select * from currency where id=42;");
    query.first();
    QVERIFY(not query.isValid());
}

} // ledger namespace
} // ub namespace
