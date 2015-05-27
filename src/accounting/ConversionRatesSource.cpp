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

// UnderBudget include(s)
#include "accounting/ConversionRatesSource.hpp"
#include "accounting/NoConversionRates.hpp"

namespace ub {

//------------------------------------------------------------------------------
QSharedPointer<ConversionRates> ConversionRatesSource::current =
	QSharedPointer<ConversionRates>();

//------------------------------------------------------------------------------
QSharedPointer<ConversionRates> ConversionRatesSource::factory()
{
	if (current.isNull())
	{
		set(QSharedPointer<ConversionRates>(new NoConversionRates));
	}

	return current;
}

//------------------------------------------------------------------------------
void ConversionRatesSource::set(ConversionRates* service)
{
	set(QSharedPointer<ConversionRates>(service));
}

//------------------------------------------------------------------------------
void ConversionRatesSource::set(const QSharedPointer<ConversionRates>& service)
{
	current = service;
}

}

