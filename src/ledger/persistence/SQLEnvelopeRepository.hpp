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
#include <map>

// Qt include(s)
#include <QSqlDatabase>
#include <QString>

// UnderBudget include(s)
#include <ledger/model/CurrencyRepository.hpp>
#include <ledger/model/Envelope.hpp>
#include <ledger/model/EnvelopeRepository.hpp>

namespace ub {
namespace ledger {

/**
 * Envelope repository implementation that uses an SQLite database.
 *
 * @ingroup ledger
 */
class SQLEnvelopeRepository : public EnvelopeRepository {
public:
    /**
     * Initializes the repository with the given database connection.
     *
     * This will set up the envelope table and add an entry for the root
     * envelope if one does not exist.
     *
     * @param[in] db SQL database connection
     * @throw std::runtime_error if the database table could not be set up
     */
    SQLEnvelopeRepository(QSqlDatabase & db);

    /**
     * Creates a new envelope as a child of the specified parent envelope.
     *
     * @param[in] envelope Envelope to be created
     * @param[in] parent   Envelope under which to create the new envelope
     * @return ID assigned to the newly created envelope
     */
    int64_t create(const Envelope & envelope, const Envelope & parent) override;

    /**
     * Retrieves an envelope from the repository with the given ID.
     *
     * If no envelope exists for the given ID, an envelope with a -1 ID will be returned.
     *
     * @param[in] id Envelope ID to be retrieved
     * @return Envelope for the given ID
     */
    Envelope getEnvelope(int64_t id) override;

    /**
     * Retrieves all leaf envelopes.
     *
     * @return List of all leaf envelopes
     */
    std::vector<Envelope> getLeafEnvelopes() override;

    /**
     * Retrieves the root envelope from the repository.
     *
     * @return Root envelope
     * @throw std::invalid_argument if no root envelope exists
     */
    Envelope getRoot() override;

    /**
     * Returns the message of the last error to have occurred.
     *
     * @return Last error message
     */
    QString lastError() override;

    /**
     * Moves the given envelope to the specified parent envelope.
     *
     * @param[in] envelope Envelope to be moved
     * @param[in] parent   New parent envelope
     * @return @c true if successful
     */
    bool move(const Envelope & envelope, const Envelope & parent) override;

    /**
     * Removes the given envelope from the repository.
     *
     * The ID is the only parameter required to match the existing entry.
     *
     * @param envelope Envelope to be removed
     * @return @c true if successful
     */
    bool remove(const Envelope & envelope) override;

    /**
     * Updates the given envelope in the repository.
     *
     * The ID must match an existing entry. All other parameters will be updated.
     *
     * @param envelope Envelope to be updated
     * @return @c true if successful
     */
    bool update(const Envelope & envelope) override;

private:
    /** Currency table name */
    static QString table_name_;

    /** Envelope with additional nested set model fields */
    class NestedSetEnvelope : public Envelope {
    public:
        /** Left value */
        int64_t lft = 0;
        /** Right value */
        int64_t rgt = 0;

        /**
         * Initializes an envelope with default parameters.
         */
        NestedSetEnvelope() : Envelope() {}

        /**
         * Initializes an envelope with the given ID.
         *
         * @param[in] id Envelope ID
         */
        NestedSetEnvelope(int64_t id) : Envelope(id) {}
    };

    /** Database connection */
    QSqlDatabase db_;
    /** Last error message */
    QString last_error_;
    /** Cached envelopes */
    std::map<int64_t, NestedSetEnvelope> envelopes_;

    /**
     * Populates the cache with all envelope content from the database.
     */
    void cache();
};

} // ledger namespace
} // ub namespace
