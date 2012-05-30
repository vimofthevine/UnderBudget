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
 * Budgeting period based on a calendar month, starting
 * on the first Friday of the month and ending on the
 * last Thursday (before the next month's first Friday)
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
@Root(name="period")
public class MonthlyBudgetingPeriod implements BudgetingPeriod {
	
	/**
	 * Budgeting period type identifier
	 */
	@Attribute(name="type")
	public static final String TYPE = "monthly";

	/**
	 * The month of the budgeting period
	 */
	@Element
	public int month;
	
	/**
	 * The year of the budgeting period
	 */
	@Element
	public int year;
	
	/**
	 * Default constructor
	 */
	public MonthlyBudgetingPeriod()
	{
		// Initialize period to current month/year
		Calendar cal = Calendar.getInstance();
		month = cal.get(Calendar.MONTH);
		year  = cal.get(Calendar.YEAR);
	}
	
	/**
	 * Constructor, given the calendar month
	 * 
	 * @param month month defining the budgeting period
	 * @param year  year defining the budgeting period
	 */
	public MonthlyBudgetingPeriod(int month, int year)
	{
		this.month = month;
		this.year  = year;
	}
	
	/**
	 * Determines the start date, which is the
	 * first Friday of the specified month
	 * 
	 * @return date of the first Friday of the specified month
	 */
	@Override
	public SimpleDate getStartDate()
    {
		// Set calendar to selected month/year
		Calendar cal = Calendar.getInstance();
		cal.clear();
		cal.set(year, month, 15);
		
		// Get first Friday of specified month
		cal.set(Calendar.DAY_OF_WEEK, Calendar.FRIDAY);
		cal.set(Calendar.DAY_OF_WEEK_IN_MONTH, 1);
		
		return new SimpleDate(cal.getTime());
    }

	/**
	 * Determines the end date, which is the Thursday
	 * before the first Friday of the month after
	 * the specified month
	 * 
	 * @return date of the Thursday before the next month
	 */
	@Override
	public SimpleDate getEndDate()
    {
		// Set calendar to month after selected month/year
		Calendar cal = Calendar.getInstance();
		cal.clear();
		cal.set(year, month + 1, 15);
		
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
		if (obj instanceof MonthlyBudgetingPeriod)
		{
			MonthlyBudgetingPeriod that = (MonthlyBudgetingPeriod) obj;
			
			return (this.month == that.month)
				&& (this.year == that.year);
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
		return month + "/" + year;
	}

}
