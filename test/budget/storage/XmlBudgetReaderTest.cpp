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
#include "XmlBudgetReaderTest.hpp"
#include "budget/storage/XmlBudgetReader.hpp"

//------------------------------------------------------------------------------
QTEST_MAIN(ub::XmlBudgetReaderTest)

namespace ub {

//------------------------------------------------------------------------------
void XmlBudgetReaderTest::readInvalidXml()
{
	QBuffer buffer;
	buffer.open(QBuffer::ReadWrite);
	buffer.write("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
		"<budget version=\"4.0\">"
		" <name>Invalid Budget</name>"
		" <estimate id=\"0\" version=\"1.1\" uid=\"12341234\">"
		"  <name>Root" // missing closing tag
		"  <amount currency=\"USD\">0.00</amount>"
		"  <type>EXPENSE</type>"
		"  <complete>false</complete>"
		"  <estimates />"
		" </estimate>"
		" <rules>"
		"  <rule>"
		"   <estimate reference=\"3\"/>"
		"   <conditions>"
		"    <condition>"
		"     <field>DEPOSIT</field>"
		"     <operator>ENDS_WITH</operator>"
		"     <value>condition_text</value>"
		"    </condition>"
		"   </conditions>"
		"  </rule>"
		" </rules>"
		"</budget>");
	buffer.seek(0);

	XmlBudgetReader reader;

	// Make sure no budget was read
	QCOMPARE(reader.read(&buffer), false);
	QCOMPARE(reader.lastReadBudget().isNull(), true);
}

//------------------------------------------------------------------------------
void XmlBudgetReaderTest::readNonBudgetXml()
{
	QBuffer buffer;
	buffer.open(QBuffer::ReadWrite);
	buffer.write("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
		"<document>"
		"  <element/>"
		"  <element/>"
		"</document>");
	buffer.seek(0);

	XmlBudgetReader reader;

	// Make sure no budget was read
	QCOMPARE(reader.read(&buffer), false);
	QCOMPARE(reader.lastReadBudget().isNull(), true);
}

//------------------------------------------------------------------------------
void XmlBudgetReaderTest::readOldVersion()
{
	QBuffer buffer;
	buffer.open(QBuffer::ReadWrite);
	buffer.write("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
		"<budget version=\"3.0\">"
		" <name>Old Version Budget</name>"
		"</budget>");
	buffer.seek(0);

	XmlBudgetReader reader;

	// Make sure no budget was read
	QCOMPARE(reader.read(&buffer), false);
	QCOMPARE(reader.lastReadBudget().isNull(), true);
}

}

