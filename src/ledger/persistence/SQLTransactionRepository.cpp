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

// Standard include(s)
#include <cstdint>
#include <memory>
#include <stdexcept>

// Qt include(s)
#include <QtCore>
#include <QtSql>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountRepository.hpp>
#include <ledger/model/AccountTransaction.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/model/EnvelopeRepository.hpp>
#include <ledger/model/EnvelopeTransaction.hpp>
#include <ledger/model/Money.hpp>
#include <ledger/model/Transaction.hpp>
#include "SQLTransactionRepository.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
SQLTransactionRepository::SQLTransactionRepository(QSqlDatabase & db,
                                                   std::shared_ptr<AccountRepository> accounts,
                                                   std::shared_ptr<EnvelopeRepository> envelopes)
        : db_(db), accounts_(accounts), envelopes_(envelopes), in_transaction_(false) {
    if (not db_.tables().contains("account")) {
        last_error_ = QObject::tr("Account table does not exist");
        throw std::runtime_error(last_error_.toStdString());
    }
    if (not db_.tables().contains("envelope")) {
        last_error_ = QObject::tr("Envelope table does not exist");
        throw std::runtime_error(last_error_.toStdString());
    }
    if (not db_.tables().contains("reconciliation")) {
        last_error_ = QObject::tr("Reconciliation table does not exist");
        throw std::runtime_error(last_error_.toStdString());
    }

    QSqlQuery query(db);
    query.exec("PRAGMA foreign_keys = ON;");
    bool success = query.exec("CREATE TABLE IF NOT EXISTS transaction_entry("
                              "id INTEGER PRIMARY KEY, "
                              "date DATE NOT NULL, "
                              "payee VARCHAR NOT NULL);");
    if (not success) {
        throw std::runtime_error("Unable to create transaction table: " +
                                 query.lastError().text().toStdString());
    }

    success = query.exec(
        "CREATE TABLE IF NOT EXISTS account_transaction("
        "id INTEGER PRIMARY KEY, "
        "transaction_entry_id INTEGER NOT NULL, "
        "account_id INTEGER NOT NULL, "
        "amount INTEGER NOT NULL, "
        "memo VARCHAR, "
        "cleared BOOLEAN DEFAULT 0, "
        "reconciliation_id INTEGER, "
        "FOREIGN KEY(transaction_entry_id) REFERENCES transaction_entry(id) ON DELETE CASCADE, "
        "FOREIGN KEY(account_id) REFERENCES account(id) ON DELETE RESTRICT, "
        "FOREIGN KEY(reconciliation_id) REFERENCES reconciliation(id) ON DELETE SET NULL);");
    if (not success) {
        throw std::runtime_error("Unable to create account_transaction table: " +
                                 query.lastError().text().toStdString());
    }

    success = query.exec(
        "CREATE TABLE IF NOT EXISTS envelope_transaction("
        "id INTEGER PRIMARY KEY, "
        "transaction_entry_id INTEGER NOT NULL, "
        "envelope_id INTEGER NOT NULL, "
        "amount INTEGER NOT NULL, "
        "memo VARCHAR, "
        "FOREIGN KEY(transaction_entry_id) REFERENCES transaction_entry(id) ON DELETE CASCADE, "
        "FOREIGN KEY(envelope_id) REFERENCES envelope(id) ON DELETE RESTRICT);");
    if (not success) {
        throw std::runtime_error("Unable to create envelope_transaction table: " +
                                 query.lastError().text().toStdString());
    }
}

//--------------------------------------------------------------------------------------------------
int64_t SQLTransactionRepository::create(const AccountTransaction & transaction) {
    if (not in_transaction_) {
        in_transaction_ = db_.transaction();
    }

    QSqlQuery query(db_);
    query.prepare("INSERT INTO account_transaction(transaction_entry_id, account_id, amount, memo, "
                  "cleared, reconciliation_id) VALUES(:transaction, :account, :amount, :memo, "
                  ":cleared, :reconciliation);");
    query.bindValue(":transaction", QVariant::fromValue(transaction.transaction().id()));
    query.bindValue(":account", QVariant::fromValue(transaction.account().id()));
    query.bindValue(":amount", QVariant::fromValue(transaction.amount().scaled()));
    query.bindValue(":memo", transaction.memo());
    query.bindValue(":cleared", transaction.isCleared());
    query.bindValue(":reconciliation",
                    (transaction.reconciliation() > 0)
                        ? QVariant::fromValue(transaction.reconciliation())
                        : QVariant(QVariant::Int));

    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        in_transaction_ = false;
        return -1;
    }

    return query.lastInsertId().value<int64_t>();
}

//--------------------------------------------------------------------------------------------------
int64_t SQLTransactionRepository::create(const EnvelopeTransaction & transaction) {
    if (not in_transaction_) {
        in_transaction_ = db_.transaction();
    }

    QSqlQuery query(db_);
    query.prepare(
        "INSERT INTO envelope_transaction(transaction_entry_id, envelope_id, amount, memo) "
        "VALUES(:transaction, :envelope, :amount, :memo);");
    query.bindValue(":transaction", QVariant::fromValue(transaction.transaction().id()));
    query.bindValue(":envelope", QVariant::fromValue(transaction.envelope().id()));
    query.bindValue(":amount", QVariant::fromValue(transaction.amount().scaled()));
    query.bindValue(":memo", transaction.memo());

    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        in_transaction_ = false;
        return -1;
    }

    return query.lastInsertId().value<int64_t>();
}

//--------------------------------------------------------------------------------------------------
int64_t SQLTransactionRepository::create(const Transaction & transaction) {
    if (not in_transaction_) {
        in_transaction_ = db_.transaction();
    }

    QSqlQuery query(db_);
    query.prepare(
        "INSERT INTO transaction_entry(date, payee) VALUES(strftime('%s', :date), :payee);");
    query.bindValue(":date", transaction.date().toString("yyyy-MM-dd"));
    query.bindValue(":payee", transaction.payee());

    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        in_transaction_ = false;
        return -1;
    }

    return query.lastInsertId().value<int64_t>();
}

//--------------------------------------------------------------------------------------------------
AccountTransaction SQLTransactionRepository::getAccountTransaction(int64_t id) {
    QSqlQuery query(db_);
    query.prepare("SELECT account_transaction.*, transaction_entry.payee, "
                  "date(transaction_entry.date, 'unixepoch') AS date "
                  "FROM account_transaction JOIN transaction_entry ON "
                  "account_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE account_transaction.id=:id;");
    query.bindValue(":id", QVariant::fromValue(id));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    } else if (not query.first()) {
        last_error_ = QObject::tr("No account transaction found for ID %1").arg(id);
    } else {
        auto trn = toAccountTransaction(query.record());
        trn.setBalance(getBalance(trn.transaction().date(), trn.account()));
        return trn;
    }
    return AccountTransaction();
}

//--------------------------------------------------------------------------------------------------
std::vector<AccountTransaction>
SQLTransactionRepository::getAccountTransactions(const Transaction & transaction) {
    std::vector<AccountTransaction> accounts;
    QSqlQuery query(db_);
    query.prepare("SELECT account_transaction.*, transaction_entry.payee, "
                  "date(transaction_entry.date, 'unixepoch') AS date "
                  "FROM account_transaction JOIN transaction_entry ON "
                  "account_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE account_transaction.transaction_entry_id=:id;");
    query.bindValue(":id", QVariant::fromValue(transaction.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    while (query.next()) {
        auto trn = toAccountTransaction(query.record());
        trn.setBalance(getBalance(trn.transaction().date(), trn.account()));
        accounts.push_back(trn);
    }
    return accounts;
}

//--------------------------------------------------------------------------------------------------
std::vector<AccountTransaction>
SQLTransactionRepository::getAccountTransactions(const QDate & start, const QDate & stop) {
    std::vector<AccountTransaction> accounts;
    QSqlQuery query(db_);
    query.prepare("SELECT account_transaction.*, transaction_entry.payee, "
                  "date(transaction_entry.date, 'unixepoch') AS date "
                  "FROM account_transaction JOIN transaction_entry ON "
                  "account_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE transaction_entry.date>=strftime('%s', :begin) AND "
                  "transaction_entry.date<=strftime('%s', :end);");
    query.bindValue(":begin", start.toString("yyyy-MM-dd"));
    query.bindValue(":end", stop.toString("yyyy-MM-dd"));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    while (query.next()) {
        auto trn = toAccountTransaction(query.record());
        trn.setBalance(getBalance(trn.transaction().date(), trn.account()));
        accounts.push_back(trn);
    }
    return accounts;
}

//--------------------------------------------------------------------------------------------------
Money SQLTransactionRepository::getBalance(const QDate & date, const Currency & currency) {
    QSqlQuery query(db_);
    query.prepare("SELECT TOTAL(amount) AS balance "
                  "FROM account_transaction JOIN transaction_entry ON "
                  "account_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE transaction_entry.date<=strftime('%s', :date);");
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    if (not query.exec() or not query.first()) {
        last_error_ = query.lastError().text();
    } else {
        return Money(query.value("balance").value<int64_t>(), currency);
    }
    return Money();
}

//--------------------------------------------------------------------------------------------------
Money SQLTransactionRepository::getBalance(const QDate & date, const Account & account) {
    QSqlQuery query(db_);
    query.prepare("SELECT TOTAL(amount) AS balance "
                  "FROM account_transaction JOIN transaction_entry ON "
                  "account_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE account_transaction.account_id=:account AND "
                  "transaction_entry.date<=strftime('%s', :date);");
    query.bindValue(":account", QVariant::fromValue(account.id()));
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    if (not query.exec() or not query.first()) {
        last_error_ = query.lastError().text();
    } else {
        return Money(query.value("balance").value<int64_t>(), account.currency());
    }
    return Money();
}

//--------------------------------------------------------------------------------------------------
Money SQLTransactionRepository::getBalance(const QDate & date, const Envelope & envelope) {
    QSqlQuery query(db_);
    query.prepare("SELECT TOTAL(amount) AS balance "
                  "FROM envelope_transaction JOIN transaction_entry ON "
                  "envelope_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE envelope_transaction.envelope_id=:envelope AND "
                  "transaction_entry.date<=strftime('%s', :date);");
    query.bindValue(":envelope", QVariant::fromValue(envelope.id()));
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    if (not query.exec() or not query.first()) {
        last_error_ = query.lastError().text();
    } else {
        return Money(query.value("balance").value<int64_t>(), envelope.currency());
    }
    return Money();
}

//--------------------------------------------------------------------------------------------------
EnvelopeTransaction SQLTransactionRepository::getEnvelopeTransaction(int64_t id) {
    QSqlQuery query(db_);
    query.prepare("SELECT envelope_transaction.*, transaction_entry.payee, "
                  "date(transaction_entry.date, 'unixepoch') AS date "
                  "FROM envelope_transaction JOIN transaction_entry ON "
                  "envelope_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE envelope_transaction.id=:id;");
    query.bindValue(":id", QVariant::fromValue(id));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    } else if (not query.first()) {
        last_error_ = QObject::tr("No envelope transaction found for ID %1").arg(id);
    } else {
        auto trn = toEnvelopeTransaction(query.record());
        trn.setBalance(getBalance(trn.transaction().date(), trn.envelope()));
        return trn;
    }
    return EnvelopeTransaction();
}

//--------------------------------------------------------------------------------------------------
std::vector<EnvelopeTransaction>
SQLTransactionRepository::getEnvelopeTransactions(const Transaction & transaction) {
    std::vector<EnvelopeTransaction> envelopes;
    QSqlQuery query(db_);
    query.prepare("SELECT envelope_transaction.*, transaction_entry.payee, "
                  "date(transaction_entry.date, 'unixepoch') AS date "
                  "FROM envelope_transaction JOIN transaction_entry ON "
                  "envelope_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE envelope_transaction.transaction_entry_id=:id;");
    query.bindValue(":id", QVariant::fromValue(transaction.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    while (query.next()) {
        auto trn = toEnvelopeTransaction(query.record());
        trn.setBalance(getBalance(trn.transaction().date(), trn.envelope()));
        envelopes.push_back(trn);
    }
    return envelopes;
}

//--------------------------------------------------------------------------------------------------
std::vector<EnvelopeTransaction>
SQLTransactionRepository::getEnvelopeTransactions(const QDate & start, const QDate & stop) {
    std::vector<EnvelopeTransaction> envelopes;
    QSqlQuery query(db_);
    query.prepare("SELECT envelope_transaction.*, transaction_entry.payee, "
                  "date(transaction_entry.date, 'unixepoch') AS date "
                  "FROM envelope_transaction JOIN transaction_entry ON "
                  "envelope_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE transaction_entry.date>=strftime('%s', :begin) AND "
                  "transaction_entry.date<=strftime('%s', :end);");
    query.bindValue(":begin", start.toString("yyyy-MM-dd"));
    query.bindValue(":end", stop.toString("yyyy-MM-dd"));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    while (query.next()) {
        auto trn = toEnvelopeTransaction(query.record());
        trn.setBalance(getBalance(trn.transaction().date(), trn.envelope()));
        envelopes.push_back(trn);
    }
    return envelopes;
}

//--------------------------------------------------------------------------------------------------
Money SQLTransactionRepository::getReconciledBalance(const QDate & date, const Account & account) {
    QSqlQuery query(db_);
    query.prepare("SELECT TOTAL(amount) AS balance "
                  "FROM account_transaction JOIN transaction_entry ON "
                  "account_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE account_transaction.account_id=:account AND "
                  "account_transaction.reconciliation_id>0 AND "
                  "transaction_entry.date<=strftime('%s', :date);");
    query.bindValue(":account", QVariant::fromValue(account.id()));
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    if (not query.exec() or not query.first()) {
        last_error_ = query.lastError().text();
    } else {
        return Money(query.value("balance").value<int64_t>(), account.currency());
    }
    return Money();
}

//--------------------------------------------------------------------------------------------------
Transaction SQLTransactionRepository::getTransaction(int64_t id) {
    QSqlQuery query(db_);
    query.prepare("SELECT id, date(transaction_entry.date, 'unixepoch') AS date, payee "
                  "FROM transaction_entry WHERE id=:id;");
    query.bindValue(":id", QVariant::fromValue(id));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    } else if (not query.first()) {
        last_error_ = QObject::tr("No transaction found for ID %1").arg(id);
    } else {
        Transaction transaction(query.value("id").value<int64_t>());
        transaction.setDate(QDate::fromString(query.value("date").toString(), "yyyy-MM-dd"));
        transaction.setPayee(query.value("payee").toString());

        return transaction;
    }
    return Transaction();
}

//--------------------------------------------------------------------------------------------------
std::vector<AccountTransaction> SQLTransactionRepository::getTransactions(const Account & account) {
    std::vector<AccountTransaction> accounts;
    QSqlQuery query(db_);
    query.prepare("SELECT account_transaction.*, transaction_entry.payee, "
                  "date(transaction_entry.date, 'unixepoch') AS date "
                  "FROM account_transaction JOIN transaction_entry ON "
                  "account_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE account_transaction.account_id=:id ORDER BY transaction_entry.date ASC;");
    query.bindValue(":id", QVariant::fromValue(account.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    Money balance(0.0, account.currency());
    while (query.next()) {
        auto trn = toAccountTransaction(query.record());
        trn.setBalance(balance + trn.amount());
        accounts.push_back(trn);
        balance = trn.balance();
    }
    return accounts;
}

//--------------------------------------------------------------------------------------------------
std::vector<EnvelopeTransaction>
SQLTransactionRepository::getTransactions(const Envelope & envelope) {
    std::vector<EnvelopeTransaction> envelopes;
    QSqlQuery query(db_);
    query.prepare("SELECT envelope_transaction.*, transaction_entry.payee, "
                  "date(transaction_entry.date, 'unixepoch') AS date "
                  "FROM envelope_transaction JOIN transaction_entry ON "
                  "envelope_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE envelope_transaction.envelope_id=:id "
                  "ORDER BY transaction_entry.date ASC;");
    query.bindValue(":id", QVariant::fromValue(envelope.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    Money balance(0.0, envelope.currency());
    while (query.next()) {
        auto trn = toEnvelopeTransaction(query.record());
        trn.setBalance(balance + trn.amount());
        envelopes.push_back(trn);
        balance = trn.balance();
    }
    return envelopes;
}

//--------------------------------------------------------------------------------------------------
QString SQLTransactionRepository::lastError() {
    return last_error_;
}

//--------------------------------------------------------------------------------------------------
bool SQLTransactionRepository::remove(const Transaction & transaction) {
    if (not in_transaction_) {
        in_transaction_ = db_.transaction();
    }

    QSqlQuery query(db_);
    query.prepare("DELETE FROM transaction_entry WHERE id=:id;");
    query.bindValue(":id", QVariant::fromValue(transaction.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLTransactionRepository::remove(const AccountTransaction & transaction) {
    if (not in_transaction_) {
        in_transaction_ = db_.transaction();
    }

    QSqlQuery query(db_);
    query.prepare("DELETE FROM account_transaction WHERE id=:id;");
    query.bindValue(":id", QVariant::fromValue(transaction.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLTransactionRepository::remove(const EnvelopeTransaction & transaction) {
    if (not in_transaction_) {
        in_transaction_ = db_.transaction();
    }

    QSqlQuery query(db_);
    query.prepare("DELETE FROM envelope_transaction WHERE id=:id;");
    query.bindValue(":id", QVariant::fromValue(transaction.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLTransactionRepository::save() {
    bool success = db_.commit();
    in_transaction_ = false;
    if (not success) {
        last_error_ = db_.lastError().text();
    }
    return success;
}

//--------------------------------------------------------------------------------------------------
bool SQLTransactionRepository::update(const Transaction & transaction) {
    if (not in_transaction_) {
        in_transaction_ = db_.transaction();
    }

    QSqlQuery query(db_);
    query.prepare("UPDATE transaction_entry SET payee=:payee, date=strftime('%s', :date) "
                  " WHERE id=:id;");
    query.bindValue(":date", transaction.date().toString("yyyy-MM-dd"));
    query.bindValue(":payee", transaction.payee());
    query.bindValue(":id", QVariant::fromValue(transaction.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLTransactionRepository::update(const AccountTransaction & transaction) {
    if (not in_transaction_) {
        in_transaction_ = db_.transaction();
    }

    QSqlQuery query(db_);
    query.prepare("UPDATE account_transaction SET account_id=:account, amount=:amount, "
                  "memo=:memo, cleared=:cleared, reconciliation_id=:reconciliation "
                  "WHERE id=:id;");
    query.bindValue(":transaction", QVariant::fromValue(transaction.transaction().id()));
    query.bindValue(":account", QVariant::fromValue(transaction.account().id()));
    query.bindValue(":amount", QVariant::fromValue(transaction.amount().scaled()));
    query.bindValue(":memo", transaction.memo());
    query.bindValue(":cleared", transaction.isCleared());
    query.bindValue(":reconciliation",
                    (transaction.reconciliation() > 0)
                        ? QVariant::fromValue(transaction.reconciliation())
                        : QVariant(QVariant::Int));
    query.bindValue(":id", QVariant::fromValue(transaction.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLTransactionRepository::update(const EnvelopeTransaction & transaction) {
    if (not in_transaction_) {
        in_transaction_ = db_.transaction();
    }

    QSqlQuery query(db_);
    query.prepare("UPDATE envelope_transaction SET envelope_id=:envelope, amount=:amount, "
                  "memo=:memo WHERE id=:id;");
    query.bindValue(":transaction", QVariant::fromValue(transaction.transaction().id()));
    query.bindValue(":envelope", QVariant::fromValue(transaction.envelope().id()));
    query.bindValue(":amount", QVariant::fromValue(transaction.amount().scaled()));
    query.bindValue(":memo", transaction.memo());
    query.bindValue(":id", QVariant::fromValue(transaction.id()));
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
AccountTransaction SQLTransactionRepository::toAccountTransaction(const QSqlRecord & record) {
    auto account = accounts_->getAccount(record.value("account_id").value<int64_t>());

    Transaction transaction(record.value("transaction_entry_id").value<int64_t>());
    transaction.setDate(QDate::fromString(record.value("date").toString(), "yyyy-MM-dd"));
    transaction.setPayee(record.value("payee").toString());

    AccountTransaction split(record.value("id").value<int64_t>());
    split.setAccount(account);
    split.setAmount(Money(record.value("amount").value<int64_t>(), account.currency()));
    split.setCleared(record.value("cleared").toBool());
    split.setMemo(record.value("memo").toString());
    auto rec = record.value("reconciliation_id");
    if (not rec.isNull()) {
        split.setReconciliation(rec.value<int64_t>());
    }
    split.setTransaction(transaction);

    return split;
}

//--------------------------------------------------------------------------------------------------
EnvelopeTransaction SQLTransactionRepository::toEnvelopeTransaction(const QSqlRecord & record) {
    auto envelope = envelopes_->getEnvelope(record.value("envelope_id").value<int64_t>());

    Transaction transaction(record.value("transaction_entry_id").value<int64_t>());
    transaction.setDate(QDate::fromString(record.value("date").toString(), "yyyy-MM-dd"));
    transaction.setPayee(record.value("payee").toString());

    EnvelopeTransaction split(record.value("id").value<int64_t>());
    split.setEnvelope(envelope);
    split.setAmount(Money(record.value("amount").value<int64_t>(), envelope.currency()));
    split.setMemo(record.value("memo").toString());
    split.setTransaction(transaction);

    return split;
}

} // ledger namespace
} // ub namespace
