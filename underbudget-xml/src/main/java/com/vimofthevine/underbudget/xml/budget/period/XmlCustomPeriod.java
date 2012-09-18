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

package com.vimofthevine.underbudget.xml.budget.period;

import org.simpleframework.xml.Attribute;
import org.simpleframework.xml.Element;

import com.vimofthevine.underbudget.core.budget.period.CustomPeriod;
import com.vimofthevine.underbudget.core.budget.period.DefaultCustomPeriod;
import com.vimofthevine.underbudget.core.budget.period.PeriodType;
import com.vimofthevine.underbudget.core.date.SimpleDate;
import com.vimofthevine.underbudget.xml.date.XmlDate;

/**
 * A <code>BudgetingPeriod</code> implementation using
 * Simple XML annotations for serialization.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class XmlCustomPeriod implements XmlBudgetingPeriod,
CustomPeriod {
	
	/**
	 * Custom period type definer
	 */
	@Attribute(name="type")
	static final String TYPE = "custom";
	
	/**
	 * Original custom period wrapped by this object
	 */
	private final CustomPeriod original;
	
	/**
	 * Start date
	 */
	@Element(name="start-date")
	private final XmlDate startDate;
	
	/**
	 * End date
	 */
	@Element(name="end-date")
	private final XmlDate endDate;
	
	/**
	 * Constructs a new XML serializable custom
	 * period wrapper around the given custom period.
	 * 
	 * @param period original custom period
	 */
	public XmlCustomPeriod(CustomPeriod period)
	{
		original = period;
		
		startDate = new XmlDate(period.getStartDate());
		endDate = new XmlDate(period.getEndDate());
	}
	
	/**
	 * Constructs a new XML serializable custom
	 * period for the given start and end dates.
	 * 
	 * This constructor is only used when deserializing.
	 * 
	 * @param start start date
	 * @param end   end date
	 */
	XmlCustomPeriod(
		@Element(name="start-date") XmlDate start,
		@Element(name="end-date") XmlDate end)
	{
		original = new DefaultCustomPeriod(start, end);
		startDate = start;
		endDate = end;
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
	public final PeriodType getType()
	{
		return PeriodType.CUSTOM;
	}

	@Override
    public boolean contains(SimpleDate date)
    {
		return original.contains(date);
    }

	@Override
    public String getDescription()
    {
		return original.getDescription();
    }

}
