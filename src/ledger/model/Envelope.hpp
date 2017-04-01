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
    explicit Envelope(int64_t id);

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
    std::vector<int64_t> children() const;

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
    int64_t id() const;

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
    int64_t parent() const;

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
    void setChildren(const std::vector<int64_t> & children);

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
    void setParent(int64_t id);

private:
    /** Is archived */
    bool archived_;
    /** Child IDs */
    std::vector<int64_t> children_;
    /** Currency */
    Currency currency_;
    /** Envelope ID */
    int64_t id_;
    /** Envelope name */
    QString name_;
    /** Parent ID */
    int64_t parent_;
};

} // ledger namespace
} // ub namespace
