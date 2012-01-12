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
	protected EstimateDomParser[] expectedDomParsers;
	
	/**
	 * Sets up the test objects
	 */
	@Before
	public void setUp()
	{
		expectedDomParsers = new EstimateDomParser[4];
		expectedDomParsers[1] = new EstimateDomParserV1(null);
		expectedDomParsers[2] = new EstimateDomParserV2(null);
		expectedDomParsers[3] = new EstimateDomParserV3(null);
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
			EstimateDomParser parser = EstimateParserFactory.createDomParser(i, null);
			
			assertEquals(expectedDomParsers[i].getClass().getName(), parser.getClass().getName());
		}
	}

}
