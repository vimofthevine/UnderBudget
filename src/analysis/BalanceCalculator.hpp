/*
 * Copyright 2014 Kyle Treubig
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

#ifndef BALANCECALCULATOR_HPP
#define BALANCECALCULATOR_HPP

// Qt include(s)
#include <QObject>
#include <QSharedPointer>

namespace ub {

// Forward declaration(s)
class Actuals;
class Estimate;
class ProjectedBalance;
class SortedDifferences;

/**
 * Calculates projected balances from estimated and actual values.
 *
 * @ingroup analysis
 */
class BalanceCalculator : public QObject
{
	Q_OBJECT

public:
	/**
	 * Constructs a new calculator.
	 *
	 * @param[in] estimates  root estimate
	 * @param[in] actuals    actual values
	 * @param[in] estimated  estimated projected balance
	 * @param[in] actual     actual projected balance
	 * @param[in] expected   extrapolated projected balance
	 * @param[in] overDiffs  over-budget estimate differences
	 * @param[in] underDiffs under-budget estimate differences
	 * @param[in] parent     parent object
	 */
	BalanceCalculator(QSharedPointer<Estimate> estimates,
		Actuals* actuals, ProjectedBalance* estimated,
		ProjectedBalance* actual, ProjectedBalance* expected,
		SortedDifferences* overDiffs, SortedDifferences* underDiffs,
		QObject* parent = 0);

public slots:
	/**
	 * Calculates the projected estimated, actual, and expected
	 * balances.
	 */
	void calculateBalances();

signals:
	/**
	 * Emitted when a calculation operation commences.
	 */
	void started();

	/**
	 * If emitted, indicates the current progress of the
	 * calculation as a percentage (out of 100).
	 *
	 * @param percent calculation percent complete
	 */
	void progress(int percent);

	/**
	 * Emitted when the calculation operation is completed.
	 */
	void finished();

private:
	/** Estimate tree */
	QSharedPointer<Estimate> estimates;
	/** Actuals */
	Actuals* actuals;
	/** Projected estimated balance */
	ProjectedBalance* estimated;
	/** Projected actual balance */
	ProjectedBalance* actual;
	/** Projected expected balance */
	ProjectedBalance* expected;
	/** Over-budget differences */
	SortedDifferences* overBudget;
	/** Under-budget differences */
	SortedDifferences* underBudget;
	/** Whether the calculator is currencly calculating */
	bool isCalculating;
	/** Actuals cache */
	QHash<uint,Money> actualsCache;

	/**
	 * Clears all results from a previous calculation.
	 */
	void clear();

	/**
	 * Adds the given estimate's impact values to the
	 * projected balances. This function is recursive, descending
	 * to all child estimates of the estimate.
	 *
	 * @param[in] estimate estimate whose impact is to be added
	 */
	void add(const Estimate* estimate);
};

}

#endif //BALANCECALCULATOR_HPP
