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
#include "Currency.hpp"
#include "Money.hpp"

namespace ub {
namespace ledger {

/**
 * An envelope contains a portion of the user's available funds, as designated towards a particular
 * use.
 */
class Envelope {
public:
    /**
     * Initializes an envelope with default values.
     */
    Envelope();

    /**
     * Initializes an envelope with the given ID.
     *
     * @param[in] id Envelope ID
     */
    explicit Envelope(int id);

    // Copy constructor
    Envelope(const Envelope & orig) = default;

    // Assignment operator
    Envelope & operator=(const Envelope & orig) = default;

    /**
     * Returns whether the envelope has been archived.
     *
     * @return @c true if the envelope has been archived
     */
    bool archived() const;

    /**
     * Returns the IDs of the children of this envelope.
     *
     * @return Children envelope IDs
     */
    std::vector<int> children() const;

    /**
     * Returns the currency of this envelope.
     *
     * @return Currency of this envelope
     */
    Currency currency() const;

    /**
     * Returns the ID of this envelope.
     *
     * @return Envelope ID
     */
    int id() const;

    /**
     * Returns the name of this envelope.
     *
     * @return Envelope name
     */
    QString name() const;

    /**
     * Returns the ID of the parent to this envelope.
     *
     * @return Parent envelope ID
     */
    int parent() const;

    /**
     * Updates the archive state of this envelope.
     *
     * @param[in] archive @c true if archived
     */
    void setArchived(bool archived);

    /**
     * Updates the children of this envelope.
     *
     * @param[in] children New children IDs
     */
    void setChildren(const std::vector<int> & children);

    /**
     * Updates the currency of this envelope.
     *
     * @param[in] currency New currency
     */
    void setCurrency(const Currency & currency);

    /**
     * Updates the name of this envelope.
     *
     * @param[in] name New envelope name
     */
    void setName(const QString & name);

    /**
     * Updates the parent ID of this envelope.
     *
     * @param[in] id New parent ID
     */
    void setParent(int id);

private:
    /** Is archived */
    bool archived_;
    /** Child IDs */
    std::vector<int> children_;
    /** Currency */
    Currency currency_;
    /** Envelope ID */
    int id_;
    /** Envelope name */
    QString name_;
    /** Parent ID */
    int parent_;
};

} // ledger namespace
} // ub namespace
