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

#pragma once

// Qt include(s)
#include <QSqlDatabase>
#include <QtTest/QtTest>

namespace ub {
namespace ledger {

/** Unit test for the AssetAccount class. */
class AssetAccountTest : public QObject {
    Q_OBJECT

private slots:
    /** Clears the in-memory database */
    void cleanup();

    /** Sets up the in-memory database */
    void init();

    /** Verifies that the asset_accounts table is created when it does not already exist. */
    void shouldCreateTableWhenItDoesNotAlreadyExist();

    /** Verifies that new accounts are inserted into the table. */
    void shouldInsertNewAccountIntoTable();

    /** Verifies that the account ID is updated after insertion. */
    void shouldUpdateIDAfterInsertion();

    /** Verifies that all fields of the account are inserted into the table. */
    void shouldSetAllFieldsInTableOnInsertion();

    /** Verifies that all fields of the account are retrieved on initialization. */
    void shouldRetrieveAllFieldsFromTableOnInitialization();

    /** Verifies that all fields of the account are updated in the table after modification. */
    void shouldSetModifiedFieldsInTable();

private:
    /** In-memory SQL database */
    QSqlDatabase db;

    /** Drops all tables in the database */
    void dropAllTables();
};

} // ledger namespace
} // ub namespace
