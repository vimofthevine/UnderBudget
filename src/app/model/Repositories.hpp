#pragma once

// Standard include(s)
#include <memory>

// Qt include(s)
#include <QString>

namespace ub {

namespace ledger {
// Forward declaration(s)
class AccountRepository;
class CurrencyRepository;
class EnvelopeRepository;
class TransactionRepository;
}

/**
 * Collection of repositories for the application.
 */
class Repositories {
public:
    // Virtual destructor
    virtual ~Repositories() {}

    /**
     * Returns a description of the source of the repositories.
     *
     * @return Repository source location
     */
    virtual QString location() const = 0;

    /**
     * Returns a pointer to the account repository.
     *
     * @return Account repository
     */
    virtual std::shared_ptr<ledger::AccountRepository> accounts() const = 0;

    /**
     * Returns a pointer to the currency repository.
     *
     * @return Currency repository
     */
    virtual std::shared_ptr<ledger::CurrencyRepository> currencies() const = 0;

    /**
     * Returns a pointer to the envelope repository.
     *
     * @return Envelope repository
     */
    virtual std::shared_ptr<ledger::EnvelopeRepository> envelopes() const = 0;

    /**
     * Returns a pointer to the transaction repository.
     *
     * @return Transaction repository
     */
    virtual std::shared_ptr<ledger::TransactionRepository> transactions() const = 0;
};
} // ub namespace
