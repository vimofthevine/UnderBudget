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

// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "accounting/ConversionRatesSource.hpp"
#include "accounting/Currency.hpp"

namespace ub {

//------------------------------------------------------------------------------
Currency::Currency()
	: iso4217(byLocale().iso4217)
{ }

//------------------------------------------------------------------------------
Currency::Currency(const QString& code)
	: iso4217(code)
{ }

//------------------------------------------------------------------------------
Currency::Currency(const char* code)
	: iso4217(code)
{ }

//------------------------------------------------------------------------------
Currency::Currency(const Currency& orig)
	: iso4217(orig.iso4217)
{ }

//------------------------------------------------------------------------------
Currency Currency::byLocale(const QLocale& locale)
{
	return Currency(locale.currencySymbol(QLocale::CurrencyIsoCode));
}

//------------------------------------------------------------------------------
const QString& Currency::code() const
{
	return iso4217;
}

//------------------------------------------------------------------------------
QString Currency::format(double value) const
{
	Currency local = byLocale();
	if (*this == local)
	{
		return QLocale().toCurrencyString(value);
	}
	else
	{
		return QLocale().toCurrencyString(value, iso4217);
	}
}

//------------------------------------------------------------------------------
double Currency::conversionRate(const Currency& target) const
{
	if (*this == target)
	{
		return 1.0;
	}
	else
	{
		QSharedPointer<ConversionRates> rates = ConversionRatesSource::factory();
		return conversionRate(target, *rates);
	}
}

//------------------------------------------------------------------------------
double Currency::conversionRate(const Currency& target,
	ConversionRates& rates) const
{
	return rates.get(iso4217, target.iso4217);
}

//------------------------------------------------------------------------------
bool Currency::operator==(const Currency& that) const
{
	return (iso4217 == that.iso4217);
}

//------------------------------------------------------------------------------
bool Currency::operator!=(const Currency& that) const
{
	return (iso4217 != that.iso4217);
}

}

