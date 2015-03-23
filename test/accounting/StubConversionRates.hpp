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

#ifndef STUBCONVERSIONRATES_HPP
#define STUBCONVERSIONRATES_HPP

#include "accounting/ConversionRates.hpp"

namespace ub {

/**
 * Stub implementation of `ConversionRates`.
 */
class StubConversionRates : public ConversionRates
{
public:
	/** Constructor */
	StubConversionRates(double rate = 1.0) : match(false), rate(rate) { }
	/** Constructor */
	StubConversionRates(double rate, const QString& source, const QString& target)
		: match(true), source(source), target(target), rate(rate) { }
	/** Destructor */
	~StubConversionRates() { }
	/** Rate accessor */
	double get(const QString& source, const QString& target) const
	{
		if (match)
		{
			if (source == this->source && target == this->target)
				return rate;
			else
				return 1.0;
		}
		else
		{
			return rate;
		}
	}

private:
	const bool match;
	const QString source, target;
	const double rate;
};

}

#endif // STUBCONVERSIONRATES_HPP
