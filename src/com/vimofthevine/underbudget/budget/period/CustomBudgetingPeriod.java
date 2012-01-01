package com.vimofthevine.underbudget.budget.period;

import java.util.Calendar;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

import com.vimofthevine.underbudget.util.XmlHelper;

/**
 * Budgeting period based on user-specified start
 * and end dates
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CustomBudgetingPeriod implements BudgetingPeriod {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(CustomBudgetingPeriod.class.getName());

	/**
	 * Budgeting period type identifier
	 */
	public static final String TYPE = "custom";
	
	/**
	 * Start date year
	 */
	public int startYear;
	
	/**
	 * Start date month
	 */
	public int startMonth;
	
	/**
	 * Start date day
	 */
	public int startDay;
	
	/**
	 * End date year
	 */
	public int endYear;
	
	/**
	 * End date month
	 */
	public int endMonth;
	
	/**
	 * End date day
	 */
	public int endDay;
	
	/**
	 * Default constructor
	 */
	public CustomBudgetingPeriod()
	{
		// Initialize period to current month
		Calendar cal = Calendar.getInstance();
		
		// Set the start/end year
		startYear = cal.get(Calendar.YEAR);
		endYear   = cal.get(Calendar.YEAR);
		
		// Set the start/end month
		startMonth = cal.get(Calendar.MONTH);
		endMonth   = cal.get(Calendar.MONTH);
		
		// Set the start day (1st)
		startDay = 1;
		
		// Calculate the end day (last day of the month)
		cal.add(Calendar.MONTH, 1);
		cal.add(Calendar.DAY_OF_MONTH, -1);
		endDay = cal.get(Calendar.DAY_OF_MONTH);
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
	 */
	public CustomBudgetingPeriod(int startYear, int startMonth,
		int startDay, int endYear, int endMonth, int endDay)
	{
		this.startYear  = startYear;
		this.startMonth = startMonth;
		this.startDay   = startDay;
		
		this.endYear    = endYear;
		this.endMonth   = endMonth;
		this.endDay     = endDay;
	}
	
	/**
	 * Returns the start date, defined by the start
	 * year/month/date
	 * 
	 * @return specified start date
	 */
	@Override
    public Date getStartDate()
    {
		Calendar cal = Calendar.getInstance();
		cal.set(startYear, startMonth, startDay);
		return cal.getTime();
    }

	/**
	 * Returns the end date, defined by the end
	 * year/month/date
	 * 
	 * @return specified end date
	 */
	@Override
    public Date getEndDate()
    {
		Calendar cal = Calendar.getInstance();
		cal.set(endYear, endMonth, endDay);
		return cal.getTime();
    }

	/**
	 * Stores the budgeting period as start and end date
	 * elements under the given parent element
	 */
	@Override
    public void store(Document doc, Element element)
	{
		// Create start and end elements
		Element startElement = doc.createElement("start-date");
		Element endElement   = doc.createElement("end-date");
		
		element.appendChild(startElement);
		element.appendChild(endElement);
		
		// Create start date elements
		Element startYearElement  = doc.createElement("year");
		Element startMonthElement = doc.createElement("month");
		Element startDayElement   = doc.createElement("day");
		
		startYearElement.appendChild(doc.createTextNode(String.valueOf(startYear)));
		startMonthElement.appendChild(doc.createTextNode(String.valueOf(startMonth)));
		startDayElement.appendChild(doc.createTextNode(String.valueOf(startDay)));
		
		startElement.appendChild(startYearElement);
		startElement.appendChild(startMonthElement);
		startElement.appendChild(startDayElement);
		
		// Create end date elements
		Element endYearElement  = doc.createElement("year");
		Element endMonthElement = doc.createElement("month");
		Element endDayElement   = doc.createElement("day");
		
		endYearElement.appendChild(doc.createTextNode(String.valueOf(endYear)));
		endMonthElement.appendChild(doc.createTextNode(String.valueOf(endMonth)));
		endDayElement.appendChild(doc.createTextNode(String.valueOf(endDay)));
		
		endElement.appendChild(endYearElement);
		endElement.appendChild(endMonthElement);
		endElement.appendChild(endDayElement);
		
		// Set custom period type
		element.setAttribute("type", TYPE);
    }

	/**
	 * Parses the custom budgeting period defined by
	 * start and end date elements
	 * 
	 * @return CustomBudgetingPeriod if a valid custom
	 *         period is defined in the XML
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
				Element startElement = (Element) element.getElementsByTagName("start-date").item(0);
				Element endElement   = (Element) element.getElementsByTagName("end-date").item(0);
				
				int startDay   = Integer.parseInt(XmlHelper.readTextNode(startElement, "day"));
				int startMonth = Integer.parseInt(XmlHelper.readTextNode(startElement, "month"));
				int startYear  = Integer.parseInt(XmlHelper.readTextNode(startElement, "year"));
				
				int endDay   = Integer.parseInt(XmlHelper.readTextNode(endElement, "day"));
				int endMonth = Integer.parseInt(XmlHelper.readTextNode(endElement, "month"));
				int endYear  = Integer.parseInt(XmlHelper.readTextNode(endElement, "year"));
				
				return new CustomBudgetingPeriod(startYear, startMonth, startDay,
					endYear, endMonth, endDay);
			}
			catch (Exception e)
			{
				logger.log(Level.WARNING, "Error parsing custom budgeting period", e);
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
		if (obj instanceof CustomBudgetingPeriod)
		{
			CustomBudgetingPeriod tmp = (CustomBudgetingPeriod) obj;
			
			return (tmp.startYear == startYear)
			       && (tmp.startMonth == startMonth)
			       && (tmp.startDay == startDay)
			       && (tmp.endYear == endYear)
			       && (tmp.endMonth == endMonth)
			       && (tmp.endDay == endDay);
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
		return startDay + "/" + startMonth + "/" + startYear
			+ " to "
			+ endDay + "/" + endMonth + "/" + endYear;
	}

}
