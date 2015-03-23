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
#include <QtCore>

// UnderBudget include(s)
#include "accounting/Money.hpp"
#include "budget/conditions.hpp"

namespace ub {

//------------------------------------------------------------------------------
bool qualifies(const QDate& date, const AssignmentRule::Condition& condition)
{
	QDate cmpDate = QVariant(condition.value).toDate();
	if ( ! cmpDate.isValid() || ! date.isValid())
		return false;

	switch (condition.op)
	{
	case AssignmentRule::Before:
		return (date < cmpDate);
	case AssignmentRule::After:
		return (date > cmpDate);
	case AssignmentRule::DateEquals:
		return (date == cmpDate);
	default:
		return false;
	}
}

//------------------------------------------------------------------------------
bool qualifies(const Money& amount, const AssignmentRule::Condition& condition)
{
	if ( ! condition.value.contains(","))
		return false;
	QStringList amtParts = condition.value.split(",");
	if (amtParts.size() != 2)
		return false;
	Money cmpAmt = Money(QVariant(amtParts[0]).toDouble(), amtParts[1]);

	switch (condition.op)
	{
	case AssignmentRule::LessThan:
		return (amount < cmpAmt);
	case AssignmentRule::LessThanOrEqual:
		return (amount <= cmpAmt);
	case AssignmentRule::GreaterThan:
		return (amount > cmpAmt);
	case AssignmentRule::GreaterThanOrEqual:
		return (amount >= cmpAmt);
	case AssignmentRule::AmountEquals:
		return (amount == cmpAmt);
	default:
		return false;
	}
}

//------------------------------------------------------------------------------
bool qualifies(const QString& string, const AssignmentRule::Condition& condition)
{
	Qt::CaseSensitivity sensitive = condition.sensitive
		? Qt::CaseSensitive : Qt::CaseInsensitive;
	QString cmpStr = condition.value;

	switch (condition.op)
	{
	case AssignmentRule::BeginsWith:
		return string.startsWith(cmpStr, sensitive);
	case AssignmentRule::EndsWith:
		return string.endsWith(cmpStr, sensitive);
	case AssignmentRule::Contains:
		return string.contains(cmpStr, sensitive);
	case AssignmentRule::StringEquals:
		return (string.compare(cmpStr, sensitive) == 0);
	default:
		return false;
	}
}

}

