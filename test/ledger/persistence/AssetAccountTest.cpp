// Qt include(s)
#include <QtCore>
#include <QtSql>

// UnderBudget include(s)
#include <ledger/persistence/AssetAccount.hpp>
#include "AssetAccountTest.hpp"

//--------------------------------------------------------------------------------------------------
QTEST_MAIN(ub::ledger::AssetAccountTest)

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
void AssetAccountTest::dropAllTables() {
    auto tables = db.tables();
    QSqlQuery query;
    for (auto table : tables) {
        if (not query.exec("drop table " + table)) {
            qWarning() << query.lastError();
        }
    }
}

//--------------------------------------------------------------------------------------------------
void AssetAccountTest::cleanup() {
    dropAllTables();
}

//--------------------------------------------------------------------------------------------------
void AssetAccountTest::init() {
    if (QSqlDatabase::contains()) {
        db = QSqlDatabase::database();
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(":memory:");
        QVERIFY2(db.open(), "Unable to open database");
    }

    dropAllTables();

    QVERIFY(AssetAccount::prepareDatabase());
}

//--------------------------------------------------------------------------------------------------
void AssetAccountTest::shouldCreateTableWhenItDoesNotAlreadyExist() {
    auto tables = db.tables();
    QVERIFY(tables.contains("asset_accounts"));
}

//--------------------------------------------------------------------------------------------------
void AssetAccountTest::shouldInsertNewAccountIntoTable() {
    AssetAccount account("Test Acct");
    account.setAccountNumber("1234-5678");
    account.setCurrentBalance(6700);
    account.setReconciledBalance(6432);
    QVERIFY(account.save());

    QSqlQuery query;
    query.exec("select count(id) from asset_accounts;");
    query.first();
    QSqlRecord record = query.record();
    QCOMPARE(record.value(0).toInt(), 1);
}

//--------------------------------------------------------------------------------------------------
void AssetAccountTest::shouldUpdateIDAfterInsertion() {
    AssetAccount account("Test Acct");
    account.setAccountNumber("1234-5678");
    account.setCurrentBalance(6700);
    account.setReconciledBalance(6432);
    QVERIFY(account.save());
    QCOMPARE(account.id(), 1);
}

//--------------------------------------------------------------------------------------------------
void AssetAccountTest::shouldSetAllFieldsInTableOnInsertion() {
    AssetAccount account("Test Acct");
    account.setAccountNumber("1234-5678");
    account.setCurrentBalance(6700);
    account.setReconciledBalance(6432);
    QVERIFY(account.save());

    QSqlQuery query("select * from asset_accounts where id=1;");
    query.first();
    QVERIFY(query.isValid());
    QSqlRecord record = query.record();
    QCOMPARE(record.value("id"), QVariant(1));
    QCOMPARE(record.value("account_name"), QVariant("Test Acct"));
    QCOMPARE(record.value("account_number"), QVariant("1234-5678"));
    QCOMPARE(record.value("currency_id"), QVariant(0));
    QCOMPARE(record.value("current_balance"), QVariant(6700));
    QCOMPARE(record.value("institution"), QVariant(""));
    QCOMPARE(record.value("reconciled_balance"), QVariant(6432));
}

//--------------------------------------------------------------------------------------------------
void AssetAccountTest::shouldRetrieveAllFieldsFromTableOnInitialization() {
    QSqlQuery query("insert into asset_accounts(id, account_name, account_number, currency_id, "
            "current_balance, institution, reconciled_balance) values(42, 'bank account', "
            "'a00001234', 7, 132467, 'Best Bank', 132466);");

    AssetAccount account(42);
    QCOMPARE(account.id(), 42);
    QCOMPARE(account.name(), QString("bank account"));
    QCOMPARE(account.accountNumber(), QString("a00001234"));
    QCOMPARE(account.currencyType(), 7);
    QCOMPARE(account.currentBalance(), 132467);
    QCOMPARE(account.financialInstitution(), QString("Best Bank"));
    QCOMPARE(account.reconciledBalance(), 132466);
}

//--------------------------------------------------------------------------------------------------
void AssetAccountTest::shouldSetModifiedFieldsInTable() {
    {
        QSqlQuery query("insert into asset_accounts(id, account_name, account_number, currency_id, "
            "current_balance, institution, reconciled_balance) values(42, 'bank account', "
            "'a00001234', 7, 132467, 'Best Bank', 132466);");
    }

    AssetAccount account(42);
    QCOMPARE(account.id(), 42);

    account.setAccountNumber("00");
    account.setCurrencyType(99);
    account.setCurrentBalance(131267);
    account.setFinancialInstitution("wallet");
    account.setName("cash");
    account.setReconciledBalance(131266);
    account.save();

    QSqlQuery query("select * from asset_accounts where id=42;");
    query.first();
    QVERIFY(query.isValid());
    QSqlRecord record = query.record();
    QCOMPARE(record.value("account_name"), QVariant("cash"));
    QCOMPARE(record.value("account_number"), QVariant("00"));
    QCOMPARE(record.value("currency_id"), QVariant(99));
    QCOMPARE(record.value("current_balance"), QVariant(131267));
    QCOMPARE(record.value("institution"), QVariant("wallet"));
    QCOMPARE(record.value("reconciled_balance"), QVariant(131266));
}

} // ledger namespace
} // ub namespace
