package com.vimofthevine.underbudget.budget.file.parsers.meta;

import static org.junit.Assert.*;

import java.util.Date;

import javax.xml.parsers.DocumentBuilderFactory;

import org.junit.Before;
import org.junit.Test;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import com.vimofthevine.underbudget.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.budget.period.MonthlyBudgetingPeriod;

/**
 * Unit test case for the BudgetMetaDomParserV3 class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetMetaDomParserV3Test {
	
	/**
	 * The class under test
	 */
	BudgetMetaDomParserV3 parser;
	
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
		parser = new BudgetMetaDomParserV3(null);
		createTestDocument();
	}
	
	/**
	 * Verifies that a monthly budget is created
	 * if the period type is unknown
	 */
	@Test
	public void testParseBudgetingPeriodDefault()
	{
		try
		{
			BudgetingPeriod period = parser.parseBudgetingPeriod(metaElement);
			assertTrue(period instanceof MonthlyBudgetingPeriod);
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}
	
	/**
	 * Verifies that stub budgeting period is
	 * detected and parsed
	 */
	@Test
	public void testParseBudgetingPeriod()
	{
		try
		{
			parser.types = new BudgetingPeriod[] { new StubBudgetingPeriod() };
			
			BudgetingPeriod period = parser.parseBudgetingPeriod(metaElement);
			assertTrue(period instanceof StubBudgetingPeriod);
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}

	/**
	 * Creates a test budget meta DOM element
	 */
	protected void createTestDocument()
	{
		try
		{
			Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument();
			metaElement = doc.createElement("meta");
			
			Element periodElement = doc.createElement("period");
			periodElement.setAttribute("type", "stub");
			
			metaElement.appendChild(periodElement);
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}
	
	/**
	 * Stub budgeting period
	 */
	class StubBudgetingPeriod implements BudgetingPeriod {

		@Override
        public Date getStartDate()
        {
	        return null;
        }

		@Override
        public Date getEndDate()
        {
	        return null;
        }

		@Override
        public void store(Document doc, Element element)
        {
			// Do nothing
        }

		@Override
        public BudgetingPeriod parse(Element element)
        {
			if (element.getAttribute("type").equals("stub"))
				return new StubBudgetingPeriod();
			else
				return null;
        }
		
	}

}
