package com.vimofthevine.underbudget.budget.period;

import static org.junit.Assert.*;

import java.util.Calendar;

import javax.xml.parsers.DocumentBuilderFactory;

import org.junit.Test;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import com.vimofthevine.underbudget.util.XmlHelper;

/**
 * Unit test case for the MonthlyBudgetingPeriod class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class MonthlyBudgetingPeriodTest {

	/**
	 * Verifies the calculated start dates
	 */
	@Test
	public void testGetStartDate()
	{
		MonthlyBudgetingPeriod period;
		Calendar cal = Calendar.getInstance();
		cal.clear();
		
		int[] days = {7, 4, 4, 1, 6, 3, 1, 5, 2, 7, 4, 2};
		
		for (int i=0; i<12; i++)
		{
			period = new MonthlyBudgetingPeriod(i, 2011);
			cal.set(2011, i, days[i]);
			assertEquals("Start date not correct for month, " + (i+1) + "/2011",
				cal.getTime(), period.getStartDate());
		}
	}

	/**
	 * Verifies the calculated end dates
	 */
	@Test
	public void testGetEndDate()
	{
		MonthlyBudgetingPeriod period;
		Calendar cal = Calendar.getInstance();
		cal.clear();
		
		int[][] days = {
			{1,3}, {2,3}, {2,31}, {4,5}, {5,2}, {5,30},
			{7,4}, {8,1}, {9,6}, {10,3}, {11,1}, {12,5}
		};
		
		for (int i=0; i<12; i++)
		{
			period = new MonthlyBudgetingPeriod(i, 2011);
			cal.set(2011, days[i][0], days[i][1]);
			assertEquals("End date not correct for month, " + (i+1) + "/2011",
				cal.getTime(), period.getEndDate());
		}
	}

	/**
	 * Verifies the monthly budgeting period is stored in XML
	 */
	@Test
	public void testStore()
	{
		try
		{
			MonthlyBudgetingPeriod period = new MonthlyBudgetingPeriod(12, 2011);
			Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument();
			Element element = doc.createElement("period");
			
			// Store the period in the element
			period.store(doc, element);
			
			// Verify element now defines the period
			assertEquals(MonthlyBudgetingPeriod.TYPE, element.getAttribute("type"));
			assertEquals("12", XmlHelper.readTextNode(element, "month"));
			assertEquals("2011", XmlHelper.readTextNode(element, "year"));
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}

	/**
	 * Verifies the monthly budgeting period is parsed from XML
	 */
	@Test
	public void testParse()
	{
		try
		{
			MonthlyBudgetingPeriod period = new MonthlyBudgetingPeriod();
			Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument();
			Element element = doc.createElement("period");
			
			// Verify nothing parsed
			assertNull(period.parse(element));
			
			// Populate element with period definition
			element.setAttribute("type", MonthlyBudgetingPeriod.TYPE);
			Element monthElement = doc.createElement("month");
			Element yearElement  = doc.createElement("year");
			monthElement.appendChild(doc.createTextNode("4"));
			yearElement.appendChild(doc.createTextNode("2011"));
			element.appendChild(monthElement);
			element.appendChild(yearElement);
			
			// Verify period is parsed
			BudgetingPeriod parsed = period.parse(element);
			assertNotNull(parsed);
			assertTrue(parsed instanceof MonthlyBudgetingPeriod);
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}

}
