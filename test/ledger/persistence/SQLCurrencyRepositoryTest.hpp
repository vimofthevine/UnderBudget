#pragma once

// Qt include(s)
#include <QSqlDatabase>
#include <QtTest/QtTest>

namespace ub {
namespace ledger {

/** Unit test for the SQLCurrencyRepository class. */
class SQLCurrencyRepositoryTest : public QObject {
    Q_OBJECT

private slots:
    /** Clears the in-memory database */
    void cleanup();

    /** Sets up the in-memory database */
    void init();

    /** Verifies that the currency table is created when it does not already exist. */
    void shouldCreateTableWhenItDoesNotAlreadyExist();

    /** Verifies that the default currency entry is created if it does not already exist. */
    void shouldCreateDefaultCurrencyEntryWhenItDoesNotAlreadyExist();

    /** Verifies that new currencies are inserted into the table. */
    void shouldInsertNewCurrencyIntoTable();

    /** Verifies that the currency ID is returned after insertion. */
    void shouldReturnNewIDAfterInsertion();

    /** Verifies that existing ID is returned after insertion attempt. */
    void shouldReturnExistingIDIfCodeAlreadyExists();

    /** Verifies that all fields of the currency are inserted into the table. */
    void shouldSetAllFieldsInTableOnInsertion();

    /** Verifies that all fields of the currency are retrieved. */
    void shouldRetrieveAllFieldsFromTable();

    /** Verifies that exception is thrown when modifying a non-default currency. */
    void shouldThrowWhenUpdatingNonDefaultCurrency();

    /** Verifies that all fields of the currency are updated in the table after modification. */
    void shouldSetModifiedFieldsInTable();

    /** Verifies that the row is removed from the table after removal. */
    void shouldRemoveRowFromTable();

private:
    /** In-memory SQL database */
    QSqlDatabase db;

    /** Drops all tables in the database */
    void dropAllTables();
};

} // ledger namespace
} // ub namespace
