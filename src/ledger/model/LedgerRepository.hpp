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

#pragma once

// Standard include(s)
#include <memory>

// UnderBudget include(s)
#include "AccountRepository.hpp"
#include "CurrencyRepository.hpp"
#include "EnvelopeRepository.hpp"
#include "ReconciliationRepository.hpp"
#include "TransactionRepository.hpp"

namespace ub {
namespace ledger {

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
     * Returns a pointer to the reconciliation repository.
     *
     * @return Reconciliation repository
     */
    virtual std::shared_ptr<ReconciliationRepository> reconciliations() const = 0;

    /**
     * Returns a pointer to the transaction repository.
     *
     * @return Transaction repository
     */
    virtual std::shared_ptr<TransactionRepository> transactions() const = 0;
};

} // ledger namespace
} // ub namespace
