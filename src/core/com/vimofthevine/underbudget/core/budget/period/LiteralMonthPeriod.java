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
import java.util.Date;

import com.vimofthevine.underbudget.core.date.DateTime;
import com.vimofthevine.underbudget.core.date.SimpleDate;

/**
 * A budgeting period based on a literal calendar month.
 * Defined by the year and the month, the start date is
 * the first day of the specified month and the end date
 * is the last day of the specified month.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class LiteralMonthPeriod implements MonthlyBudgetingPeriod {
	
	/**
	 * Month of this period
	 */
	private final Month month;
	
	/**
	 * Year of this period
	 */
	private final int year;
	
	/**
	 * Constructs a period representing the
	 * current literal month.
	 */
	public LiteralMonthPeriod()
	{
		// Initialize period to current month/year
		Calendar calendar = Calendar.getInstance();
		month = Month.valueOf(calendar.get(Calendar.MONTH));
		year = calendar.get(Calendar.YEAR);
	}
	
	/**
	 * Constructs a period representing the
	 * given month.
	 * 
	 * @param month month of the year
	 * @param year  year in which the month occurs
	 */
	public LiteralMonthPeriod(Month month, int year)
	{
		this.month = month;
		this.year = year;
	}
	
	@Override
	public Month getMonth()
	{
		return month;
	}
	
	@Override
	public int getYear()
	{
		return year;
	}
	
	@Override
	public boolean contains(SimpleDate date)
	{
		Calendar calendar = Calendar.getInstance();
		calendar.clear();
		calendar.set(year, month.ordinal(), 1);
		
		final Date start = calendar.getTime();
		
		calendar.add(Calendar.MONTH, 1);
		calendar.add(Calendar.DAY_OF_MONTH, -1);
		final Date end = calendar.getTime();
		
		final boolean before = date.getTime().before(start);
		final boolean after = date.getTime().after(end);
		
		return ! (before || after);
	}

	@Override
	public String getDescription()
	{
		Calendar calendar = Calendar.getInstance();
		calendar.clear();
		calendar.set(year, month.ordinal(), 1);
		
		final DateTime start = new DateTime(calendar.getTime());
		
		calendar.add(Calendar.MONTH, 1);
		calendar.add(Calendar.DAY_OF_MONTH, -1);
		final DateTime end = new DateTime(calendar.getTime());
		
		return start.formatAsString() + " - " +
			end.formatAsString();
	}
	
	@Override
	public int hashCode()
	{
		int result = 72;
		result = result * 31 + month.hashCode();
		result = result * 31 + year;
		return result;
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj == this)
			return true;
		if ( ! (obj instanceof LiteralMonthPeriod))
			return false;
		
		LiteralMonthPeriod that = (LiteralMonthPeriod) obj;
		return this.month.equals(that.month)
			&& (this.year == that.year);
	}
	
	@Override
	public String toString()
	{
		return month + " " + year;
	}

}
