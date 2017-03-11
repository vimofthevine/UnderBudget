/*
 * Copyright 2016 Kyle Treubig
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
#include <cstdint>

// Qt include(s)
#include <QString>

// UnderBudget include(s)
#include "Currency.hpp"

namespace ub {
namespace ledger {

/**
 * Repository of currencies.
 *
 * @ingroup ledger
 */
class CurrencyRepository {
public:
    // Virtual destructor
    virtual ~CurrencyRepository() {}

    /**
     * Creates a new currency in the repository with the parameters of the given currency.
     *
     * @param[in] currency Currency to be created
     * @return ID assigned to the newly created currency
     */
    virtual int64_t create(const Currency & currency) = 0;

    /**
     * Retrieves a currency from the repository with the given ID.
     *
     * @param[in] id Currency ID to be retrieved
     * @return Currency for the given ID
     * @throw std::invalid_argument if no currency exists for the given ID
     */
    virtual Currency getCurrency(int64_t id) = 0;

    /**
     * Returns the message of the last error to have occurred.
     *
     * @return Last error message
     */
    virtual QString lastError() = 0;

    /**
     * Removes the given currency from the repository.
     *
     * The ID is the only parameter required to match the existing entry.
     *
     * @param currency Currency to be removed
     * @return @c true if successful
     */
    virtual bool remove(const Currency & currency) = 0;

    /**
     * Updates the given currency in the repository.
     *
     * The ID must match an existing entry. All other parameters will be updated.
     *
     * Only the default (ID 1) currency may be updated. All others are unmodifiable.
     *
     * @param currency Currency to be updated
     * @return @c true if successful
     * @throw std::invalid_argument if currency is not the default (ID 1) currency
     */
    virtual bool update(const Currency & currency) = 0;
};

} // ledger namespace
} // ub namespace
