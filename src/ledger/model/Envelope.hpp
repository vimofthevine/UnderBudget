#pragma once

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

    /**
     * Returns the category of this envelope.
     *
     * @return Category of this envelope
     */
    QString category() const;

    /**
     * Returns the currency of this envelope.
     *
     * @return Currency of this envelope
     */
    Currency currency() const;

    /**
     * Returns the current balance of this envelope.
     *
     * @return Current balance
     */
    Money currentBalance() const;

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
     * Updates the category of this envelope.
     *
     * @param[in] category New Category
     */
    void setCategory(const QString & category);

    /**
     * Updates the currency of this envelope.
     *
     * @param[in] currency New currency
     */
    void setCurrency(const Currency & currency);

    /**
     * Updates the current balance of this envelope.
     *
     * @param[in] balance New current balance
     * @throw std::invalid_argument if the balance currency does not match the
     *        envelope currency
     */
    void setCurrentBalance(const Money & balance);

    /**
     * Updates the name of this envelope.
     *
     * @param[in] name New envelope name
     */
    void setName(const QString & name);

private:
    /** Category */
    QString category_;
    /** Currency */
    Currency currency_;
    /** Current balance */
    Money current_balance_;
    /** Envelope ID */
    int id_;
    /** Envelope name */
    QString name_;
};

} // ledger namespace
} // ub namespace
