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

#ifndef XMLBUDGETREADER_HPP
#define XMLBUDGETREADER_HPP

// Qt include(s)
#include <QSharedPointer>

// UnderBudget include(s)
#include "budget/AssignmentRule.hpp"

// Forward declaration(s)
class QIODevice;

namespace ub {

// Forward declaration(s)
class Budget;
class BudgetingPeriod;
class Estimate;

/**
 * XML stream reader for unserializing a budget from an XML document.
 */
class XmlBudgetReader
{
public:
	/**
	 * Constructs a new XML budget reader.
	 */
	XmlBudgetReader();

	/**
	 * Parses the XML from the given IO device. If successful, this will return
	 * `true` and `lastReadBudget()` will return the budget read from the XML.
	 * If an error occured, this will return `false` and `lastReadBudget()`
	 * will return a null pointer.
	 *
	 * @param[in] device IO device from which to parse as XML
	 * @return `true` if successful in parsing the budget from the XML stream
	 */
	bool read(QIODevice* device);

	/**
	 * Returns the budget last read from a given input. If the last parse
	 * operation encountered an error, this will return a null pointer.
	 *
	 * @return budget last read from XML
	 */
	QSharedPointer<Budget> lastReadBudget() const;

	/**
	 * Returns a description of the last error to occur while reading
	 * the budget from XML.
	 *
	 * @return last error
	 */
	QString errorString() const;

private:
	/** XML stream reader */
	QXmlStreamReader xml;
	/** Last read budget */
	QSharedPointer<Budget> budget;

	/**
	 * Reads the XML as a version 4.0 budget definition.
	 */
	void readVersion4();

	/**
	 * Reads the XML as a version 5.0 budget definition.
	 */
	void readVersion5();

	/**
	 * Reads a budgeting period defined by the version 4.0 schema.
	 *
	 * @param[in,out] budgeting period
	 */
	void readVersion4Period(QSharedPointer<BudgetingPeriod>& period);

	/**
	 * Reads a budgeting period defined by the version 5.0 schema.
	 *
	 * @param[in,out] budgeting period
	 */
	void readVersion5Period(QSharedPointer<BudgetingPeriod>& period);

	/**
	 * Reads a date defined by three elements for the day, month,
	 * and year.
	 *
	 * @param[in] januaryIsZero indicates whether the months
	 *                          begin with 0 or 1
	 * @return defined date
	 */
	QDate readDate(bool januaryIsZero);

	/**
	 * Reads an estimate as defined by the version 4.0 schema,
	 * adding it to the given parent estimate. This method will
	 * execute recursively to parse all children of the current
	 * estimate in the XML data.
	 *
	 * @param[in] parent parent estimate
	 */
	void readVersion4Estimate(Estimate* parent);

	/**
	 * Reads an estimate as defined by the version 5.0 schema,
	 * adding it to the given parent estimate. This method will
	 * execute recursively to parse all children of the current
	 * estimate in the XML data.
	 *
	 * @param[in] parent parent estimate
	 */
	void readVersion5Estimate(Estimate* parent);

	/**
	 * Reads an assignment rule as defined by the version 4.0 schema,
	 * adding it to the given assignment rules list.
	 *
	 * @param[in] rules assignment rules list
	 */
	void readVersion4Rule(QSharedPointer<AssignmentRules> rules);

	/**
	 * Reads an assignment rule as defined by the version 5.0 schema,
	 * adding it to the given assignment rules list.
	 *
	 * @param[in] rules assignment rules list
	 */
	void readVersion5Rule(QSharedPointer<AssignmentRules> rules);

	/**
	 * Reads a rule condition as defined by the version 5.0 schema,
	 * adding it to the given rule conditions list.
	 *
	 * @param[in] conditions rule conditions list
	 */
	void readVersion5Condition(QList<AssignmentRule::Condition>& conditions);
};

}

#endif //XMLBUDGETREADER_HPP
