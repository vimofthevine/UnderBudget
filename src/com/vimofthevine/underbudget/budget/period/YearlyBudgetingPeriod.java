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

package com.vimofthevine.underbudget.budget.period;

import java.util.Calendar;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

import com.vimofthevine.underbudget.util.XmlHelper;

/**
 * Budgeting period based on a calendary year, starting
 * on the first Friday of the year and ending on the
 * last Thursday (before the next year's first Friday)
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class YearlyBudgetingPeriod implements BudgetingPeriod {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(YearlyBudgetingPeriod.class.getName());
	
	/**
	 * Budgeting period type identifier
	 */
	public static final String TYPE = "yearly";
	
	/**
	 * The year of the budgeting period
	 */
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
	 * Returns the start date, which is the first
	 * Friday of the specified year
	 * 
	 * @return date of the first Friday of the specified year
	 */
	@Override
	public Date getStartDate()
	{
		// Set calendar to January of the selected year
		Calendar cal = Calendar.getInstance();
		cal.clear();
		cal.set(year, 0, 15);
		
		// Get first Friday of specified month
		cal.set(Calendar.DAY_OF_WEEK, Calendar.FRIDAY);
		cal.set(Calendar.DAY_OF_WEEK_IN_MONTH, 1);
		
		return cal.getTime();
	}
	
	/**
	 * Returns the end date, which is the Thursday
	 * before the first Friday of the year after
	 * the specified year
	 * 
	 * @return date of the Thursday before the next year
	 */
	@Override
	public Date getEndDate()
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
		
		return cal.getTime();
	}
	
	/**
	 * Stores the budgeting period as year element
	 * under the given parent element
	 */
	@Override
	public void store(Document doc, Element element)
	{
		// Create year element
		Element yearElement = doc.createElement("year");
		
		yearElement.appendChild(doc.createTextNode(String.valueOf(year)));
		
		element.appendChild(yearElement);
		
		// Set yearly period type
		element.setAttribute("type", TYPE);
	}
	
	/**
	 * Parses the yearly budgeting period defined by
	 * year element
	 * 
	 * @return YearlyBudgetingPeriod if a valid yearly period
	 *         is defined in the XML
	 */
	@Override
	public BudgetingPeriod parse(Element element)
	{
		if ( ! element.getAttribute("type").equals(TYPE))
			return null;
		else
		{
			try
			{
				int year = Integer.parseInt(XmlHelper.readTextNode(element, "year"));
				
				return new YearlyBudgetingPeriod(year);
			}
			catch (Exception e)
			{
				logger.log(Level.WARNING, "Error parsing yearly budgeting period");
				return null;
			}
		}
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
			YearlyBudgetingPeriod tmp = (YearlyBudgetingPeriod) obj;
			
			return (tmp.year == year);
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
