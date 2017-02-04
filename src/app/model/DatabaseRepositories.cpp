// Standard include(s)
#include <memory>

// Qt include(s)
#include <QtCore>
#include <QtSql>

// UnderBudget include(s)
#include "DatabaseRepositories.hpp"
#include <ledger/persistence/SQLAccountRepository.hpp>
#include <ledger/persistence/SQLCurrencyRepository.hpp>
#include <ledger/persistence/SQLEnvelopeRepository.hpp>
#include <ledger/persistence/SQLTransactionRepository.hpp>

namespace ub {

//--------------------------------------------------------------------------------------------------
DatabaseRepositories::DatabaseRepositories(const QString &loc) : location_(loc) {
    db_ = QSqlDatabase::addDatabase("QSQLITE", loc);
    db_.setDatabaseName(loc);
    if (not db_.open()) {
        qCritical() << "Unable to open database file:" << loc << "error:" << db_.lastError();
    } else {
        qDebug() << "Opened database file:" << loc;
        currencies_.reset(new ledger::SQLCurrencyRepository(db_));
        accounts_.reset(new ledger::SQLAccountRepository(db_));
        envelopes_.reset(new ledger::SQLEnvelopeRepository(db_));
        transactions_.reset(new ledger::SQLTransactionRepository(db_, accounts_, envelopes_));
    }
}

//--------------------------------------------------------------------------------------------------
DatabaseRepositories::~DatabaseRepositories() {
    if (db_.isOpen()) {
        db_.close();
    }
}

//--------------------------------------------------------------------------------------------------
bool DatabaseRepositories::isOpen() const {
    return db_.isOpen();
}

//--------------------------------------------------------------------------------------------------
std::string DatabaseRepositories::location() const {
    return location_.toStdString();
}

//--------------------------------------------------------------------------------------------------
std::shared_ptr<ledger::AccountRepository> DatabaseRepositories::accounts() const {
    return accounts_;
}

//--------------------------------------------------------------------------------------------------
std::shared_ptr<ledger::CurrencyRepository> DatabaseRepositories::currencies() const {
    return currencies_;
}

//--------------------------------------------------------------------------------------------------
std::shared_ptr<ledger::EnvelopeRepository> DatabaseRepositories::envelopes() const {
    return envelopes_;
}

//--------------------------------------------------------------------------------------------------
std::shared_ptr<ledger::TransactionRepository> DatabaseRepositories::transactions() const {
    return transactions_;
}

} // ub namespace
