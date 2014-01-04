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

#ifndef PROJECTEDBALANCE_HPP
#define PROJECTEDBALANCE_HPP

// Qt include(s)
#include <QObject>
#include <QSharedPointer>

// UnderBudget include(s)
#include "accounting/Money.hpp"
#include "budget/Balance.hpp"

namespace ub {

/**
 * A projection of an ending balance, consisting of an initial balance
 * and a set of adjustments.
 */
class ProjectedBalance : public QObject
{
	Q_OBJECT

public:
	/**
	 * Constructs a new projected balance.
	 *
	 * @param[in] initial initial balance
	 * @param[in] parent  parent object
	 */
	ProjectedBalance(QSharedPointer<Balance> initial = Balance::create(),
		QObject* parent = 0);

	/**
	 * Removes all adjustments to the projected balance.
	 */
	void clear();

	/**
	 * Adds an adjustment to the projected balance.
	 *
	 * @param[in] amount adjustment amount
	 */
	void add(const Money& amount);

	/**
	 * Returns the net increase to the initial balance.
	 *
	 * @return net increase
	 */
	Money netIncrease() const;

	/**
	 * Returns the net decrease to the initial balance.
	 *
	 * @return net decrease
	 */
	Money netDecrease() const;

	/**
	 * Returns the net change, or adjustment, to the initial balance.
	 *
	 * @return net change
	 */
	Money netChange() const;

	/**
	 * Returns the initial balance value.
	 *
	 * @return initial balance value.
	 */
	Money initial() const;

	/**
	 * Returns the projected balance value.
	 *
	 * @return projected balance value.
	 */
	Money value() const;

signals:
	/**
	 * Emitted whenever the projected balance changes. This will also
	 * get emitted if the initial balance changes.
	 */
	void balanceChanged();

private:
	/** Initial balance */
	QSharedPointer<Balance> initialBalance;
	/** Increase adjustment */
	Money increase;
	/** Decrease adjustment */
	Money decrease;
};

}

#endif //PROJECTEDBALANCE_HPP
