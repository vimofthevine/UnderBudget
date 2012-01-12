/*
 * Copyright 2012 Kyle Treubig
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

package com.vimofthevine.underbudget.budget.period;

import static org.junit.Assert.*;

import java.util.Calendar;

import javax.xml.parsers.DocumentBuilderFactory;

import org.junit.Test;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import com.vimofthevine.underbudget.util.XmlHelper;

/**
 * Unit test case for the CustomBudgetingPeriod class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CustomBudgetingPeriodTest {

	/**
	 * Verifies the returned start date
	 */
	@Test
	public final void testGetStartDate()
	{
		CustomBudgetingPeriod period = new CustomBudgetingPeriod(2012, 0, 2, 2012, 0, 28);
		
		Calendar cal = Calendar.getInstance();
		cal.set(2012, 0, 2);
		assertEquals(cal.getTime(), period.getStartDate());
	}

	/**
	 * Verifies the returned end date
	 */
	@Test
	public final void testGetEndDate()
	{
		CustomBudgetingPeriod period = new CustomBudgetingPeriod(2012, 0, 2, 2012, 0, 28);
		
		Calendar cal = Calendar.getInstance();
		cal.set(2012, 0, 28);
		assertEquals(cal.getTime(), period.getEndDate());
	}

	/**
	 * Verifies the custom budgeting period is stored in XML
	 */
	@Test
	public final void testStore()
	{
		try
		{
			CustomBudgetingPeriod period = new CustomBudgetingPeriod(2012, 0, 2, 2012, 0, 28);
			Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument();
			Element element = doc.createElement("period");
			
			// Store the period in the element
			period.store(doc, element);
			
			// Verify element now defines the period
			assertEquals(CustomBudgetingPeriod.TYPE, element.getAttribute("type"));
			
			Element startElement = (Element) element.getElementsByTagName("start-date").item(0);
			assertEquals("2012", XmlHelper.readTextNode(startElement, "year"));
			assertEquals("0", XmlHelper.readTextNode(startElement, "month"));
			assertEquals("2", XmlHelper.readTextNode(startElement, "day"));
			
			Element endElement   = (Element) element.getElementsByTagName("end-date").item(0);
			assertEquals("2012", XmlHelper.readTextNode(endElement, "year"));
			assertEquals("0", XmlHelper.readTextNode(endElement, "month"));
			assertEquals("28", XmlHelper.readTextNode(endElement, "day"));
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}

	/**
	 * Verifies the custom budgeting period is parsed from XML
	 */
	@Test
	public final void testParse()
	{
		try
		{
			CustomBudgetingPeriod period = new CustomBudgetingPeriod();
			Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument();
			Element element = doc.createElement("period");
			
			// Verify nothing parsed
			assertNull(period.parse(element));
			
			// Populate element with period definition
			element.setAttribute("type", CustomBudgetingPeriod.TYPE);
			Element startElement = doc.createElement("start-date");
			element.appendChild(startElement);
			
			Element startYear    = doc.createElement("year");
			Element startMonth   = doc.createElement("month");
			Element startDay     = doc.createElement("day");
			
			startYear.appendChild(doc.createTextNode("2012"));
			startMonth.appendChild(doc.createTextNode("1"));
			startDay.appendChild(doc.createTextNode("8"));
			
			startElement.appendChild(startYear);
			startElement.appendChild(startMonth);
			startElement.appendChild(startDay);
			
			Element endElement = doc.createElement("end-date");
			element.appendChild(endElement);
			
			Element endYear  = doc.createElement("year");
			Element endMonth = doc.createElement("month");
			Element endDay   = doc.createElement("day");
			
			endYear.appendChild(doc.createTextNode("2012"));
			endMonth.appendChild(doc.createTextNode("1"));
			endDay.appendChild(doc.createTextNode("29"));
			
			endElement.appendChild(endYear);
			endElement.appendChild(endMonth);
			endElement.appendChild(endDay);
			
			// Verify period is parsed
			BudgetingPeriod parsed = period.parse(element);
			assertNotNull(parsed);
			assertTrue(parsed instanceof CustomBudgetingPeriod);
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}

}
