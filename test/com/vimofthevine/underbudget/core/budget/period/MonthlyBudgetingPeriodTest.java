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

import java.util.Calendar;
import java.util.HashMap;
import java.util.Map.Entry;

import org.junit.Test;

import com.vimofthevine.underbudget.core.util.SimpleDate;

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
				new SimpleDate(days[i], i, 2011), period.getStartDate());
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
				new SimpleDate(days[i][1], days[i][0], 2011), period.getEndDate());
		}
	}

	/**
	 * Verifies the ability to determine if
	 * a given date is within the budgeting period
	 */
	@Test
	public final void testContains()
	{
		MonthlyBudgetingPeriod period = new MonthlyBudgetingPeriod(2, 2012);
		
		HashMap<SimpleDate,Boolean> testCases = new HashMap<SimpleDate,Boolean>();
		testCases.put(new SimpleDate(29, 1, 2012), false);
		testCases.put(new SimpleDate(1, 2, 2012), false);
		testCases.put(new SimpleDate(2, 2, 2012), true);
		testCases.put(new SimpleDate(3, 2, 2012), true);
		testCases.put(new SimpleDate(4, 2, 2012), true);
		testCases.put(new SimpleDate(31, 2, 2012),true);
		testCases.put(new SimpleDate(4, 3, 2012), true);
		testCases.put(new SimpleDate(5, 3, 2012), true);
		testCases.put(new SimpleDate(6, 3, 2012), false);
		testCases.put(new SimpleDate(7, 3, 2012), false);
		testCases.put(new SimpleDate(8, 3, 2012), false);
		
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
