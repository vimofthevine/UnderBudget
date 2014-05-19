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
#include "accounting/Money.hpp"
#include "budget/AssignmentRule.hpp"
#include "budget/AssignmentRules.hpp"
#include "budget/Balance.hpp"
#include "budget/Budget.hpp"
#include "budget/BudgetingPeriod.hpp"
#include "budget/storage/XmlBudgetWriter.hpp"

namespace ub {

//------------------------------------------------------------------------------
static const QString ub_ns = "http://underbudget.vimofthevine.com/ub";
static const QString balance_ns = "http://underbudget.vimofthevine.com/balance";
static const QString contributor_ns = "http://underbudget.vimofthevine.com/contributor";
static const QString period_ns = "http://underbudget.vimofthevine.com/budgeting-period";
static const QString estimate_ns = "http://underbudget.vimofthevine.com/estimate";
static const QString rule_ns = "http://underbudget.vimofthevine.com/rule";
static const QString condition_ns = "http://underbudget.vimofthevine.com/condition";

//------------------------------------------------------------------------------
bool XmlBudgetWriter::write(QIODevice* device, const QSharedPointer<Budget> budget)
{
	XmlBudgetWriter writer(device);
	return writer.write(budget);
}

//------------------------------------------------------------------------------
XmlBudgetWriter::XmlBudgetWriter(QIODevice* device)
	: xml(device)
{ }

//------------------------------------------------------------------------------
bool XmlBudgetWriter::write(const QSharedPointer<Budget> budget)
{
	xml.setAutoFormatting(true);

	xml.writeNamespace(ub_ns, "ub");
	xml.writeNamespace(balance_ns, "balance");
	xml.writeNamespace(contributor_ns, "contributor");
	xml.writeNamespace(period_ns, "period");
	xml.writeNamespace(estimate_ns, "estimate");
	xml.writeNamespace(rule_ns, "rule");
	xml.writeNamespace(condition_ns, "condition");

	xml.writeStartDocument();

	xml.writeStartElement(ub_ns, "budget");
	xml.writeAttribute("version", "5.0");

	// Budget info
	xml.writeTextElement(ub_ns, "name", budget->name());
	write(ub_ns, "initial-balance", budget->initialBalance());
	write(budget->budgetingPeriod());
	// Estimates
	write(budget->estimates().data());
	// Rules
	write(budget->rules());

	xml.writeEndElement(); // budget

	xml.writeEndDocument();
	return ( ! xml.hasError());
}

//------------------------------------------------------------------------------
void XmlBudgetWriter::write(const QString& ns, const QString& tag,
	const Money& value)
{
	xml.writeStartElement(ns, tag);
	xml.writeAttribute("currency", value.currency().code());
	xml.writeCharacters(QVariant(value.amount()).toString());
	xml.writeEndElement();
}

//------------------------------------------------------------------------------
void XmlBudgetWriter::write(const QString&ns, const QString& tag,
	const QSharedPointer<Balance> balance)
{
	xml.writeStartElement(ns, tag);

	for (int i=0; i<balance->contributorCount(); ++i)
	{
		Balance::Contributor contributor = balance->contributorAt(i);

		xml.writeStartElement(balance_ns, "contributor");
		xml.writeTextElement(contributor_ns, "name", contributor.name);
		write(contributor_ns, "amount", contributor.amount);
		xml.writeTextElement(contributor_ns, "increase",
			QVariant(contributor.increase).toString());
		xml.writeEndElement();
	}

	xml.writeEndElement();
}

//------------------------------------------------------------------------------
QString serialize(BudgetingPeriod::Type type)
{
	switch (type)
	{
	case BudgetingPeriod::CalendarYear:
		return "calendar-year";
	case BudgetingPeriod::CalendarMonth:
		return "calendar-month";
	case BudgetingPeriod::PaydateMonth:
		return "paydate-month";
	case BudgetingPeriod::CustomDateRange:
		return "custom";
	default:
		return "unknown";
	}
}

//------------------------------------------------------------------------------
void XmlBudgetWriter::write(const QSharedPointer<BudgetingPeriod> period)
{
	BudgetingPeriod::Parameters params = period->parameters();
	xml.writeStartElement(ub_ns, "period");
	xml.writeAttribute("type", serialize(params.type));
	xml.writeTextElement(period_ns, "param1", params.param1.toString());
	xml.writeTextElement(period_ns, "param2", params.param2.toString());
	xml.writeEndElement();
}

//------------------------------------------------------------------------------
void XmlBudgetWriter::write(const Estimate* estimate)
{
	xml.writeStartElement(estimate_ns, "estimate");
	xml.writeAttribute("id", QVariant(estimate->estimateId()).toString());

	// If not root, write estimate parameters
	if (estimate->parentEstimate() != 0)
	{
		// Always write name
		xml.writeTextElement(estimate_ns, "name", estimate->estimateName());

		// Only if description is defined, write it
		if ( ! estimate->estimateDescription().isEmpty())
		{
			xml.writeTextElement(estimate_ns, "description", estimate->estimateDescription());
		}

		// Only if top-level estimate (parent is root), write type
		if (estimate->parentEstimate()->parentEstimate() == 0)
		{
			xml.writeTextElement(estimate_ns, "type",
				toString(estimate->estimateType()).toLower());
		}

		// If not a parent of estimates, write leaf-only parameters
		if (estimate->childCount() == 0)
		{
			write(estimate_ns, "amount", estimate->estimatedAmount());

			// Only if due date is defined, write it
			if (estimate->activityDueDate().isValid())
			{
				xml.writeTextElement(estimate_ns, "due-date",
					estimate->activityDueDate().toString(Qt::ISODate));
			}

			// Only if finished, write it (omission means it's not finished)
			if (estimate->isActivityFinished())
			{
				xml.writeEmptyElement(estimate_ns, "finished");
			}
		}
	}

	for (int i=0; i<estimate->childCount(); ++i)
	{
		write(estimate->childAt(i));
	}

	xml.writeEndElement();
}

//------------------------------------------------------------------------------
void XmlBudgetWriter::write(const QSharedPointer<AssignmentRules> rules)
{
	xml.writeStartElement(ub_ns, "rules");

	for (int i=0; i<rules->size(); ++i)
	{
		write(rules->at(i));
	}

	xml.writeEndElement();
}

//------------------------------------------------------------------------------
void XmlBudgetWriter::write(const AssignmentRule* rule)
{
	xml.writeStartElement(rule_ns, "rule");
	xml.writeAttribute("id", QVariant(rule->ruleId()).toString());
	xml.writeTextElement(rule_ns, "estimate",
		QVariant(rule->estimateId()).toString());

	for (int i=0; i<rule->conditionCount(); ++i)
	{
		write(rule->conditionAt(i));
	}

	xml.writeEndElement();
}

//------------------------------------------------------------------------------
QString serialize(AssignmentRule::Field field)
{
	switch (field)
	{
	case AssignmentRule::Date:
		return "date";
	case AssignmentRule::Amount:
		return "amount";
	case AssignmentRule::Payee:
		return "payee";
	case AssignmentRule::Memo:
		return "memo";
	case AssignmentRule::DepositAccount:
		return "deposit";
	case AssignmentRule::WithdrawalAccount:
		return "withdrawal";
	default:
		return "unknown";
	}
}

//------------------------------------------------------------------------------
QString serialize(AssignmentRule::Operator oper)
{
	switch (oper)
	{
	case AssignmentRule::BeginsWith:
		return "begins-with";
	case AssignmentRule::EndsWith:
		return "ends-with";
	case AssignmentRule::StringEquals:
		return "string-equals";
	case AssignmentRule::Contains:
		return "contains";
	case AssignmentRule::Before:
		return "before";
	case AssignmentRule::After:
		return "after";
	case AssignmentRule::DateEquals:
		return "date-equals";
	case AssignmentRule::LessThan:
		return "less-than";
	case AssignmentRule::LessThanOrEqual:
		return "less-than-or-equal";
	case AssignmentRule::GreaterThan:
		return "greater-than";
	case AssignmentRule::GreaterThanOrEqual:
		return "greater-than-or-equal";
	case AssignmentRule::AmountEquals:
		return "amount-equals";
	default:
		return "unknown";
	}
}

//------------------------------------------------------------------------------
void XmlBudgetWriter::write(const AssignmentRule::Condition& condition)
{
	xml.writeStartElement(rule_ns, "condition");
	xml.writeTextElement(condition_ns, "field", serialize(condition.field));
	xml.writeTextElement(condition_ns, "operator", serialize(condition.op));
	xml.writeTextElement(condition_ns, "case-sensitive",
		QVariant(condition.sensitive).toString());
	xml.writeTextElement(condition_ns, "value", condition.value);
	xml.writeEndElement();
}

}

