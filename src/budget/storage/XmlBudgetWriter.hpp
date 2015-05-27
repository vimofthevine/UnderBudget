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

#ifndef XMLBUDGETWRITER_HPP
#define XMLBUDGETWRITER_HPP

// Qt include(s)
#include <QSharedPointer>

// UnderBudget include(s)
#include "budget/AssignmentRule.hpp"

// Forward declaration(s)
class QIODevice;

namespace ub {

// Forward declaration(s)
class AssignmentRules;
class Balance;
class Budget;
class BudgetingPeriod;
class Estimate;
class Money;

/**
 * XML stream writer for serializing a budget as an XML document according
 * to the version 5.0 XML budget specification.
 *
 * Since the `QXmlStreamWriter` used for writing is not intended to be
 * re-used (e.g., the error condition flag can't be reset), it doesn't
 * really make sense for anyone to actually have an instantiation of
 * this class. Therefore, a single static function is provided to perform
 * the write operation.
 *
 * @ingroup budget_storage
 */
class XmlBudgetWriter
{
public:
	/**
	 * Writes the budget as an XML document to the given IO device.
	 *
	 * @param[in] device IO device to which to write the XML
	 * @param[in] budget budget to be serialized
	 * @return `true` if successful in serializing the budget to the XML stream
	 */
	static bool write(QIODevice* device, const QSharedPointer<Budget> budget);

private:
	/** XML stream writer */
	QXmlStreamWriter xml;

	/**
	 * Constructs a new XML budget writer.
	 *
	 * @param[in] device IO device to which to write the XML
	 */
	XmlBudgetWriter(QIODevice* device);

	/**
	 * Writes the given budget to the XML stream.
	 *
	 * @param[in] budget budget to be serialized
	 * @return `true` if successful in serializing the budget to the XML stream
	 */
	bool write(const QSharedPointer<Budget> budget);

	/**
	 * Writes the given money value to the XML stream.
	 *
	 * @param[in] ns    namespace for the money element
	 * @param[in] tag   name of the element to contain the money value
	 * @param[in] value money value to be written
	 */
	void write(const QString& ns, const QString& tag, const Money& value);

	/**
	 * Writes the given balance to the XML stream.
	 *
	 * @param[in] ns      namespace for the balance element
	 * @param[in] tag     name of the element to contain the balance
	 * @param[in] balance balance to be serialized
	 */
	void write(const QString& ns, const QString& tag,
		const QSharedPointer<Balance> balance);

	/**
	 * Writes the given budgeting period to the XML stream.
	 *
	 * @param[in] period budgeting period to be serialized
	 */
	void write(const QSharedPointer<BudgetingPeriod> period);

	/**
	 * Writes the given estimate to the XML stream. All children to this
	 * estimate will also be written, recursively.
	 *
	 * @param[in] estimate estimate to be serialized
	 */
	void write(const Estimate* estimate);

	/**
	 * Writes the given assignment rules list to the XML stream.
	 *
	 * @param[in] rules assignment rules list
	 */
	void write(const QSharedPointer<AssignmentRules> rules);

	/**
	 * Writes the given assignment rule to the XML stream.
	 *
	 * @param[in] rule assignment rule to be serialized
	 */
	void write(const AssignmentRule* rule);

	/**
	 * Writes the given rule condition to the XML stream.
	 *
	 * @param[in] condition rule condition to be serialized
	 */
	void write(const AssignmentRule::Condition& condition);
};

}

#endif //XMLBUDGETWRITER_HPP
