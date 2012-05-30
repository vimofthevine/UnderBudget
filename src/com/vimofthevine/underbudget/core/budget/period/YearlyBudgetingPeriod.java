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

import java.util.Calendar;

import org.simpleframework.xml.Attribute;
import org.simpleframework.xml.Element;
import org.simpleframework.xml.Root;

import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * Budgeting period based on a calendar year, starting
 * on the first Friday of the year and ending on the
 * last Thursday (before the next year's first Friday)
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
@Root(name="period")
public class YearlyBudgetingPeriod implements BudgetingPeriod {
	
	/**
	 * Budgeting period type identifier
	 */
	@Attribute(name="type")
	public static final String TYPE = "yearly";
	
	/**
	 * The year of the budgeting period
	 */
	@Element
	public int year;
	
	/**
	 * Default constructor
	 */
	public YearlyBudgetingPeriod()
	{
		// Initialize period to current year
		Calendar cal = Calendar.getInstance();
		year = cal.get(Calendar.YEAR);
	}

	/**
	 * Constructor, given the calendar year
	 * 
	 * @param year year defining the budgeting period
	 */
	public YearlyBudgetingPeriod(int year)
	{
		this.year = year;
	}
	
	/**
	 * Determines the start date, which is the
	 * first Friday of the specified year
	 * 
	 * @return date of the first Friday of the specified year
	 */
	@Override
	public SimpleDate getStartDate()
	{
		// Set calendar to January of the selected year
		Calendar cal = Calendar.getInstance();
		cal.clear();
		cal.set(year, 0, 15);
		
		// Get first Friday of specified month
		cal.set(Calendar.DAY_OF_WEEK, Calendar.FRIDAY);
		cal.set(Calendar.DAY_OF_WEEK_IN_MONTH, 1);
		
		return new SimpleDate(cal.getTime());
	}
	
	/**
	 * Determines the end date, which is the Thursday
	 * before the first Friday of the year after
	 * the specified year
	 * 
	 * @return date of the Thursday before the next year
	 */
	@Override
	public SimpleDate getEndDate()
	{
		// Set calendar to January after selected year
		Calendar cal = Calendar.getInstance();
		cal.clear();
		cal.set(year + 1, 0, 15);
		
		// Get first Friday of specified month
		cal.set(Calendar.DAY_OF_WEEK, Calendar.FRIDAY);
		cal.set(Calendar.DAY_OF_WEEK_IN_MONTH, 1);
		
		// Move back a day to Thursday
		cal.add(Calendar.DAY_OF_WEEK, -1);
		
		return new SimpleDate(cal.getTime());
	}
	
	@Override
	public boolean contains(SimpleDate when)
	{
		return ! (when.before(getStartDate()) || when.after(getEndDate()));
	}
	
	/**
	 * Compares this budgeting period to a given object
	 * 
	 * @param obj object to which to be compared
	 */
	@Override
	public boolean equals(Object obj)
	{
		if (obj instanceof YearlyBudgetingPeriod)
		{
			YearlyBudgetingPeriod that = (YearlyBudgetingPeriod) obj;
			
			return (this.year == that.year);
		}
		else
			return false;
	}
	
	/**
	 * Returns a string representation of the period
	 */
	@Override
	public String toString()
	{
		return String.valueOf(year);
	}
	
}
