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
#include "analysis/Actuals.hpp"

namespace ub {

//------------------------------------------------------------------------------
Actuals::Actuals(QObject* parent)
	: QObject(parent)
{ }

//------------------------------------------------------------------------------
void Actuals::clear()
{
	actuals.clear();
	emit actualsChanged();
}

//------------------------------------------------------------------------------
void Actuals::record(uint estId, const Money& amount)
{
	// We want to make sure to initialize the actual amount
	// with a given amount, so we start with the actual currency,
	// rather than the default currency.
	if (actuals.contains(estId))
	{
		actuals[estId]+= amount;
	}
	else
	{
		actuals[estId] = amount;
	}

	emit actualsChanged();
}

//------------------------------------------------------------------------------
Money Actuals::forEstimate(uint estId) const
{
	return actuals[estId];
}

//------------------------------------------------------------------------------
QHash<uint,Money> Actuals::map() const
{
	return actuals;
}

}

