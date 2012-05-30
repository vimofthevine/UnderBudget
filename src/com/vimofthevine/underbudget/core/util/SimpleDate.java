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

package com.vimofthevine.underbudget.core.util;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import org.simpleframework.xml.Element;

/**
 * Simple date wrapper
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class SimpleDate implements Comparable<SimpleDate> {

	/**
	 * Day of the month, starting at 1. This field
	 * should be considered final.
	 */
	@Element(name="day")
	private int day;
	
	/**
	 * Month of the year, starting at 0. This field
	 * should be considered final.
	 */
	@Element(name="month")
	private int month;
	
	/**
	 * Year. This field
	 * should be considered final.
	 */
	@Element(name="year")
	private int year;
	
	/**
	 * Constructs a new date, initialized to the
	 * current date
	 */
	public SimpleDate()
	{
		this(new Date());
	}
	
	/**
	 * Constructs a new date with the given day
	 * of the current month.
	 * 
	 * @param day day of the month
	 */
	public SimpleDate(int day)
	{
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(new Date());
		
		this.day = day;
		month = calendar.get(Calendar.MONTH);
		year = calendar.get(Calendar.YEAR);
	}
	
	/**
	 * Constructs a new date with the given parameters
	 * 
	 * @param day   day of the month
	 * @param month month of the year, starting at 0
	 * @param year  year
	 */
	public SimpleDate(int day, int month, int year)
	{
		this.day = day;
		this.month = month;
		this.year = year;
	}
	
	/**
	 * Constructs a new date given a java.util.Date
	 * 
	 * @param date java.util.Date object to be
	 *              converted to a SimpleDate object
	 */
	public SimpleDate(Date date)
	{
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		
		day = calendar.get(Calendar.DAY_OF_MONTH);
		month = calendar.get(Calendar.MONTH);
		year = calendar.get(Calendar.YEAR);
	}
	
	/**
	 * Converts the simple date to a java.util.Date
	 * 
	 * @return Date representing the simple date
	 */
	private Date getTime()
	{
		Calendar calendar = Calendar.getInstance();
		calendar.clear();
		calendar.set(year, month, day);
		return calendar.getTime();
	}
	
	/**
	 * Checks if this date occurs before
	 * the specified date
	 * 
	 * @param when date to compare
	 */
	public boolean before(SimpleDate when)
	{
		return getTime().before(when.getTime());
	}
	
	/**
	 * Checks if the given date occurs
	 * after this date
	 * 
	 * @param when date to compare
	 */
	public boolean after(SimpleDate when)
	{
		return getTime().after(when.getTime());
	}
	
	@Override
	public int compareTo(SimpleDate that)
	{
		return this.getTime().compareTo(that.getTime());
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (this == obj)
			return true;
		if ( ! (obj instanceof SimpleDate))
			return false;
		
		SimpleDate that = (SimpleDate) obj;
		
		return (this.getTime().equals(that.getTime()));
	}
	
	@Override
	public int hashCode()
	{
		int result = 20;
		result = 31 * result + day;
		result = 31 * result + month;
		result = 31 * result + year;
		return result;
	}
	
	/**
	 * Returns the date formatted as a human-readable string
	 * in the format of MM/dd/YY
	 * 
	 * @return date as a string in the format, MM/dd/yy
	 */
	public String formatAsString()
	{
		SimpleDateFormat format = new SimpleDateFormat("MM/dd/yy");
		return format.format(getTime());
	}
	
	
	@Override
	public String toString()
	{
		return (month + 1) + "/" + day + "/" + year;
	}
	
}
