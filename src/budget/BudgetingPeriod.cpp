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
#include "budget/BudgetingPeriod.hpp"
#include "budget/ChangePeriodParamsCommand.hpp"

namespace ub {

//------------------------------------------------------------------------------
BudgetingPeriod::BudgetingPeriod(const Parameters& params, QObject* parent)
	: QObject(parent)
{
	setParams(params);
}

//------------------------------------------------------------------------------
bool BudgetingPeriod::contains(const QDate& date) const
{
	return (periodStartDate <= date) || (date <= periodEndDate);
}

//------------------------------------------------------------------------------
QUndoCommand* BudgetingPeriod::update(const Parameters& params,
	QUndoCommand* parent)
{
	return new ChangePeriodParamsCommand(this, periodParameters, params, parent);
}

//------------------------------------------------------------------------------
BudgetingPeriod::Parameters BudgetingPeriod::parameters() const
{
	return periodParameters;
}

//------------------------------------------------------------------------------
QDate BudgetingPeriod::startDate() const
{
	return periodStartDate;
}

//------------------------------------------------------------------------------
QDate BudgetingPeriod::endDate() const
{
	return periodEndDate;
}

//------------------------------------------------------------------------------
void BudgetingPeriod::setParams(const Parameters& newParams)
{
	periodParameters = newParams;

	switch (periodParameters.type)
	{
	case LiteralMonth:
		calculateLiteralMonthDates();
		break;
	case LiteralYear:
		calculateLiteralYearDates();
		break;
	case PaydateMonth:
		calculatePaydateMonthDates();
		break;
	case CustomDateRange:
		calculateCustomDates();
		break;
	default:
		periodStartDate = QDate();
		periodEndDate = QDate();
	}

	emit paramsChanged(periodParameters);
}

}

