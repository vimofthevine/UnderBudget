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

package com.vimofthevine.underbudget.core.util;

import static org.junit.Assert.*;

import java.util.HashMap;
import java.util.Map.Entry;

import org.junit.Test;

/**
 * Unit test case for the SimpleDate class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class SimpleDateTest {

	/**
	 * Verifies the ability to perform before comparisons
	 */
	@Test
	public final void testBeforeSimpleDate()
	{
		SimpleDate date = new SimpleDate(18, 2, 2012);
		
		HashMap<SimpleDate,Boolean> testCases = new HashMap<SimpleDate,Boolean>();
		testCases.put(new SimpleDate(1, 6, 2010), true);   // year before
		testCases.put(new SimpleDate(31, 1, 2012), true);  // month before
		testCases.put(new SimpleDate(3, 2, 2012), true);   // earlier in month
		testCases.put(new SimpleDate(17, 2, 2012), true);  // day before
		testCases.put(new SimpleDate(18, 2, 2012), false); // day of
		testCases.put(new SimpleDate(19, 2, 2012), false); // day later
		testCases.put(new SimpleDate(29, 2, 2012), false); // later in month
		testCases.put(new SimpleDate(3, 3, 3012), false);  // month later
		testCases.put(new SimpleDate(10, 2, 2013), false); // year later
		
		for (Entry<SimpleDate,Boolean> entry : testCases.entrySet())
		{
			SimpleDate input = entry.getKey();
			boolean expected = entry.getValue();
			String msg = "Expected date, " + input + ", to be " +
				(expected ? "before" : "after") + " " + date;
				
			assertEquals(msg, expected, input.before(date));
		}
	}

	/**
	 * Verifies the ability to perform after comparisons
	 */
	@Test
	public final void testAfterSimpleDate()
	{
		SimpleDate date = new SimpleDate(18, 2, 2012);
		
		HashMap<SimpleDate,Boolean> testCases = new HashMap<SimpleDate,Boolean>();
		testCases.put(new SimpleDate(1, 6, 2010), false);   // year before
		testCases.put(new SimpleDate(31, 1, 2012), false);  // month before
		testCases.put(new SimpleDate(3, 2, 2012), false);   // earlier in month
		testCases.put(new SimpleDate(17, 2, 2012), false);  // day before
		testCases.put(new SimpleDate(18, 2, 2012), false); // day of
		testCases.put(new SimpleDate(19, 2, 2012), true); // day later
		testCases.put(new SimpleDate(29, 2, 2012), true); // later in month
		testCases.put(new SimpleDate(3, 3, 3012), true);  // month later
		testCases.put(new SimpleDate(10, 2, 2013), true); // year later
		
		for (Entry<SimpleDate,Boolean> entry : testCases.entrySet())
		{
			SimpleDate input = entry.getKey();
			boolean expected = entry.getValue();
			String msg = "Expected date, " + input + ", to be " +
				(expected ? "after" : "before") + " " + date;
			
			assertEquals(msg, expected, input.after(date));
		}
	}

}
