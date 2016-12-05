#pragma once

// Qt include(s)
#include <QString>

namespace ub {
namespace ledger {

/**
 * Model of a single monetary currency.
 *
 * @ingroup ledger
 */
class Currency {
public:
    /**
     * Initializes a currency for the current locale.
     */
    Currency();

    /**
     * Initializes a currency with the given ISO 4217 code.
     *
     * @param[in] code ISO 4217 currency code
     */
    Currency(const QString & code);

    /**
     * Initializes a currency with the given ISO 4217 code.
     *
     * This allows for literal strings to be used easily.
     *
     * @param[in] code ISO 4217 currency code
     */
    //Currency(const QString & code);

    // Copy constructor
    Currency(const Currency & orig) = default;

    // Assignment operator
    Currency & operator=(const Currency & orig) = default;

    /**
     * Determines if the given currency and this currency are equivalent.
     *
     * @param[in] that Currency to be checked for equality
     * @return @c true if the currencies are the same
     */
    bool operator==(const Currency & that) const;

    /**
     * Determines if the given currency and this currency are not equivalent.
     *
     * @param[in] that Currency to be checked for inequality
     * @return @c true if the currencies are different
     */
    bool operator!=(const Currency & that) const;

    /**
     * Returns the ISO 4217 code for this currency.
     *
     * @return ISO 4217 code
     */
    QString code() const;

    /**
     * Formats the given value according to this currency and the current locale.
     *
     * @param[in] value Decimal value to be formatted
     */
    QString format(double value) const;

    /**
     * Returns the UTF symbol for this currency.
     *
     * @return UTF symbol
     */
    QString symbol() const;

private:
    /** ISO 4217 currency code */
    QString iso4217_;
};

} // ledger namespace
} // ub namespace

// Make Currency known to QMetaType
Q_DECLARE_METATYPE(ub::ledger::Currency)
