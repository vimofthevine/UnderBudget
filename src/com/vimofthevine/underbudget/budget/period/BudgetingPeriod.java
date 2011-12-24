package com.vimofthevine.underbudget.budget.period;

import java.util.Date;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

/**
 * Interface for budgeting period representations
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface BudgetingPeriod {

	/**
	 * Returns the start date for the
	 * budgeting period
	 * 
	 * @return budgeting period start date
	 */
	public Date getStartDate();
	
	/**
	 * Returns the end date for the
	 * budgeting period
	 * 
	 * @return budgeting period end date
	 */
	public Date getEndDate();
	
	/**
	 * Stores the budgeting period as XML elements
	 * of the given XML element
	 * 
	 * @param doc     DOM document to use
	 * @param element XML element in which to store
	 */
	public void store(Document doc, Element element);
	
	/**
	 * Parses the budgeting period from a given XML element
	 * 
	 * @param element budgeting period XML element
	 * @return the budgeting period defined by the XML,
	 *         if the XML defines a valid budgeting period
	 */
	public BudgetingPeriod parse(Element element);
	
}
