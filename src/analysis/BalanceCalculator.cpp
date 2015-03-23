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

// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "analysis/Actuals.hpp"
#include "analysis/BalanceCalculator.hpp"
#include "analysis/ProjectedBalance.hpp"
#include "analysis/SortedDifferences.hpp"
#include "budget/Estimate.hpp"

namespace ub {

//------------------------------------------------------------------------------
BalanceCalculator::BalanceCalculator(QSharedPointer<Estimate> estimates,
		Actuals* actuals, ProjectedBalance* estimated,
		ProjectedBalance* actual, ProjectedBalance* expected,
		SortedDifferences* overDiffs, SortedDifferences* underDiffs,
		QObject* parent)
	: QObject(parent),
	  estimates(estimates),
	  actuals(actuals),
	  estimated(estimated),
	  actual(actual),
	  expected(expected),
	  overBudget(overDiffs),
	  underBudget(underDiffs),
	  isCalculating(false)
{}

//------------------------------------------------------------------------------
void BalanceCalculator::calculateBalances()
{
	if ( ! isCalculating)
	{
		isCalculating = true;
		emit started();

		clear();

		// Grab cache/map of actuals
		actualsCache = actuals->map();
		add(estimates.data());

		// Sort differences
		overBudget->sort();
		underBudget->sort();

		isCalculating = false;
		emit finished();
	}
}

//------------------------------------------------------------------------------
void BalanceCalculator::clear()
{
	estimated->clear();
	actual->clear();
	expected->clear();
	overBudget->clear();
	underBudget->clear();
}

//------------------------------------------------------------------------------
void BalanceCalculator::add(const Estimate* estimate)
{
	Estimate::Progress progress = estimate->progress(actualsCache);
	Estimate::Impact impact = estimate->impact(actualsCache);

	// Add to projected balances
	estimated->add(impact.estimated);
	actual->add(impact.actual);
	expected->add(impact.expected);

	// Get estimated vs actual
	if (progress.isHealthy)
	{
		// Since we want an absolute value representing how much
		// we're under (or over for an income), we negate the
		// difference between the estimated and actual. For example:
		// For an underspent expense (est=-2, act=-1), -(-2 - (-1)) = 1
		// For an underestimated income (est=2, act=3), -(2 - 3) = 1
		Money diff = -(impact.estimated - impact.actual);

		// Only record diff if it's not 0
		if ( ! diff.isZero())
		{
			underBudget->record(estimate->estimateId(), diff);
		}
	}
	else
	{
		// Since we want an absolute value representing how much
		// we're over (or under for an income), we negate the
		// difference between the actual and estimated. For example:
		// For an overspent expense, (est=-1, act=-2), -(-2 - (-1)) = 1
		// For an overestimated income (est=3, act=2), -(2 - 3) = 1
		Money diff = -(impact.actual - impact.estimated);

		// Only record diff if it's not 0
		if ( ! diff.isZero())
		{
			overBudget->record(estimate->estimateId(), diff);
		}
	}

	for (int i=0; i<estimate->childCount(); ++i)
	{
		add(estimate->childAt(i));
	}
}

}

