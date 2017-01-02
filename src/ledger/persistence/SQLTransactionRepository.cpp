// Standard include(s)
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

    success = query.exec("CREATE TABLE IF NOT EXISTS account_transaction("
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

    success = query.exec("CREATE TABLE IF NOT EXISTS envelope_transaction("
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
int SQLTransactionRepository::create(const AccountTransaction & transaction) {
    if (not in_transaction_) {
        in_transaction_ = db_.transaction();
    }

    QSqlQuery query(db_);
    query.prepare("INSERT INTO account_transaction(transaction_entry_id, account_id, amount, memo, "
                  "cleared, reconciliation_id) VALUES(:transaction, :account, :amount, :memo, "
                  ":cleared, :reconciliation);");
    query.bindValue(":transaction", transaction.transaction().id());
    query.bindValue(":account", transaction.account().id());
    query.bindValue(":amount", transaction.amount().scaled());
    query.bindValue(":memo", transaction.memo());
    query.bindValue(":cleared", transaction.isCleared());
    query.bindValue(":reconciliation", (transaction.reconciliation() > 0)
                    ? transaction.reconciliation() : QVariant());

    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        in_transaction_ = false;
        return -1;
    }

    return query.lastInsertId().toInt();
}

//--------------------------------------------------------------------------------------------------
int SQLTransactionRepository::create(const EnvelopeTransaction & transaction) {
    if (not in_transaction_) {
        in_transaction_ = db_.transaction();
    }

    QSqlQuery query(db_);
    query.prepare("INSERT INTO envelope_transaction(transaction_entry_id, envelope_id, amount, memo) "
                  "VALUES(:transaction, :envelope, :amount, :memo);");
    query.bindValue(":transaction", transaction.transaction().id());
    query.bindValue(":envelope", transaction.envelope().id());
    query.bindValue(":amount", transaction.amount().scaled());
    query.bindValue(":memo", transaction.memo());

    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        in_transaction_ = false;
        return -1;
    }

    return query.lastInsertId().toInt();
}

//--------------------------------------------------------------------------------------------------
int SQLTransactionRepository::create(const Transaction & transaction) {
    if (not in_transaction_) {
        in_transaction_ = db_.transaction();
    }

    QSqlQuery query(db_);
    query.prepare("INSERT INTO transaction_entry(date, payee) VALUES(strftime('%s', :date), :payee);");
    query.bindValue(":date", transaction.date().toString("yyyy-MM-dd"));
    query.bindValue(":payee", transaction.payee());

    if (not query.exec()) {
        last_error_ = query.lastError().text();
        db_.rollback();
        in_transaction_ = false;
        return -1;
    }

    return query.lastInsertId().toInt();
}

//--------------------------------------------------------------------------------------------------
AccountTransaction SQLTransactionRepository::getAccountTransaction(int id) {
    QSqlQuery query(db_);
    query.prepare("SELECT account_transaction.*, transaction_entry.payee, "
                  "date(transaction_entry.date, 'unixepoch') AS date "
                  "FROM account_transaction JOIN transaction_entry ON "
                  "account_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE account_transaction.id=:id;");
    query.bindValue(":id", id);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    } else if (not query.first()) {
        last_error_ = QObject::tr("No account transaction found for ID %1").arg(id);
    } else {
        return toAccountTransaction(query.record());
    }
    return AccountTransaction();
}

//--------------------------------------------------------------------------------------------------
std::vector<AccountTransaction> SQLTransactionRepository::getAccountTransactions(
        const Transaction & transaction) {
    std::vector<AccountTransaction> accounts;
    QSqlQuery query(db_);
    query.prepare("SELECT account_transaction.*, transaction_entry.payee, "
                  "date(transaction_entry.date, 'unixepoch') AS date "
                  "FROM account_transaction JOIN transaction_entry ON "
                  "account_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE account_transaction.transaction_entry_id=:id;");
    query.bindValue(":id", transaction.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    while (query.next()) {
        accounts.push_back(toAccountTransaction(query.record()));
    }
    return accounts;
}

//--------------------------------------------------------------------------------------------------
Money SQLTransactionRepository::getBalance(const QDate & date, const Account & account) {
    QSqlQuery query(db_);
    query.prepare("SELECT TOTAL(amount) AS balance "
                  "FROM account_transaction JOIN transaction_entry ON "
                  "account_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE account_transaction.account_id=:account AND "
                  "transaction_entry.date<=strftime('%s', :date);");
    query.bindValue(":account", account.id());
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    if (not query.exec() or not query.first()) {
        last_error_ = query.lastError().text();
    } else {
        return Money(query.value("balance").toInt(), account.currency());
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
    query.bindValue(":envelope", envelope.id());
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    if (not query.exec() or not query.first()) {
        last_error_ = query.lastError().text();
    } else {
        return Money(query.value("balance").toInt(), envelope.currency());
    }
    return Money();
}

//--------------------------------------------------------------------------------------------------
EnvelopeTransaction SQLTransactionRepository::getEnvelopeTransaction(int id) {
    QSqlQuery query(db_);
    query.prepare("SELECT envelope_transaction.*, transaction_entry.payee, "
                  "date(transaction_entry.date, 'unixepoch') AS date "
                  "FROM envelope_transaction JOIN transaction_entry ON "
                  "envelope_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE envelope_transaction.id=:id;");
    query.bindValue(":id", id);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    } else if (not query.first()) {
        last_error_ = QObject::tr("No envelope transaction found for ID %1").arg(id);
    } else {
        return toEnvelopeTransaction(query.record());
    }
    return EnvelopeTransaction();
}

//--------------------------------------------------------------------------------------------------
std::vector<EnvelopeTransaction> SQLTransactionRepository::getEnvelopeTransactions(
        const Transaction & transaction) {
    std::vector<EnvelopeTransaction> envelopes;
    QSqlQuery query(db_);
    query.prepare("SELECT envelope_transaction.*, transaction_entry.payee, "
                  "date(transaction_entry.date, 'unixepoch') AS date "
                  "FROM envelope_transaction JOIN transaction_entry ON "
                  "envelope_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE envelope_transaction.transaction_entry_id=:id;");
    query.bindValue(":id", transaction.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    while (query.next()) {
        envelopes.push_back(toEnvelopeTransaction(query.record()));
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
    query.bindValue(":account", account.id());
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    if (not query.exec() or not query.first()) {
        last_error_ = query.lastError().text();
    } else {
        return Money(query.value("balance").toInt(), account.currency());
    }
    return Money();
}

//--------------------------------------------------------------------------------------------------
Transaction SQLTransactionRepository::getTransaction(int id) {
    QSqlQuery query(db_);
    query.prepare("SELECT id, date(transaction_entry.date, 'unixepoch') AS date, payee "
                  "FROM transaction_entry WHERE id=:id;");
    query.bindValue(":id", id);
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    } else if (not query.first()) {
        last_error_ = QObject::tr("No transaction found for ID %1").arg(id);
    } else {
        Transaction transaction(query.value("id").toInt());
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
                  "WHERE account_transaction.account_id=:id ORDER BY transaction_entry.date DESC;");
    query.bindValue(":id", account.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    while (query.next()) {
        accounts.push_back(toAccountTransaction(query.record()));
    }
    return accounts;
}

//--------------------------------------------------------------------------------------------------
std::vector<EnvelopeTransaction> SQLTransactionRepository::getTransactions(const Envelope & envelope) {
    std::vector<EnvelopeTransaction> envelopes;
    QSqlQuery query(db_);
    query.prepare("SELECT envelope_transaction.*, transaction_entry.payee, "
                  "date(transaction_entry.date, 'unixepoch') AS date "
                  "FROM envelope_transaction JOIN transaction_entry ON "
                  "envelope_transaction.transaction_entry_id=transaction_entry.id "
                  "WHERE envelope_transaction.envelope_id=:id "
                  "ORDER BY transaction_entry.date DESC;");
    query.bindValue(":id", envelope.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
    }
    while (query.next()) {
        envelopes.push_back(toEnvelopeTransaction(query.record()));
    }
    return envelopes;
}

//--------------------------------------------------------------------------------------------------
QString SQLTransactionRepository::lastError() {
    return last_error_;
}

//--------------------------------------------------------------------------------------------------
bool SQLTransactionRepository::remove(const Transaction & transaction) {
    QSqlQuery query(db_);
    query.prepare("DELETE FROM transaction_entry WHERE id=:id;");
    query.bindValue(":id", transaction.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLTransactionRepository::remove(const AccountTransaction & transaction) {
    QSqlQuery query(db_);
    query.prepare("DELETE FROM account_transaction WHERE id=:id;");
    query.bindValue(":id", transaction.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLTransactionRepository::remove(const EnvelopeTransaction & transaction) {
    QSqlQuery query(db_);
    query.prepare("DELETE FROM envelope_transaction WHERE id=:id;");
    query.bindValue(":id", transaction.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLTransactionRepository::update(const Transaction & transaction) {
    QSqlQuery query(db_);
    query.prepare("UPDATE transaction_entry SET payee=:payee, date=strftime('%s', :date) "
                  " WHERE id=:id;");
    query.bindValue(":date", transaction.date().toString("yyyy-MM-dd"));
    query.bindValue(":payee", transaction.payee());
    query.bindValue(":id", transaction.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLTransactionRepository::update(const AccountTransaction & transaction) {
    QSqlQuery query(db_);
    query.prepare("UPDATE account_transaction SET account_id=:account, amount=:amount, "
                  "memo=:memo, cleared=:cleared, reconciliation_id=:reconciliation "
                  "WHERE id=:id;");
    query.bindValue(":transaction", transaction.transaction().id());
    query.bindValue(":account", transaction.account().id());
    query.bindValue(":amount", transaction.amount().scaled());
    query.bindValue(":memo", transaction.memo());
    query.bindValue(":cleared", transaction.isCleared());
    query.bindValue(":reconciliation", (transaction.reconciliation() > 0)
                    ? transaction.reconciliation() : QVariant());
    query.bindValue(":id", transaction.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
bool SQLTransactionRepository::update(const EnvelopeTransaction & transaction) {
    QSqlQuery query(db_);
    query.prepare("UPDATE envelope_transaction SET envelope_id=:envelope, amount=:amount, "
                  "memo=:memo WHERE id=:id;");
    query.bindValue(":transaction", transaction.transaction().id());
    query.bindValue(":envelope", transaction.envelope().id());
    query.bindValue(":amount", transaction.amount().scaled());
    query.bindValue(":memo", transaction.memo());
    query.bindValue(":id", transaction.id());
    if (not query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
AccountTransaction SQLTransactionRepository::toAccountTransaction(const QSqlRecord & record) {
    auto account = accounts_->getAccount(record.value("account_id").toInt());

    Transaction transaction(record.value("transaction_entry_id").toInt());
    transaction.setDate(QDate::fromString(record.value("date").toString(), "yyyy-MM-dd"));
    transaction.setPayee(record.value("payee").toString());

    AccountTransaction split(record.value("id").toInt());
    split.setAccount(account);
    split.setAmount(Money(record.value("amount").toInt(), account.currency()));
    split.setCleared(record.value("cleared").toBool());
    split.setMemo(record.value("memo").toString());
    auto rec = record.value("reconciliation_id");
    if (not rec.isNull()) {
        split.setReconciliation(rec.toInt());
    }
    split.setTransaction(transaction);

    return split;
}

//--------------------------------------------------------------------------------------------------
EnvelopeTransaction SQLTransactionRepository::toEnvelopeTransaction(const QSqlRecord & record) {
    auto envelope = envelopes_->getEnvelope(record.value("envelope_id").toInt());

    Transaction transaction(record.value("transaction_entry_id").toInt());
    transaction.setDate(QDate::fromString(record.value("date").toString(), "yyyy-MM-dd"));
    transaction.setPayee(record.value("payee").toString());

    EnvelopeTransaction split(record.value("id").toInt());
    split.setEnvelope(envelope);
    split.setAmount(Money(record.value("amount").toInt(), envelope.currency()));
    split.setMemo(record.value("memo").toString());
    split.setTransaction(transaction);

    return split;
}

} // ledger namespace
} // ub namespace
