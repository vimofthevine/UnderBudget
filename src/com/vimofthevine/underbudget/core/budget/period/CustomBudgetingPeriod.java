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
 * Budgeting period based on user-specified start
 * and end dates
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
@Root(name="period")
public class CustomBudgetingPeriod implements BudgetingPeriod {

	/**
	 * Budgeting period type identifier
	 */
	@Attribute(name="type")
	public static final String TYPE = "custom";
	
	/**
	 * Start date
	 */
	@Element(name="start-date")
	private SimpleDate startDate;
	
	/**
	 * End date
	 */
	@Element(name="end-date")
	private SimpleDate endDate;
	
	/**
	 * Constructs a new custom budgeting period with the
	 * current month's start and end dates.
	 */
	public CustomBudgetingPeriod()
	{
		// Initialize period to current month
		Calendar cal = Calendar.getInstance();
		
		// Set the start day (1st of the month)
		int startDay = 1;
		int startMonth = cal.get(Calendar.MONTH);
		int startYear = cal.get(Calendar.YEAR);
		
		// Calculate the end day (last day of the month)
		cal.add(Calendar.MONTH, 1);
		cal.add(Calendar.DAY_OF_MONTH, -1);
		int endDay = cal.get(Calendar.DAY_OF_MONTH);
		int endMonth = cal.get(Calendar.MONTH);
		int endYear = cal.get(Calendar.YEAR);
		
		startDate = new SimpleDate(startDay, startMonth, startYear);
		endDate = new SimpleDate(endDay, endMonth, endYear);
	}
	
	/**
	 * Constructor, given the start and end dates
	 * 
	 * @param startYear  year of the start date
	 * @param startMonth month of the start date
	 * @param startDay   day of the start date
	 * @param endYear    year of the end date
	 * @param endMonth   month of the end date
	 * @param endDay     day of the end date
	 * @deprecated as CustomBudgetingPeriod(SimpleDate start, SimpleDate end)
	 *               should be used instead
	 */
	public CustomBudgetingPeriod(int startYear, int startMonth,
		int startDay, int endYear, int endMonth, int endDay)
	{
		startDate = new SimpleDate(startDay, startMonth, startYear);
		endDate = new SimpleDate(endDay, endMonth, endYear);
	}
	
	/**
	 * Constructs a new custom budgeting period with
	 * the given start and end dates.
	 * 
	 * @param start start date for the period
	 * @param end   end date for the period
	 */
	public CustomBudgetingPeriod(@Element(name="start-date") SimpleDate start,
		@Element(name="end-date") SimpleDate end)
	{
		startDate = start;
		endDate = end;
	}
    
    @Override
    public boolean contains(SimpleDate when)
    {
    	return ! (when.before(startDate) || when.after(endDate));
    }
    
	@Override
	public SimpleDate getStartDate()
	{
		return startDate;
	}
	
	@Override
	public SimpleDate getEndDate()
	{
		return endDate;
	}

	@Override
	public boolean equals(Object obj)
	{
		if (obj instanceof CustomBudgetingPeriod)
		{
			CustomBudgetingPeriod that = (CustomBudgetingPeriod) obj;
			
			return this.startDate.equals(that.startDate)
				&& this.endDate.equals(that.endDate);
		}
		else
			return false;
	}
	
	@Override
	public String toString()
	{
		return startDate + " to " + endDate;
	}

}
