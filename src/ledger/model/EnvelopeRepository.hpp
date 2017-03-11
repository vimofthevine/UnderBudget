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
#include <vector>

// Qt include(s)
#include <QString>

// UnderBudget include(s)
#include "Envelope.hpp"

namespace ub {
namespace ledger {

/**
 * Repository of envelopes.
 *
 * @ingroup ledger
 */
class EnvelopeRepository {
public:
    // Virtual destructor
    virtual ~EnvelopeRepository() {}

    /**
     * Creates a new envelope as a child of the specified parent envelope.
     *
     * @param[in] envelope Envelope to be created
     * @param[in] parent   Envelope under which to create the new envelope
     * @return ID assigned to the newly created envelope
     */
    virtual int create(const Envelope & envelope, const Envelope & parent) = 0;

    /**
     * Retrieves an envelope from the repository with the given ID.
     *
     * If no envelope exists for the given ID, an envelope with a -1 ID will be returned.
     *
     * @param[in] id Envelope ID to be retrieved
     * @return Envelope for the given ID
     */
    virtual Envelope getEnvelope(int id) = 0;

    /**
     * Retrieves all leaf envelopes.
     *
     * @return List of all leaf envelopes
     */
    virtual std::vector<Envelope> getLeafEnvelopes() = 0;

    /**
     * Retrieves the root envelope from the repository.
     *
     * @return Root envelope
     * @throw std::invalid_argument if no root envelope exists
     */
    virtual Envelope getRoot() = 0;

    /**
     * Returns the message of the last error to have occurred.
     *
     * @return Last error message
     */
    virtual QString lastError() = 0;

    /**
     * Moves the given envelope to the specified parent envelope.
     *
     * @param[in] envelope Envelope to be moved
     * @param[in] parent   New parent envelope
     * @return @c true if successful
     */
    virtual bool move(const Envelope & envelope, const Envelope & parent) = 0;

    /**
     * Removes the given envelope from the repository.
     *
     * The ID is the only parameter required to match the existing entry.
     *
     * @param envelope Envelope to be removed
     * @return @c true if successful
     */
    virtual bool remove(const Envelope & envelope) = 0;

    /**
     * Updates the given envelope in the repository.
     *
     * The ID must match an existing entry. All other parameters will be updated.
     *
     * @param envelope Envelope to be updated
     * @return @c true if successful
     */
    virtual bool update(const Envelope & envelope) = 0;
};

} // ledger namespace
} // ub namespace
