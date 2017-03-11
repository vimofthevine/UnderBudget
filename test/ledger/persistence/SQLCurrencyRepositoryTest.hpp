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
