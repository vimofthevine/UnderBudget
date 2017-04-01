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
#include <cstdint>
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
    virtual int64_t create(const Envelope & envelope, const Envelope & parent) = 0;

    /**
     * Retrieves an envelope from the repository with the given ID.
     *
     * If no envelope exists for the given ID, an envelope with a -1 ID will be returned.
     *
     * @param[in] id Envelope ID to be retrieved
     * @return Envelope for the given ID
     */
    virtual Envelope getEnvelope(int64_t id) = 0;

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
