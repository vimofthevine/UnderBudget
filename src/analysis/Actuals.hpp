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

#ifndef ACTUALS_HPP
#define ACTUALS_HPP

// Qt include(s)
#include <QHash>
#include <QObject>

// UnderBudget include(s)
#include "accounting/Money.hpp"

namespace ub {

/**
 * Records actual transactions made against estimates.
 */
class Actuals : public QObject
{
	Q_OBJECT

public:
	/**
	 * Constructs a new actuals collection.
	 *
	 * @param[in] parent parent object
	 */
	Actuals(QObject* parent = 0);

	/**
	 * Clears all previous recordings.
	 */
	void clear();

	/**
	 * Records the application of the given amount towards
	 * the specified estimate.
	 *
	 * @param[in] estId  unique ID of the estimate to which the actual applies
	 * @param[in] amount actual amount
	 */
	void record(uint estId, const Money& amount);

	/**
	 * Returns the actual amount for the given estimate ID.
	 *
	 * @param[in] estId unique ID of an estimate
	 * @return actual amount for the estimate
	 */
	Money forEstimate(uint estId) const;

	/**
	 * Returns a copy of the actuals as a simple map.
	 *
	 * @return actuals map copy
	 */
	QHash<uint, Money> map() const;

signals:
	/**
	 * Emitted whenever the actuals have changed. The change
	 * may be that the actuals have been cleared, or an individual
	 * actual has been updated.
	 */
	void actualsChanged();

private:
	/** Map of estimate ID to actual amount */
	QHash<uint, Money> actuals;
};

}

#endif //ACTUALS_HPP
