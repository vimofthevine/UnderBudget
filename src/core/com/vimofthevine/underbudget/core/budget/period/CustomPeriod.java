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

import com.vimofthevine.underbudget.core.date.SimpleDate;

/**
 * A budgeting period using custom start and end
 * dates.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CustomPeriod implements BudgetingPeriod {
	
	/**
	 * Start date
	 */
	private final SimpleDate startDate;
	
	/**
	 * End date
	 */
	private final SimpleDate endDate;
	
	/**
	 * Constructs a period with the given start
	 * and end dates.
	 * 
	 * @param start start date
	 * @param end   end date
	 */
	public CustomPeriod(SimpleDate start, SimpleDate end)
	{
		startDate = start;
		endDate = end;
	}
	
	/**
	 * Returns the start date of this period.
	 * 
	 * @return start date
	 */
	public SimpleDate getStartDate()
	{
		return startDate;
	}
	
	/**
	 * Returns the end date of this period.
	 * 
	 * @return end date
	 */
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
		if ( ! (obj instanceof CustomPeriod))
			return false;
		
		CustomPeriod that = (CustomPeriod) obj;
		return this.startDate.equals(that.startDate)
			&& this.endDate.equals(that.endDate);
	}
	
	@Override
	public String toString()
	{
		return startDate + " to " + endDate;
	}

}
