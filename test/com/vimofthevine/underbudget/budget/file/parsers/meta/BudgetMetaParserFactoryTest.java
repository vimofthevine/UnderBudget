package com.vimofthevine.underbudget.budget.file.parsers.meta;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

/**
 * Unit test case for the BudgetMetaParserFactory class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetMetaParserFactoryTest {
	
	/**
	 * Array of file versions vs. parser classes
	 */
	protected BudgetMetaDomParser[] expectedDomParsers;
	
	/**
	 * Sets up the test objects
	 */
	@Before
	public void setUp()
	{
		expectedDomParsers = new BudgetMetaDomParser[4];
		expectedDomParsers[1] = new BudgetMetaDomParserV1(null);
		expectedDomParsers[2] = new BudgetMetaDomParserV1(null);
		expectedDomParsers[3] = new BudgetMetaDomParserV3(null);
	}

	/**
	 * Verifies that the correct parser is created
	 * for each file version
	 */
	@Test
	public void testCreateDomParser()
	{
		// Iterate through each version/parser
		for (int i=1; i<expectedDomParsers.length; i++)
		{
			BudgetMetaDomParser parser = BudgetMetaParserFactory.createDomParser(i, null);
			
			assertEquals(expectedDomParsers[i].getClass().getName(), parser.getClass().getName());
		}
	}

}
