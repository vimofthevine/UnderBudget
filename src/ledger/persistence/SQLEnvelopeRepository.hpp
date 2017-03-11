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
    int create(const Envelope & envelope, const Envelope & parent) override;

    /**
     * Retrieves an envelope from the repository with the given ID.
     *
     * If no envelope exists for the given ID, an envelope with a -1 ID will be returned.
     *
     * @param[in] id Envelope ID to be retrieved
     * @return Envelope for the given ID
     */
    Envelope getEnvelope(int id) override;

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
        int lft = 0;
        /** Right value */
        int rgt = 0;

        /**
         * Initializes an envelope with default parameters.
         */
        NestedSetEnvelope() : Envelope() {}

        /**
         * Initializes an envelope with the given ID.
         *
         * @param[in] id Envelope ID
         */
        NestedSetEnvelope(int id) : Envelope(id) {}
    };

    /** Database connection */
    QSqlDatabase db_;
    /** Last error message */
    QString last_error_;
    /** Cached envelopes */
    std::map<int, NestedSetEnvelope> envelopes_;

    /**
     * Populates the cache with all envelope content from the database.
     */
    void cache();
};

} // ledger namespace
} // ub namespace
