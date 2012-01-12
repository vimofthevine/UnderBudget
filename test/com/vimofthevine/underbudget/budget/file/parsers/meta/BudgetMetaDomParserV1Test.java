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

package com.vimofthevine.underbudget.budget.file.parsers.meta;

import static org.junit.Assert.*;

import java.math.BigDecimal;

import javax.xml.parsers.DocumentBuilderFactory;

import org.junit.Before;
import org.junit.Test;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import com.vimofthevine.underbudget.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.budget.period.MonthlyBudgetingPeriod;

/**
 * Unit test case for the BudgetMetaDomParserV1 class 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetMetaDomParserV1Test {
	
	/**
	 * The class under test
	 */
	BudgetMetaDomParserV1 parser;
	
	/**
	 * Budget meta DOM element
	 */
	Element metaElement;
	
	/**
	 * Sets up the test objects
	 */
	@Before
	public void setUp()
	{
		parser = new BudgetMetaDomParserV1(null);
		createTestDocument();
	}

	/**
	 * Verifies that the name element is parsed
	 */
	@Test
	public void testParseName()
	{
		try
		{
			String name = parser.parseName(metaElement);
			assertEquals("junit budget", name);
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}

	/**
	 * Verifies that the initial balance element is parsed
	 */
	@Test
	public void testParseInitialBalance()
	{
		try
		{
			BigDecimal value = parser.parseInitialBalance(metaElement);
			assertEquals(new BigDecimal("12.34"), value);
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}

	/**
	 * Verifies that the budgeting period is parsed as
	 * a monthly budgeting period
	 */
	@Test
	public void testParseBudgetingPeriod()
	{
		try
		{
			BudgetingPeriod period = parser.parseBudgetingPeriod(metaElement);
			assertTrue(period instanceof MonthlyBudgetingPeriod);
			
			MonthlyBudgetingPeriod monthly = (MonthlyBudgetingPeriod) period;
			assertEquals(12, monthly.month);
			assertEquals(2011, monthly.year);
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}
	
	/**
	 * Creates a test budget meta DOM element
	 * 
	 * @return budget meta element
	 */
	protected void createTestDocument()
	{
		try
		{
			Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument();
			metaElement = doc.createElement("meta");
			
			Element nameElement = doc.createElement("name");
			nameElement.appendChild(doc.createTextNode("junit budget"));
			
			Element initialElement = doc.createElement("initialBalance");
			initialElement.appendChild(doc.createTextNode("12.34"));
			
			Element monthElement = doc.createElement("month");
			monthElement.appendChild(doc.createTextNode("12"));
			
			Element yearElement = doc.createElement("year");
			yearElement.appendChild(doc.createTextNode("2011"));
			
			metaElement.appendChild(nameElement);
			metaElement.appendChild(initialElement);
			metaElement.appendChild(monthElement);
			metaElement.appendChild(yearElement);
			
			doc.appendChild(metaElement);
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}

}
