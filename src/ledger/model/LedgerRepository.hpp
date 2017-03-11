/*
 * Copyright 2017 Kyle Treubig
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
