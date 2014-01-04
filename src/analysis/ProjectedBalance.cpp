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
#include "analysis/ProjectedBalance.hpp"

namespace ub {

//------------------------------------------------------------------------------
ProjectedBalance::ProjectedBalance(QSharedPointer<Balance> initial,
		QObject* parent)
	: QObject(parent), initialBalance(initial)
{
	connect(initial.data(), SIGNAL(valueChanged()),
		this, SIGNAL(balanceChanged()));
}

//------------------------------------------------------------------------------
void ProjectedBalance::clear()
{
	increase = Money();
	decrease = Money();
	emit balanceChanged();
}

//------------------------------------------------------------------------------
void ProjectedBalance::add(const Money& amount)
{
	if (amount.isNegative())
	{
		decrease = decrease.isZero() ? amount
			: decrease + amount;
	}
	else
	{
		increase = increase.isZero() ? amount
			: increase + amount;
	}

	emit balanceChanged();
}

//------------------------------------------------------------------------------
Money ProjectedBalance::netIncrease() const
{
	return increase;
}

//------------------------------------------------------------------------------
Money ProjectedBalance::netDecrease() const
{
	return decrease;
}

//------------------------------------------------------------------------------
Money ProjectedBalance::netChange() const
{
	return (increase + decrease);
}

//------------------------------------------------------------------------------
Money ProjectedBalance::initial() const
{
	return initialBalance->value();
}

//------------------------------------------------------------------------------
Money ProjectedBalance::value() const
{
	return (initial() + netChange());
}

}

