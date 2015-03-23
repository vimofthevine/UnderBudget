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

#ifndef NOCONVERSIONRATES_HPP
#define NOCONVERSIONRATES_HPP

// UnderBudget include(s)
#include "accounting/ConversionRates.hpp"

namespace ub {

/**
 * Stand-in conversion rate lookup that always returns 1.0.
 *
 * @ingroup accounting
 */
class NoConversionRates : public ConversionRates
{
public:
	/**
	 * Destructor
	 */
	~NoConversionRates() { }

	/**
	 * Returns 1.0 regardless of given parameters.
	 *
	 * @param[in] source source currency as ISO 4217 code
	 * @param[in] target target currency as ISO 4217 code
	 * @return 1.0 for all given parameters
	 */
	double get(const QString& source, const QString& target) const
	{
		return 1.0;
	}
};

}

#endif //NOCONVERSIONRATES_HPP
