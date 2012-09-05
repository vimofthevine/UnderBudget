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

import com.vimofthevine.underbudget.core.date.DateTime;
import com.vimofthevine.underbudget.core.date.SimpleDate;

/**
 * A budgeting period using custom start and end
 * dates.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class DefaultCustomPeriod implements CustomPeriod {
	
	/**
	 * Start date
	 */
	private final SimpleDate startDate;
	
	/**
	 * End date
	 */
	private final SimpleDate endDate;
	
	/**
	 * Constructs a period, with the first
	 * and last days of the current month
	 * as the start and end dates.
	 */
	public DefaultCustomPeriod()
	{
		Calendar calendar = Calendar.getInstance();
		calendar.set(Calendar.DAY_OF_MONTH, 1);
		startDate = new DateTime(calendar.getTime());
		
		calendar.add(Calendar.MONTH, 1);
		calendar.add(Calendar.DAY_OF_MONTH, -1);
		endDate = new DateTime(calendar.getTime());
	}
	
	/**
	 * Constructs a period with the given start
	 * and end dates.
	 * 
	 * @param start start date
	 * @param end   end date
	 */
	public DefaultCustomPeriod(SimpleDate start, SimpleDate end)
	{
		startDate = start;
		endDate = end;
	}
	
	@Override
	public final PeriodType getType()
	{
		return PeriodType.CUSTOM;
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
	public boolean contains(SimpleDate date)
	{
		final boolean before = date.before(startDate);
		final boolean after = date.after(endDate);
		return ! (before || after);
	}

	@Override
	public String getDescription()
	{
		return startDate.formatAsString() + " - " +
			endDate.formatAsString();
	}
	
	@Override
	public int hashCode()
	{
		int result = 73;
		result = result * 31 + startDate.hashCode();
		result = result * 31 + endDate.hashCode();
		return result;
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj == this)
			return true;
		if ( ! (obj instanceof DefaultCustomPeriod))
			return false;
		
		DefaultCustomPeriod that = (DefaultCustomPeriod) obj;
		return this.startDate.equals(that.startDate)
			&& this.endDate.equals(that.endDate);
	}
	
	@Override
	public String toString()
	{
		return startDate + " to " + endDate;
	}

}
