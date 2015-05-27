/*
 * Copyright 2013 Kyle Treubig
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

#ifndef CURRENCY_HPP
#define CURRENCY_HPP

// Qt include(s)
#include <QLocale>
#include <QString>

// UnderBudget include(s)
#include "accounting/ConversionRates.hpp"

namespace ub {

/**
 * Model of a single monetary currency.
 *
 * @ingroup accounting
 */
class Currency
{
public:
	/**
	 * Constructs a new currency instance for the default
	 * currency, based on locale.
	 */
	Currency();

	/**
	 * Constructs a new currency instance for the specified currency.
	 *
	 * @param[in] code ISO 4217 currency code
	 */
	Currency(const QString& code);

	/**
	 * Constructs a new currency instance for the specified currency.
	 *
	 * This constructor is provided to cover cases in which a Currency
	 * value is required as a parameter to a function, to allow specifying
	 * of that parameter as a literal string (e.g., `Money(123.45, "USD")`).
	 *
	 * @param[in] code ISO 4217 currency code
	 */
	Currency(const char* code);

	/**
	 * Constructs a copy of the given currency.
	 *
	 * @param[in] orig original currency to be copied
	 */
	Currency(const Currency& orig);

	/**
	 * Retrieves the currency associated with the given locale, or
	 * the default locale if none is specified.
	 *
	 * @param[in] locale locale for which to retrieve its currency
	 */
	static Currency byLocale(const QLocale& locale = QLocale());

	/**
	 * Returns the ISO 4217 code for this currency.
	 */
	const QString& code() const;

	/**
	 * Returns the UTF symbol for this currency.
	 */
	QString symbol() const;

	/**
	 * Formats the given value according to this currency
	 * and the current locale.
	 *
	 * @param[in] value currency value to be formatted
	 */
	QString format(double value) const;

	/**
	 * Retrieves the conversion rate necessary to convert a monetary
	 * amount of this currency to the given target currency.
	 *
	 * @param[in] target target currency
	 */
	double conversionRate(const Currency& target) const;

	/**
	 * Retrieves the conversion rate necessary to convert a monetary
	 * amount of this currency to the given target currency, using
	 * the given source of conversion rates.
	 *
	 * @param[in] target target currency
	 * @param[in] rates  conversion rates source
	 */
	double conversionRate(const Currency& target,
		ConversionRates& rates) const;

	/**
	 * Checks if the given currency and this currency are
	 * the same.
	 *
	 * @param[in] that currency to be checked for equality
	 */
	bool operator==(const Currency& that) const;

	/**
	 * Checks if the given currency and this currency are
	 * different.
	 *
	 * @param[in] that currency to be checked for inequality
	 */
	bool operator!=(const Currency& that) const;

private:
	/**
	 * ISO 4217 currency code
	 */
	QString iso4217;
};

}

// Make Currency known to QMetaType
Q_DECLARE_METATYPE(ub::Currency)

#endif // CURRENCY_HPP
