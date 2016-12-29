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
