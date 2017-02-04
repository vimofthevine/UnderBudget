#pragma once

// Standard include(s)
#include <memory>

namespace ub {
namespace ledger {

// Forward declaration(s)
class AccountRepository;
class CurrencyRepository;
class EnvelopeRepository;
class TransactionRepository;

/**
 * Repository of ledger repositories.
 */
class LedgerRepository {
public:
    // Virtual destructor
    virtual ~LedgerRepository() {}

    /**
     * Returns a pointer to the account repository.
     *
     * @return Account repository
     */
    virtual std::shared_ptr<AccountRepository> accounts() const = 0;

    /**
     * Returns a pointer to the currency repository.
     *
     * @return Currency repository
     */
    virtual std::shared_ptr<CurrencyRepository> currencies() const = 0;

    /**
     * Returns a pointer to the envelope repository.
     *
     * @return Envelope repository
     */
    virtual std::shared_ptr<EnvelopeRepository> envelopes() const = 0;

    /**
     * Returns a pointer to the transaction repository.
     *
     * @return Transaction repository
     */
    virtual std::shared_ptr<TransactionRepository> transactions() const = 0;
};

} // ledger namespace
} // ub namespace
