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
#include "budget/AssignmentRules.hpp"
#include "budget/Balance.hpp"
#include "budget/Budget.hpp"
#include "budget/storage/XmlBudgetReader.hpp"

namespace ub {

//------------------------------------------------------------------------------
XmlBudgetReader::XmlBudgetReader()
{ }

//------------------------------------------------------------------------------
bool XmlBudgetReader::read(QIODevice* device)
{
	budget.reset();
	xml.setDevice(device);

	// Go through all top-level elements
	if (xml.readNextStartElement())
	{
		if (xml.name() == "budget")
		{
			QString version = xml.attributes().value("version").toString();
			if (version == "4.0")
				readVersion4();
			else if (version == "5.0")
				readVersion5();
			else
				xml.raiseError(QObject::tr("Unsupported budget version, %1").arg(version));
		}
		else
			xml.raiseError(QObject::tr("The given XML is not a budget definition."));
	}

	return (! xml.error());
}

//------------------------------------------------------------------------------
QSharedPointer<Budget> XmlBudgetReader::lastReadBudget() const
{
	return budget;
}

//------------------------------------------------------------------------------
QString XmlBudgetReader::errorString() const
{
	if (xml.error() == QXmlStreamReader::CustomError)
		return xml.errorString();
	else
	{
		return QObject::tr("%1\nLine %2, column %3")
			.arg(xml.errorString())
			.arg(xml.lineNumber())
			.arg(xml.columnNumber());
	}
}

//------------------------------------------------------------------------------
void XmlBudgetReader::readVersion4()
{
	Q_ASSERT(xml.isStartElement() && xml.name() == "budget"
		&& xml.attributes().value("version") == "4.0");

	// Budget parameters (defaults)
	QString name("Budget");
	QSharedPointer<BudgetingPeriod> period(new BudgetingPeriod);
	QSharedPointer<Balance> initial = Balance::create();
	QSharedPointer<Estimate> root = Estimate::createRoot();
	QSharedPointer<AssignmentRules> rules = AssignmentRules::create();

	// Go through all elements under a budget element
	while (xml.readNextStartElement())
	{
		if (xml.name() == "name")
		{
			name = xml.readElementText();
		}
		else if (xml.name() == "period")
		{
			readVersion4Period(period);
		}
		else if (xml.name() == "initial-balance")
		{
			QString currency = xml.attributes().value("currency").toString();
			QVariant amount = xml.readElementText();
			initial = Balance::create(Money(amount.toDouble(), currency));
		}
		else if (xml.name() == "estimate")
		{
			// Go through all elements belonging to the root estimate
			while (xml.readNextStartElement())
			{
				if (xml.name() == "estimates")
				{
					// Go through all elements belonging to the root estimate's estimates
					while (xml.readNextStartElement())
					{
						if (xml.name() == "estimate")
							readVersion4Estimate(root.data());
						else
							xml.skipCurrentElement();
					}
				}
				else
					xml.skipCurrentElement();
			}
		}
		else if (xml.name() == "rules")
		{
			// Go through all rule elements
			while (xml.readNextStartElement())
			{
				if (xml.name() == "rule")
					readVersion4Rule(rules);
				else
					xml.skipCurrentElement();
			}
		}
		else
			xml.skipCurrentElement();
	}

	if ( ! xml.error())
	{
		budget = QSharedPointer<Budget>(
			new Budget(name, period, initial, root, rules));
	}
}

//------------------------------------------------------------------------------
void XmlBudgetReader::readVersion5()
{
	Q_ASSERT(xml.isStartElement() && xml.name() == "budget"
		&& xml.attributes().value("version") == "5.0");

	// Budget parameters (defaults)
	QString name("Budget");
	QSharedPointer<BudgetingPeriod> period(new BudgetingPeriod);
	QSharedPointer<Balance> initial = Balance::create();
	QSharedPointer<Estimate> root = Estimate::createRoot();
	QSharedPointer<AssignmentRules> rules = AssignmentRules::create();

	// Go through all elements under a budget element
	while (xml.readNextStartElement())
	{
		if (xml.name() == "name")
		{
			name = xml.readElementText();
		}
		else if (xml.name() == "period")
		{
			readVersion5Period(period);
		}
		else if (xml.name() == "initial-balance")
		{
			readVersion5Balance(initial);
		}
		else if (xml.name() == "estimate")
		{
			// Go through all elements belonging to the root estimate
			while (xml.readNextStartElement())
			{
				if (xml.name() == "estimate")
					readVersion5Estimate(root.data());
				else
					xml.skipCurrentElement();
			}
		}
		else if (xml.name() == "rules")
		{
			// Go through all rule elements
			while (xml.readNextStartElement())
			{
				if (xml.name() == "rule")
					readVersion5Rule(rules);
				else
					xml.skipCurrentElement();
			}
		}
		else
			xml.skipCurrentElement();
	}

	if ( ! xml.error())
	{
		budget = QSharedPointer<Budget>(
			new Budget(name, period, initial, root, rules));
	}
}

//------------------------------------------------------------------------------
void XmlBudgetReader::readVersion4Period(QSharedPointer<BudgetingPeriod>& period)
{
	Q_ASSERT(xml.isStartElement() && xml.name() == "period");

	BudgetingPeriod::Parameters parameters;

	QString type = xml.attributes().value("type").toString();
	if (type == "custom")
		parameters.type = BudgetingPeriod::CustomDateRange;
	else if (type == "paydate-month")
		parameters.type = BudgetingPeriod::PaydateMonth;
	else if (type == "literal-month")
		parameters.type = BudgetingPeriod::CalendarMonth;
	else if (type == "literal-year")
		parameters.type = BudgetingPeriod::CalendarYear;

	// Go through all elements under a budgeting period
	while (xml.readNextStartElement())
	{
		if (xml.name() == "year")
		{
			// Year always goes in param 1
			int year = QVariant(xml.readElementText()).toInt();
			parameters.param1 = year;
		}
		else if (xml.name() == "month")
		{
			// Month always goes in param 2
			int month = QVariant(xml.readElementText()).toInt() + 1;
			parameters.param2 = month;
		}
		else if (xml.name() == "start-date")
		{
			parameters.param1 = readDate(true);
		}
		else if (xml.name() == "end-date")
		{
			parameters.param2 = readDate(true);
		}
		else
			xml.skipCurrentElement();
	}

	period = QSharedPointer<BudgetingPeriod>(new BudgetingPeriod(parameters));
}

//------------------------------------------------------------------------------
BudgetingPeriod::Type periodTypeFrom(const QString& type)
{
	if (type == "custom")
		return BudgetingPeriod::CustomDateRange;
	else if (type == "paydate-month")
		return BudgetingPeriod::PaydateMonth;
	else if (type == "calendar-month")
		return BudgetingPeriod::CalendarMonth;
	else if (type == "calendar-year")
		return BudgetingPeriod::CalendarYear;
	else
		return BudgetingPeriod::Undefined;
}

//------------------------------------------------------------------------------
void XmlBudgetReader::readVersion5Period(QSharedPointer<BudgetingPeriod>& period)
{
	Q_ASSERT(xml.isStartElement() && xml.name() == "period");

	BudgetingPeriod::Parameters parameters;
	QString type = xml.attributes().value("type").toString();
	parameters.type = periodTypeFrom(type);

	// Go through all elements under a budgeting period
	while (xml.readNextStartElement())
	{
		if (xml.name() == "param1")
		{
			parameters.param1 = QVariant(xml.readElementText());
		}
		else if (xml.name() == "param2")
		{
			parameters.param2 = QVariant(xml.readElementText());
		}
		else
			xml.skipCurrentElement();
	}

	// Validate all parsed parameters according to the defined period type
	switch (parameters.type)
	{
	case BudgetingPeriod::CustomDateRange:
		// Both parameters must be valid dates
		if ( ! parameters.param1.toDate().isValid()
			|| ! parameters.param2.toDate().isValid())
		{
			xml.raiseError(QObject::tr("Custom budgeting period requires valid "
				"date parameters (line %1)").arg(xml.lineNumber()));
		}
		break;

	case BudgetingPeriod::CalendarMonth:
	case BudgetingPeriod::PaydateMonth:
	{
		// Second parameter must be a valid month
		bool monthOk = false;
		int monthAsInt = parameters.param2.toInt(&monthOk);
		if ( ! monthOk || monthAsInt <= 0 || monthAsInt > 12)
		{
			xml.raiseError(QObject::tr("Second budgeting period parameter must "
				"be a valid month (1-12) (line %1)").arg(xml.lineNumber()));
		}
	}
		// intentionally no break, roll into next case

	case BudgetingPeriod::CalendarYear:
	{
		// First parameter must be a valid year
		bool yearOk = false;
		int yearAsInt = parameters.param1.toInt(&yearOk);
		if ( ! yearOk || yearAsInt < 0)
		{
			xml.raiseError(QObject::tr("First budgeting period parameter must be "
				"a valid year (line %1)").arg(xml.lineNumber()));
		}
	}
		break;

	default:
		xml.raiseError(QObject::tr("Invalid budgeting period type, %1 (line %2)")
			.arg(type).arg(xml.lineNumber()));
	}

	period = QSharedPointer<BudgetingPeriod>(new BudgetingPeriod(parameters));
}

//------------------------------------------------------------------------------
void XmlBudgetReader::readVersion5Balance(QSharedPointer<Balance>& balance)
{
	Q_ASSERT(xml.isStartElement());

	QList<Balance::Contributor> contributors;

	// Go through all elements under a balance
	while (xml.readNextStartElement())
	{
		if (xml.name() == "contributor")
		{
			Balance::Contributor contributor;

			// Go through all elements under a contributor
			while (xml.readNextStartElement())
			{
				if (xml.name() == "name")
					contributor.name = xml.readElementText();
				else if (xml.name() == "amount")
				{
					QString currency = xml.attributes().value("currency").toString();
					QVariant amount = xml.readElementText();
					contributor.amount = Money(amount.toDouble(), currency);
				}
				else if (xml.name() == "increase")
					contributor.increase = QVariant(xml.readElementText()).toBool();
				else
					xml.skipCurrentElement();
			}

			contributors << contributor;
		}
		else
			xml.skipCurrentElement();
	}

	balance = Balance::create(contributors);
}

//------------------------------------------------------------------------------
QDate XmlBudgetReader::readDate(bool januaryIsZero)
{
	int day, month, year;

	// Go through all elements under a date
	while (xml.readNextStartElement())
	{
		if (xml.name() == "day")
			day = QVariant(xml.readElementText()).toInt();
		else if (xml.name() == "month")
		{
			month = QVariant(xml.readElementText()).toInt();

			if (januaryIsZero)
				month++;
		}
		else if (xml.name() == "year")
			year = QVariant(xml.readElementText()).toInt();
		else
			xml.skipCurrentElement();
	}

	return QDate(year, month, day);
}

//------------------------------------------------------------------------------
void XmlBudgetReader::readVersion4Estimate(Estimate* parent)
{
	Q_ASSERT(xml.isStartElement() && xml.name() == "estimate");

	uint id = QVariant(xml.attributes().value("id").toString()).toUInt();
	QString name;
	QString description;
	Estimate::Type type;
	Money amount;
	QDate dueDate;
	bool finished;

	// Go through all elements under an estimate
	while (xml.readNextStartElement())
	{
		if (xml.name() == "name")
			name = xml.readElementText();
		else if (xml.name() == "description")
			description = xml.readElementText();
		else if (xml.name() == "amount")
		{
			QString currency = xml.attributes().value("currency").toString();
			QVariant amt = xml.readElementText();
			amount = Money(amt.toDouble(), currency);
		}
		else if (xml.name() == "type")
		{
			QString typeStr = xml.readElementText();
			if (typeStr == "INCOME")
				type = Estimate::Income;
			else if (typeStr == "TRANSFER")
				type = Estimate::Transfer;
			else
				type = Estimate::Expense;
		}
		else if (xml.name() == "due-date")
			dueDate = readDate(true);
		else if (xml.name() == "complete")
			finished = QVariant(xml.readElementText()).toBool();
		else if (xml.name() == "estimates")
		{
			// According to the v4.0 spec, all estimate attributes have been
			// defined by now, so we can create the estimate
			Estimate* child = Estimate::create(parent, id, name, description,
				type, amount, dueDate, finished);

			// Go through all estimate elements
			while (xml.readNextStartElement())
			{
				if (xml.name() == "estimate")
					readVersion4Estimate(child);
				else
					xml.skipCurrentElement();
			}
		}
		else
			xml.skipCurrentElement();
	}
}

//------------------------------------------------------------------------------
void XmlBudgetReader::readVersion5Estimate(Estimate* parent)
{
	Q_ASSERT(xml.isStartElement() && xml.name() == "estimate");

	uint id = QVariant(xml.attributes().value("id").toString()).toUInt();
	QString name;
	QString description;
	Estimate::Type type = parent->estimateType();
	Money amount;
	QDate dueDate;
	bool finished;
	Estimate* estimate = 0;

	// Go through all elements under an estimate
	while (xml.readNextStartElement())
	{
		if (xml.name() == "name")
			name = xml.readElementText();
		else if (xml.name() == "description")
			description = xml.readElementText();
		else if (xml.name() == "amount")
		{
			QString currency = xml.attributes().value("currency").toString();
			QVariant amt = xml.readElementText();
			amount = Money(amt.toDouble(), currency);
		}
		else if (xml.name() == "type")
		{
			QString typeStr = xml.readElementText();
			if (typeStr == "income")
				type = Estimate::Income;
			else if (typeStr == "transfer")
				type = Estimate::Transfer;
			else
				type = Estimate::Expense;
		}
		else if (xml.name() == "due-date")
			dueDate = QVariant(xml.readElementText()).toDate();
		else if (xml.name() == "finished")
		{
			finished = true;
			xml.skipCurrentElement();
		}
		else if (xml.name() == "estimate")
		{
			if ( ! estimate)
			{
				// According to the v5.0 spec, all estimate attributes will
				// have been defined by now, so we can create the estimate if
				// it hasn't been created yet already
				estimate = Estimate::create(parent, id, name, description,
					type, amount, dueDate, finished);
			}

			readVersion5Estimate(estimate);
		}
		else
			xml.skipCurrentElement();
	}

	if ( ! estimate)
	{
		// If this estimate hasn't been created yet (because we didn't encounter
		// any child estimates--meaning this estimate is a leaf) create it
		estimate = Estimate::create(parent, id, name, description,
			type, amount, dueDate, finished);
	}
}

//------------------------------------------------------------------------------
void XmlBudgetReader::readVersion4Rule(QSharedPointer<AssignmentRules> rules)
{
	Q_ASSERT(xml.isStartElement() && xml.name() == "rule");

	uint ruleId = QDateTime().toTime_t(); // v4.0 doesn't specify rule IDs
	uint estimateId;
	QList<AssignmentRule::Condition> conditions;

	// Go through all elements under a rule
	while (xml.readNextStartElement())
	{
		if (xml.name() == "estimate")
		{
			estimateId = QVariant(xml.attributes().value("reference").toString()).toUInt();
			xml.skipCurrentElement();
		}
		else if (xml.name() == "conditions")
		{
			// Go through all condition elements
			while (xml.readNextStartElement())
			{
				if (xml.name() == "condition")
				{
					AssignmentRule::Condition condition;

					// Go through all elements for a condition
					while (xml.readNextStartElement())
					{
						if (xml.name() == "field")
						{
							QString field = xml.readElementText();
							if (field == "PAYEE")
								condition.field = AssignmentRule::Payee;
							else if (field == "MEMO")
								condition.field = AssignmentRule::Memo;
							else if (field == "DEPOSIT")
								condition.field = AssignmentRule::DepositAccount;
							else if (field == "WITHDRAWAL")
								condition.field = AssignmentRule::WithdrawalAccount;
							// else leave unknown
						}
						else if (xml.name() == "operator")
						{
							QString oper = xml.readElementText();
							if (oper.contains("BEGINS_WITH"))
								condition.op = AssignmentRule::BeginsWith;
							else if (oper.contains("ENDS_WITH"))
								condition.op = AssignmentRule::EndsWith;
							else if (oper.contains("CONTAINS"))
								condition.op = AssignmentRule::Contains;
							else if (oper.contains("EQUALS"))
								condition.op = AssignmentRule::StringEquals;
							// else leave unknown

							condition.sensitive = oper.contains("CASE");
						}
						else if (xml.name() == "value")
							condition.value = xml.readElementText();
						else
							xml.skipCurrentElement();
					}

					// Done defining condition
					conditions << condition;
				}
				else
					xml.skipCurrentElement();
			}
		}
		else
			xml.skipCurrentElement();
	}

	rules->createRule(ruleId, estimateId, conditions);
}

//------------------------------------------------------------------------------
void XmlBudgetReader::readVersion5Rule(QSharedPointer<AssignmentRules> rules)
{
	Q_ASSERT(xml.isStartElement() && xml.name() == "rule");

	uint ruleId = QVariant(xml.attributes().value("id").toString()).toUInt();
	uint estimateId;
	QList<AssignmentRule::Condition> conditions;

	// Go through all elements under a rule
	while (xml.readNextStartElement())
	{
		if (xml.name() == "estimate")
			estimateId = QVariant(xml.readElementText()).toUInt();
		else if (xml.name() == "condition")
			readVersion5Condition(conditions);
		else
			xml.skipCurrentElement();
	}

	rules->createRule(ruleId, estimateId, conditions);
}

//------------------------------------------------------------------------------
AssignmentRule::Field conditionFieldFrom(const QString& field)
{
	if (field == "date")
		return AssignmentRule::Date;
	else if (field == "amount")
		return AssignmentRule::Amount;
	else if (field == "payee")
		return AssignmentRule::Payee;
	else if (field == "memo")
		return AssignmentRule::Memo;
	else if (field == "deposit")
		return AssignmentRule::DepositAccount;
	else if (field == "withdrawal")
		return AssignmentRule::WithdrawalAccount;
	else
		return AssignmentRule::FieldNotDefined;
}

//------------------------------------------------------------------------------
AssignmentRule::Operator conditionOperatorFrom(const QString& oper)
{
	if (oper == "begins-with")
		return AssignmentRule::BeginsWith;
	else if (oper == "ends-with")
		return AssignmentRule::EndsWith;
	else if (oper == "contains")
		return AssignmentRule::Contains;
	else if (oper == "string-equals")
		return AssignmentRule::StringEquals;
	else if (oper == "before")
		return AssignmentRule::Before;
	else if (oper == "after")
		return AssignmentRule::After;
	else if (oper == "date-equals")
		return AssignmentRule::DateEquals;
	else if (oper == "less-than")
		return AssignmentRule::LessThan;
	else if (oper == "less-than-or-equal")
		return AssignmentRule::LessThanOrEqual;
	else if (oper == "greater-than")
		return AssignmentRule::GreaterThan;
	else if (oper == "greater-than-or-equal")
		return AssignmentRule::GreaterThanOrEqual;
	else if (oper == "amount-equals")
		return AssignmentRule::AmountEquals;
	else
		return AssignmentRule::OperatorNotDefined;
}

//------------------------------------------------------------------------------
void XmlBudgetReader::readVersion5Condition(QList<AssignmentRule::Condition>& conditions)
{
	Q_ASSERT(xml.isStartElement() && xml.name() == "condition");

	AssignmentRule::Condition condition;
	QVariant value;

	// Go through all elements for a condition
	while (xml.readNextStartElement())
	{
		if (xml.name() == "field")
			condition.field = conditionFieldFrom(xml.readElementText());
		else if (xml.name() == "operator")
			condition.op = conditionOperatorFrom(xml.readElementText());
		else if (xml.name() == "case-sensitive")
			condition.sensitive = QVariant(xml.readElementText()).toBool();
		else if (xml.name() == "value")
		{
			condition.value = xml.readElementText();
			value = QVariant(condition.value);
		}
		else
			xml.skipCurrentElement();
	}

	// Verify entries based on the field specified
	if (condition.field == AssignmentRule::Date)
	{
		if ( ! value.toDate().isValid())
		{
			xml.raiseError(QObject::tr("Condition value must specify a valid "
				"date (line %1)").arg(xml.lineNumber()));
		}
		if (condition.op != AssignmentRule::Before
			&& condition.op != AssignmentRule::After
			&& condition.op != AssignmentRule::DateEquals)
		{
			xml.raiseError(QObject::tr("Condition operator must specify a date "
				"comparison operator (line %1)").arg(xml.lineNumber()));
		}
	}
	else if (condition.field == AssignmentRule::Amount)
	{
		bool ok = false;
		value.toDouble(&ok);
		if ( ! ok)
		{
			xml.raiseError(QObject::tr("Condition value must specify a valid "
				"number (line %1)").arg(xml.lineNumber()));
		}
		if (condition.op != AssignmentRule::LessThan
			&& condition.op != AssignmentRule::LessThanOrEqual
			&& condition.op != AssignmentRule::GreaterThan
			&& condition.op != AssignmentRule::GreaterThanOrEqual
			&& condition.op != AssignmentRule::AmountEquals)
		{
			xml.raiseError(QObject::tr("Condition operator must specify a money "
				"comparison operator (line %1)").arg(xml.lineNumber()));
		}
	}
	// For all string fields
	else
	{
		if (condition.op != AssignmentRule::BeginsWith
			&& condition.op != AssignmentRule::EndsWith
			&& condition.op != AssignmentRule::Contains
			&& condition.op != AssignmentRule::StringEquals)
		{
			xml.raiseError(QObject::tr("Condition operator must specify a string "
				"comparison operator (line %1)").arg(xml.lineNumber()));
		}
	}

	conditions << condition;
}

}

