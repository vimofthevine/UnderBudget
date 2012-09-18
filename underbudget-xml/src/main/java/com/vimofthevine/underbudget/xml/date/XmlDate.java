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

package com.vimofthevine.underbudget.xml.date;

import java.util.Calendar;
import java.util.Date;

import org.simpleframework.xml.Element;

import com.vimofthevine.underbudget.core.date.DateTime;
import com.vimofthevine.underbudget.core.date.SimpleDate;

/**
 * A <code>SimpleDate</code> implementation using
 * Simple XML annotations for serialization.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class XmlDate implements SimpleDate {
	
	/**
	 * Original date wrapped by this object
	 */
	private final SimpleDate original;
	
	/**
	 * Day of the month, starting at 1
	 */
	@Element
	private final int day;
	
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
	 * Constructs a new XML serializable
	 * date wrapper around the given date.
	 * 
	 * @param date original date
	 */
	public XmlDate(SimpleDate date)
	{
		original = date;
		
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(original.getTime());
		
		day = calendar.get(Calendar.DAY_OF_MONTH);
		month = calendar.get(Calendar.MONTH);
		year = calendar.get(Calendar.YEAR);
	}
	
	/**
	 * Constructs a new XML serializable date
	 * for the given date, month, and year.
	 * 
	 * This constructor is only used when deserializing.
	 * 
	 * @param day   day of the month
	 * @param month month of the year
	 * @param year  year
	 */
	XmlDate(
		@Element(name="day") int day,
		@Element(name="month") int month,
		@Element(name="year") int year)
	{
		Calendar calendar = Calendar.getInstance();
		calendar.set(year, month, day);
		
		this.day = day;
		this.month = month;
		this.year = year;
		
		original = new DateTime(calendar.getTime());
	}

	@Override
	public int compareTo(SimpleDate that)
	{
		return original.compareTo(that);
	}

	@Override
	public boolean before(SimpleDate that)
	{
		return original.before(that);
	}

	@Override
	public boolean after(SimpleDate that)
	{
		return original.after(that);
	}

	@Override
	public String formatAsString()
	{
		return original.formatAsString();
	}

	@Override
	public Date getTime()
	{
		return original.getTime();
	}
	
	@Override
	public int hashCode()
	{
		int result = 72;
		result = result * 31 + day;
		result = result * 31 + month;
		result = result * 31 + year;
		return result;
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj == this)
			return true;
		if ( ! (obj instanceof XmlDate))
			return false;
		
		XmlDate that = (XmlDate) obj;
		return (this.day == that.day)
			&& (this.month == that.month)
			&& (this.year == that.year);
	}
	
	@Override
	public String toString()
	{
		return (month+1) + "/" + day + "/" + year;
	}

}
