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
#include <map>
#include <memory>
#include <stdexcept>

// Qt include(s)
#include <QtDebug>
#include <QtSql>

// UnderBudget include(s)
#include <app/model/Repositories.hpp>
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountTransaction.hpp>
#include <ledger/model/Currency.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/model/EnvelopeTransaction.hpp>
#include <ledger/model/JournalEntry.hpp>
#include <ledger/model/Transaction.hpp>
#include <ledger/persistence/SQLAccountRepository.hpp>
#include <ledger/persistence/SQLCurrencyRepository.hpp>
#include <ledger/persistence/SQLEnvelopeRepository.hpp>
#include <ledger/persistence/SQLTransactionRepository.hpp>
#include "GnuCashImporter.hpp"

namespace ub {
namespace adapter {

//--------------------------------------------------------------------------------------------------
GnuCashImporter::GnuCashImporter(const QString & name, QObject * parent) : QThread(parent) {
    app_db_ = QSqlDatabase::cloneDatabase(QSqlDatabase::database(name), "GnuCashImport");
}

//--------------------------------------------------------------------------------------------------
GnuCashImporter::~GnuCashImporter() {
    terminated_ = true;
    wait();
    if (app_db_.isOpen()) {
        app_db_.close();
    }
    if (gnucash_db_.isOpen()) {
        gnucash_db_.close();
    }
}

//--------------------------------------------------------------------------------------------------
void GnuCashImporter::importFromSqlite(const QString & loc) {
    qDebug() << "Importing" << loc;
    gnucash_db_ = QSqlDatabase::addDatabase("QSQLITE", loc);
    gnucash_db_.setDatabaseName(loc);
    start();
}

//--------------------------------------------------------------------------------------------------
void GnuCashImporter::run() {
    try {
        emit message(tr("Importing from %1").arg(gnucash_db_.databaseName()));
        emit progress(0, 0);

        if (not app_db_.open()) {
            throw std::runtime_error(
                tr("Unable to open database: %1").arg(app_db_.lastError().text()).toStdString());
        }

        if (not gnucash_db_.open()) {
            throw std::runtime_error(tr("Unable to open GnuCash database (%1): %2")
                                         .arg(gnucash_db_.databaseName())
                                         .arg(gnucash_db_.lastError().text())
                                         .toStdString());
        }

        account_repo_.reset(new ledger::SQLAccountRepository(app_db_));
        currency_repo_.reset(new ledger::SQLCurrencyRepository(app_db_));
        auto envelope_repo = std::make_shared<ledger::SQLEnvelopeRepository>(app_db_);
        transaction_repo_.reset(
            new ledger::SQLTransactionRepository(app_db_, account_repo_, envelope_repo));

        // Import currencies
        {
            QSqlQuery query(gnucash_db_);
            QSqlRecord record;
            if (not query.exec("SELECT * FROM commodities WHERE namespace=\"CURRENCY\";")) {
                throw std::runtime_error(query.lastError().text().toStdString());
            }
            while (query.next()) {
                record = query.record();
                ledger::Currency currency(0, record.value("mnemonic").toString(),
                                          record.value("guid").toString());
                auto id = currency_repo_->create(currency);
                currencies_[currency.externalId()] = currency_repo_->getCurrency(id);
            }
        }

        // Import accounts
        {
            QSqlQuery query(gnucash_db_);
            query.exec("SELECT * FROM accounts WHERE name=\"Root Account\";");
            if (query.first()) {
                auto record = query.record();
                auto ext_id = record.value("guid").toString();
                accounts_[ext_id] = account_repo_->getAccount(1);
                importChildAccountsOf(ext_id);
            } else {
                throw std::runtime_error(query.lastError().text().toStdString());
            }
        }

        // Import transactions
        {
            QSqlQuery query(gnucash_db_);
            int num = -1;
            if (query.exec("SELECT count(guid) FROM transactions;") and query.first()) {
                num = query.record().value(0).toInt();
            }
            if (query.exec("SELECT * FROM transactions;")) {
                while (query.next()) {
                    if (terminated_) {
                        break;
                    }
                    importTransaction(query.record());
                    ++num_transactions_;
                    if (num > 0) {
                        emit progress(num_transactions_, num);
                    }
                }
            } else {
                throw std::runtime_error(query.lastError().text().toStdString());
            }
        }

        emit message(tr("Import complete"));
        emit finished(not terminated_);
        emit progress(1, 1);
        if (not terminated_) {
            qDebug() << "Imported" << currencies_.size() << "currencies," << accounts_.size()
                     << "accounts, and" << num_transactions_ << "transactions";
        }
    } catch (std::runtime_error & e) {
        qWarning() << e.what();
        emit error(e.what());
        emit finished(false);
        emit progress(1, 1);
    }

    if (app_db_.isOpen()) {
        app_db_.close();
    }
    if (gnucash_db_.isOpen()) {
        gnucash_db_.close();
    }
}

//--------------------------------------------------------------------------------------------------
void GnuCashImporter::importTransaction(QSqlRecord trn_record) {
    ledger::JournalEntry je(transaction_repo_);
    auto currency = currencies_[trn_record.value("currency_guid").toString()];

    auto trn_id = trn_record.value("guid").toString();
    ledger::Transaction transaction;
    transaction.setPayee(trn_record.value("description").toString());
    transaction.setDate(
        QDateTime::fromString(trn_record.value("post_date").toString(), "yyyy-MM-dd HH:mm:ss")
            .date());
    // TODO transaction external ID
    je.updateTransaction(transaction);

    QSqlQuery query(gnucash_db_);
    query.prepare("SELECT * FROM splits WHERE tx_guid=:id;");
    query.bindValue(":id", trn_id);
    if (query.exec()) {
        while (query.next()) {
            auto record = query.record();
            auto split_id = record.value("guid").toString();
            auto acct_id = record.value("account_guid").toString();
            auto num = record.value("value_num").toDouble();
            auto denom = record.value("value_denom").toDouble();
            auto amount = num / denom;
            if (accounts_.count(acct_id) == 1u) {
                ledger::AccountTransaction split;
                split.setAccount(accounts_[acct_id]);
                split.setAmount(ledger::Money(amount, currency));
                bool cleared = (record.value("reconcile_state").toString() != "n");
                split.setCleared(cleared);
                split.setMemo(record.value("memo").toString());
                split.setTransaction(transaction);
                je.addSplit(split);
                qDebug() << "Importing account split" << split.amount().toString()
                         << split.account().name();
            } else {
                qDebug() << "Unknown account ID" << acct_id;
            }
        }
    } else {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    if (not je.isValid()) {
        throw std::runtime_error(tr("Journal entry for transaction %1 is not valid: %2")
                                     .arg(trn_id)
                                     .arg(je.lastError())
                                     .toStdString());
    } else {
        if (not je.save()) {
            throw std::runtime_error(tr("Unable to save journal entry for transaction %1: %2")
                                         .arg(trn_id)
                                         .arg(je.lastError())
                                         .toStdString());
        }
    }
}

//--------------------------------------------------------------------------------------------------
void GnuCashImporter::importChildAccountsOf(const QString & parent_ext_id) {
    if (terminated_) {
        return;
    }

    QSqlQuery query(gnucash_db_);
    query.prepare("SELECT * FROM accounts WHERE parent_guid=:id;");
    query.bindValue(":id", parent_ext_id);
    if (not query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
    while (query.next()) {
        auto record = query.record();
        auto ext_id = record.value("guid").toString();
        auto type = record.value("account_type").toString();

        ledger::Account account;
        account.setCurrency(currencies_[record.value("commodity_guid").toString()]);
        account.setExternalId(ext_id);
        account.setName(record.value("name").toString());
        account.setParent(accounts_[parent_ext_id].id());
        auto id = account_repo_->create(account, accounts_[parent_ext_id]);
        accounts_[ext_id] = account_repo_->getAccount(id);

        importChildAccountsOf(ext_id);
    }
}

} // namespace adapter
} // namespace ub
