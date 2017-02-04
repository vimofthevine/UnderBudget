#pragma once

// Standard include(s)
#include <memory>

// Qt include(s)
#include <QSqlDatabase>
#include <QString>

// UnderBudget include(s)
#include "Repositories.hpp"

namespace ub {

namespace ledger {
// Forward declaration(s)
class AccountRepository;
class CurrencyRepository;
class EnvelopeRepository;
class TransactionRepository;
class SQLAccountRepository;
class SQLCurrencyRepository;
class SQLEnvelopeRepository;
class SQLTransactionRepository;
}

/**
 * Collection of SQL repositories acting on a shared database.
 */
class DatabaseRepositories : public Repositories {
public:
    /**
     * Initializes the database repositories for the specified database file.
     *
     * @param loc Database file location
     */
    DatabaseRepositories(const QString &loc);

    /**
     * Closes the database.
     */
    ~DatabaseRepositories();

    /**
     * Checks if the database was opened successfully.
     *
     * @return @c true if database was successfully opened
     */
    bool isOpen() const;

    std::string location() const override;

    std::shared_ptr<ledger::AccountRepository> accounts() const override;

    std::shared_ptr<ledger::CurrencyRepository> currencies() const override;

    std::shared_ptr<ledger::EnvelopeRepository> envelopes() const override;

    std::shared_ptr<ledger::TransactionRepository> transactions() const override;

private:
    /** Database location */
    QString location_;
    /** Database connection */
    QSqlDatabase db_;
    /** Account repository */
    std::shared_ptr<ledger::SQLAccountRepository> accounts_;
    /** Currency repository */
    std::shared_ptr<ledger::SQLCurrencyRepository> currencies_;
    /** Envelope repository */
    std::shared_ptr<ledger::SQLEnvelopeRepository> envelopes_;
    /** Transaction repository */
    std::shared_ptr<ledger::SQLTransactionRepository> transactions_;
};
} // ub namespace
