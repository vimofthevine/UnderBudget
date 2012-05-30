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
		
		period = new YearlyBudgetingPeriod(2008);
		assertEquals(new SimpleDate(4, 0, 2008), period.getStartDate());
		
		period = new YearlyBudgetingPeriod(2009);
		assertEquals(new SimpleDate(2, 0, 2009), period.getStartDate());
		
		period = new YearlyBudgetingPeriod(2010);
		assertEquals(new SimpleDate(1, 0, 2010), period.getStartDate());
		
		period = new YearlyBudgetingPeriod(2011);
		assertEquals(new SimpleDate(7, 0, 2011), period.getStartDate());
		
		period = new YearlyBudgetingPeriod(2012);
		assertEquals(new SimpleDate(6, 0, 2012), period.getStartDate());
	}

	/**
	 * Verifies the calculated end dates
	 */
	@Test
	public final void testGetEndDate()
	{
		YearlyBudgetingPeriod period;
		
		period = new YearlyBudgetingPeriod(2008);
		assertEquals(new SimpleDate(1, 0, 2009), period.getEndDate());
		
		period = new YearlyBudgetingPeriod(2009);
		assertEquals(new SimpleDate(31, 11, 2009), period.getEndDate());
		
		period = new YearlyBudgetingPeriod(2010);
		assertEquals(new SimpleDate(6, 0, 2011), period.getEndDate());
		
		period = new YearlyBudgetingPeriod(2011);
		assertEquals(new SimpleDate(5, 0, 2012), period.getEndDate());
		
		period = new YearlyBudgetingPeriod(2012);
		assertEquals(new SimpleDate(3, 0, 2013), period.getEndDate());
	}

	/**
	 * Verifies the ability to determine if
	 * a given date is within the budgeting period
	 */
	@Test
	public final void testContains()
	{
		YearlyBudgetingPeriod period = new YearlyBudgetingPeriod(2011);
		
		HashMap<SimpleDate,Boolean> testCases = new HashMap<SimpleDate,Boolean>();
		testCases.put(new SimpleDate(5, 0, 2011), false);
		testCases.put(new SimpleDate(6, 0, 2011), false);
		testCases.put(new SimpleDate(7, 0, 2011), true);
		testCases.put(new SimpleDate(8, 0, 2011), true);
		testCases.put(new SimpleDate(9, 0, 2011), true);
		testCases.put(new SimpleDate(31, 11, 2011), true);
		testCases.put(new SimpleDate(4, 0, 2012), true);
		testCases.put(new SimpleDate(5, 0, 2012), true);
		testCases.put(new SimpleDate(6, 0, 2012), false);
		testCases.put(new SimpleDate(7, 0, 2012), false);
		testCases.put(new SimpleDate(8, 0, 2012), false);
		
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
