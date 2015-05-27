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

#ifndef CONVERSIONRATESSOURCE_HPP
#define CONVERSIONRATESSOURCE_HPP

// Qt include(s)
#include <QSharedPointer>

// UnderBudget include(s)
#include "accounting/ConversionRates.hpp"

namespace ub {

/**
 * Global source of `ConversionRates`.
 *
 * @ingroup accounting
 */
class ConversionRatesSource
{
public:
	/**
	 * Returns the current conversion rates lookup service.
	 *
	 * @return active `ConversionRates` instance
	 */
	static QSharedPointer<ConversionRates> factory();

	/**
	 * Specifies the conversion rates lookup service. Ownership of the
	 * pointer is transfered to the `ConversionRatesSource` class.
	 *
	 * @param[in] service active `ConversionRates` instance
	 */
	static void set(ConversionRates* service);

	/**
	 * Specifies the conversion rates lookup service.
	 *
	 * @param[in] service active `ConversionRates` instance
	 */
	static void set(const QSharedPointer<ConversionRates>& service);

private:
	/**
	 * The current `ConversionRates` instance
	 */
	static QSharedPointer<ConversionRates> current;
};

}

#endif //CONVERSIONRATESSOURCE_HPP
