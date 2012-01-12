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
 * Budgeting period based on a calendar month, starting
 * on the first Friday of the month and ending on the
 * last Thursday (before the next month's first Friday)
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class MonthlyBudgetingPeriod implements BudgetingPeriod {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(MonthlyBudgetingPeriod.class.getName());
	
	/**
	 * Budgeting period type identifier
	 */
	public static final String TYPE = "monthly";

	/**
	 * The month of the budgeting period
	 */
	public int month;
	
	/**
	 * The year of the budgeting period
	 */
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
	 * Returns the start date, which is the first
	 * Friday of the specified month
	 * 
	 * @return date of the first Friday of the specified month
	 */
	@Override
    public Date getStartDate()
    {
		// Set calendar to selected month/year
		Calendar cal = Calendar.getInstance();
		cal.clear();
		cal.set(year, month, 15);
		
		// Get first Friday of specified month
		cal.set(Calendar.DAY_OF_WEEK, Calendar.FRIDAY);
		cal.set(Calendar.DAY_OF_WEEK_IN_MONTH, 1);
		
		return cal.getTime();
    }

	/**
	 * Returns the end date, which is the Thursday
	 * before the first Friday of the month after
	 * the specified month
	 * 
	 * @return date of the Thursday before the next month
	 */
	@Override
    public Date getEndDate()
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
		
		return cal.getTime();
    }

	/**
	 * Stores the budget period as month and year elements
	 * under the given parent element
	 */
	@Override
    public void store(Document doc, Element element)
    {
		// Create month and year elements
		Element monthElement = doc.createElement("month");
		Element yearElement  = doc.createElement("year");
		
		monthElement.appendChild(doc.createTextNode(String.valueOf(month)));
		yearElement.appendChild(doc.createTextNode(String.valueOf(year)));
		
		element.appendChild(monthElement);
		element.appendChild(yearElement);
		
		// Set monthly period type
		element.setAttribute("type", TYPE);
    }

	/**
	 * Parses the monthly budgeting period defined by
	 * month and year elements
	 * 
	 * @return MonthlyBudgetingPeriod if a valid monthly period
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
				int month = Integer.parseInt(XmlHelper.readTextNode(element, "month"));
				int year  = Integer.parseInt(XmlHelper.readTextNode(element, "year"));
				
				return new MonthlyBudgetingPeriod(month, year);
			}
			catch (Exception e)
			{
				logger.log(Level.WARNING, "Error parsing monthly budgeting period", e);
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
		if (obj instanceof MonthlyBudgetingPeriod)
		{
			MonthlyBudgetingPeriod tmp = (MonthlyBudgetingPeriod) obj;
			
			return (tmp.month == month) && (tmp.year == year);
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
