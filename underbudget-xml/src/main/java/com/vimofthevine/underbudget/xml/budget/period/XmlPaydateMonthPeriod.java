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

import com.vimofthevine.underbudget.core.budget.period.Month;
import com.vimofthevine.underbudget.core.budget.period.MonthlyBudgetingPeriod;
import com.vimofthevine.underbudget.core.budget.period.PaydateMonthPeriod;
import com.vimofthevine.underbudget.core.budget.period.PeriodType;
import com.vimofthevine.underbudget.core.date.SimpleDate;

/**
 * A <code>BudgetingPeriod</code> implementation using
 * Simple XML annotations for serialization.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class XmlPaydateMonthPeriod implements XmlBudgetingPeriod,
MonthlyBudgetingPeriod {
	
	/**
	 * Custom period type definer
	 */
	@Attribute(name="type")
	static final String TYPE = "paydate-month";
	
	/**
	 * Original paydate month period wrapped by this object
	 */
	private final PaydateMonthPeriod original;
	
	/**
	 * Month of the year, starting at 0
	 */
	@Element
	private final int month;
	
	/**
	 * Year
	 */
	@Element
	private final int year;
	
	/**
	 * Constructs a new XML serializable paydate
	 * month period wrapper around the given
	 * paydate month period.
	 * 
	 * @param period original paydate month period
	 */
	public XmlPaydateMonthPeriod(PaydateMonthPeriod period)
	{
		original = period;
		
		month = period.getMonth().ordinal();
		year = period.getYear();
	}
	
	/**
	 * Constructs a new XML serializable paydate
	 * month period for the given month and year.
	 * 
	 * This constructor is only used when deserializing.
	 * 
	 * @param month month of the year
	 * @param year  year
	 */
	XmlPaydateMonthPeriod(
		@Element(name="month") int month,
		@Element(name="year") int year)
	{
		original = new PaydateMonthPeriod(Month.valueOf(month), year);
		this.month = month;
		this.year = year;
	}
	
	@Override
	public Month getMonth()
	{
		return Month.valueOf(month);
	}
	
	@Override
	public int getYear()
	{
		return year;
	}
	
	@Override
	public final PeriodType getType()
	{
		return PeriodType.PAYDATE_MONTH;
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
