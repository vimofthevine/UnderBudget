// Qt include(s)
#include <QtCore>
#include <QtSql>

// UnderBudget include(s)
#include "AssetAccount.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
QString AssetAccount::table_name_ = "asset_accounts";

//--------------------------------------------------------------------------------------------------
bool AssetAccount::prepareDatabase() {
    bool result = true;

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    auto tables = db.tables();
    if (not tables.contains(table_name_)) {
        result = query.exec("create table " + table_name_ +
                " (id integer primary key, "
                "account_name varchar, "
                "account_number varchar, "
                "currency_id integer, "
                "current_balance integer, "
                "institution varchar, "
                "reconciled_balance integer);");
    }

    if (not result) {
        qWarning() << query.lastError();
    }

    return result;
}

//--------------------------------------------------------------------------------------------------
AssetAccount::AssetAccount(const QString & name)
    : db_(QSqlDatabase::database()), account_name_(name), currency_id_(0), current_balance_(0),
      id_(0), reconciled_balance_(0) {}

//--------------------------------------------------------------------------------------------------
AssetAccount::AssetAccount(int id)
    : db_(QSqlDatabase::database()), currency_id_(0), current_balance_(0),
      id_(0), reconciled_balance_(0) {
    QSqlQuery query;
    query.prepare("select * from " + table_name_ + " where id=:id;");
    query.bindValue(":id", id);
    if (query.exec() && query.first()) {
        QSqlRecord record = query.record();
        account_name_ = record.value("account_name").toString();
        account_number_ = record.value("account_number").toString();
        currency_id_ = record.value("currency_id").toInt();
        current_balance_ = record.value("current_balance").toInt();
        id_ = record.value("id").toInt();
        institution_ = record.value("institution").toString();
        reconciled_balance_ = record.value("reconciled_balance").toInt();
    }
}

//--------------------------------------------------------------------------------------------------
QString AssetAccount::accountNumber() const {
    return account_number_;
}

//--------------------------------------------------------------------------------------------------
int AssetAccount::currencyType() const {
    return currency_id_;
}

//--------------------------------------------------------------------------------------------------
int AssetAccount::currentBalance() const {
    return current_balance_;
}

//------------------------------------------------------------------------------
int AssetAccount::id() const {
    return id_;
}

//------------------------------------------------------------------------------
QString AssetAccount::financialInstitution() const {
    return institution_;
}

//------------------------------------------------------------------------------
QString AssetAccount::name() const {
    return account_name_;
}

//------------------------------------------------------------------------------
int AssetAccount::reconciledBalance() const {
    return reconciled_balance_;
}

//------------------------------------------------------------------------------
bool AssetAccount::save() {
    bool result = true;

    QSqlQuery query;
    if (id_ < 1) {
        query.prepare("insert into " + table_name_ + "("
                "account_name, account_number, currency_id, current_balance, "
                "institution, reconciled_balance) values("
                ":name, :number, :currency, :balance, "
                ":institution, :reconciled);");
        query.bindValue(":name", account_name_);
        query.bindValue(":number", account_number_);
        query.bindValue(":currency", currency_id_);
        query.bindValue(":balance", current_balance_);
        query.bindValue(":institution", institution_);
        query.bindValue(":reconciled", reconciled_balance_);

        result = query.exec();

        if (result) {
            id_ = query.lastInsertId().toInt();
        }
    } else {
        query.prepare("update " + table_name_ + " set account_name=:name, "
                "account_number=:number, currency_id=:currency, "
                "current_balance=:balance, institution=:institution, "
                "reconciled_balance=:reconciled where id=:id;");
        query.bindValue(":id", id_);
        query.bindValue(":name", account_name_);
        query.bindValue(":number", account_number_);
        query.bindValue(":currency", currency_id_);
        query.bindValue(":balance", current_balance_);
        query.bindValue(":institution", institution_);
        query.bindValue(":reconciled", reconciled_balance_);

        result = query.exec();
    }

    if (not result) {
        qWarning() << query.lastError();
    }

    return result;
}

//------------------------------------------------------------------------------
bool AssetAccount::setAccountNumber(const QString & number) {
    account_number_ = number;
    return true;
}

//------------------------------------------------------------------------------
bool AssetAccount::setCurrencyType(int id) {
    currency_id_ = id;
    return true;
}

//------------------------------------------------------------------------------
bool AssetAccount::setCurrentBalance(int balance) {
    current_balance_ = balance;
    return true;
}

//------------------------------------------------------------------------------
bool AssetAccount::setFinancialInstitution(const QString & institution) {
    institution_ = institution;
}

//------------------------------------------------------------------------------
bool AssetAccount::setName(const QString & name) {
    account_name_ = name;
}

//------------------------------------------------------------------------------
bool AssetAccount::setReconciledBalance(int balance) {
    reconciled_balance_ = balance;
}

} // ledger namespace
} // ub namespace
