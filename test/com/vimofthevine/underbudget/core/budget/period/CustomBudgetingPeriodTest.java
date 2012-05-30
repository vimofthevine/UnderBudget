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

package com.vimofthevine.underbudget.core.budget.period;

import static org.junit.Assert.*;

import java.util.HashMap;
import java.util.Map.Entry;

import org.junit.Test;

import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * Unit test case for the CustomBudgetingPeriod class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CustomBudgetingPeriodTest {
	
	/**
	 * Verifies the ability to determine if
	 * a given date is within the budgeting period
	 */
	@Test
	public final void testContains()
	{
		CustomBudgetingPeriod period = new CustomBudgetingPeriod(
			new SimpleDate(2, 0, 2012), new SimpleDate(28, 0, 2012));
		
		HashMap<SimpleDate,Boolean> testCases = new HashMap<SimpleDate,Boolean>();
		testCases.put(new SimpleDate(31, 11, 2011), false);
		testCases.put(new SimpleDate(1, 0, 2012), false);
		testCases.put(new SimpleDate(2, 0, 2012), true);
		testCases.put(new SimpleDate(3, 0, 2012), true);
		testCases.put(new SimpleDate(4, 0, 2012), true);
		testCases.put(new SimpleDate(26, 0, 2012), true);
		testCases.put(new SimpleDate(27, 0, 2012), true);
		testCases.put(new SimpleDate(28, 0, 2012), true);
		testCases.put(new SimpleDate(29, 0, 2012), false);
		testCases.put(new SimpleDate(30, 0, 2012), false);
		
		for (Entry<SimpleDate,Boolean> entry : testCases.entrySet())
		{
			SimpleDate input = entry.getKey();
			boolean expected = entry.getValue();
			String msg = "Expected " + input + " to be " +
				(expected ? "within" : "outside") +
				" the budgeting period, " + period;
			
			assertEquals(msg, expected, period.contains(input));
		}
	}

}
