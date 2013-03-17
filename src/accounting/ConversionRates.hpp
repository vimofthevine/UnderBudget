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

#ifndef CONVERSIONRATES_HPP
#define CONVERSIONRATES_HPP

namespace ub {

/**
 * Currency conversion rate lookup service.
 *
 * The actual implementation may obtain these rates from a file, a web service,
 * etc. Although clients may use this service directly, it is not recommended.
 * This rate lookup service is used internally by the `Currency` class to
 * provide automatic conversion when performing arithmetic operations on
 * `Money` objects.
 *
 * @ingroup accounting
 */
class ConversionRates
{
public:
	/**
	 * Virtual destructor.
	 */
	virtual ~ConversionRates() { }

	/**
	 * Retrieves the conversion rate from the source currency to the target
	 * currency. Currencies are specifed with their ISO 4217 code.
	 *
	 * @param[in] source source currency as ISO 4217 code
	 * @param[in] target target currency as ISO 4217 code
	 * @return conversion rate from source currency to target currency, or
	 *         1.0 if no rate is found (no conversion)
	 */
	virtual double get(const QString& source, const QString& target) const = 0;
};

}

#endif // CONVERSIONRATES_HPP
