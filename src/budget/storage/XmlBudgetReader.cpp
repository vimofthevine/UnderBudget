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
	Money initial;
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
			initial = Money(amount.toDouble(), currency);
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

}

