package com.vimofthevine.underbudget.budget.file.parsers.estimate;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

/**
 * Unit test case for the EstimateParserFactory class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateParserFactoryTest {
	
	/**
	 * Array of file versions vs. parser classes
	 */
	protected EstimateDomParser[] expected;
	
	/**
	 * Sets up the test objects
	 */
	@Before
	public void setUp()
	{
		expected = new EstimateDomParser[4];
		expected[1] = new EstimateDomParserV1(null);
		expected[2] = new EstimateDomParserV2(null);
		expected[3] = new EstimateDomParserV3(null);
	}

	/**
	 * Verifies that the correct parser is created
	 * for each file version
	 */
	@Test
	public void testCreateParser()
	{
		// Iterate through each version/parser
		for (int i=1; i<expected.length; i++)
		{
			EstimateDomParser parser = EstimateParserFactory.createParser(i, null);
			
			assertEquals(expected[i].getClass().getName(), parser.getClass().getName());
		}
	}

}
