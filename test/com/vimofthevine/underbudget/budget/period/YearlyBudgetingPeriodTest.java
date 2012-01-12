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
 * Unit test case for the YearlyBudgetingPeriod class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class YearlyBudgetingPeriodTest {

	/**
	 * Verifies the calculated start dates
	 */
	@Test
	public final void testGetStartDate()
	{
		YearlyBudgetingPeriod period;
		Calendar cal = Calendar.getInstance();
		cal.clear();
		
		period = new YearlyBudgetingPeriod(2008);
		cal.set(2008, 0, 4);
		assertEquals(cal.getTime(), period.getStartDate());
		
		period = new YearlyBudgetingPeriod(2009);
		cal.set(2009, 0, 2);
		assertEquals(cal.getTime(), period.getStartDate());
		
		period = new YearlyBudgetingPeriod(2010);
		cal.set(2010, 0, 1);
		assertEquals(cal.getTime(), period.getStartDate());
		
		period = new YearlyBudgetingPeriod(2011);
		cal.set(2011, 0, 7);
		assertEquals(cal.getTime(), period.getStartDate());
		
		period = new YearlyBudgetingPeriod(2012);
		cal.set(2012, 0, 6);
		assertEquals(cal.getTime(), period.getStartDate());
	}

	/**
	 * Verifies the calculated end dates
	 */
	@Test
	public final void testGetEndDate()
	{
		YearlyBudgetingPeriod period;
		Calendar cal = Calendar.getInstance();
		cal.clear();
		
		period = new YearlyBudgetingPeriod(2008);
		cal.set(2009, 0, 1);
		assertEquals(cal.getTime(), period.getEndDate());
		
		period = new YearlyBudgetingPeriod(2009);
		cal.set(2009, 11, 31);
		assertEquals(cal.getTime(), period.getEndDate());
		
		period = new YearlyBudgetingPeriod(2010);
		cal.set(2011, 0, 6);
		assertEquals(cal.getTime(), period.getEndDate());
		
		period = new YearlyBudgetingPeriod(2011);
		cal.set(2012, 0, 5);
		assertEquals(cal.getTime(), period.getEndDate());
		
		period = new YearlyBudgetingPeriod(2012);
		cal.set(2013, 0, 3);
		assertEquals(cal.getTime(), period.getEndDate());
	}

	/**
	 * Verifies the yearly budgeting period is stored in XML
	 */
	@Test
	public final void testStore()
	{
		try
		{
			YearlyBudgetingPeriod period = new YearlyBudgetingPeriod(2011);
			Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument();
			Element element = doc.createElement("period");
			
			// Store the period in the element
			period.store(doc, element);
			
			// Verify element now defines the period
			assertEquals(YearlyBudgetingPeriod.TYPE, element.getAttribute("type"));
			assertEquals("2011", XmlHelper.readTextNode(element, "year"));
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}

	/**
	 * Verifies the yearly budgeting period is parsed from XML
	 */
	@Test
	public final void testParse()
	{
		try
		{
			YearlyBudgetingPeriod period = new YearlyBudgetingPeriod();
			Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument();
			Element element = doc.createElement("period");
			
			// Verify nothing parsed
			assertNull(period.parse(element));
			
			// Populate element with period definition
			element.setAttribute("type", YearlyBudgetingPeriod.TYPE);
			Element yearElement = doc.createElement("year");
			yearElement.appendChild(doc.createTextNode("2011"));
			element.appendChild(yearElement);
			
			// Verify period is parsed
			BudgetingPeriod parsed = period.parse(element);
			assertNotNull(parsed);
			assertTrue(parsed instanceof YearlyBudgetingPeriod);
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}

}
