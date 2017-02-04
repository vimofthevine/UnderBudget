#pragma once

// Standard include(s)
#include <memory>

// Qt include(s)
#include <QString>

// UnderBudget include(s)
#include <ledger/model/LedgerRepository.hpp>

namespace ub {

/**
 * Collection of repositories for the application.
 */
class Repositories : public ledger::LedgerRepository {
public:
    // Virtual destructor
    virtual ~Repositories() {}

    /**
     * Returns a description of the source of the repositories.
     *
     * @return Repository source location
     */
    virtual QString location() const = 0;
};
} // ub namespace
