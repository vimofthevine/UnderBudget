package com.vimofthevine.underbudget.budget.file.parsers.meta;

import java.util.logging.Level;
import java.util.logging.Logger;

import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import com.vimofthevine.underbudget.budget.file.BudgetFileException;
import com.vimofthevine.underbudget.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.budget.period.MonthlyBudgetingPeriod;
import com.vimofthevine.underbudget.budget.period.YearlyBudgetingPeriod;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Budget meta parser that adds support for multiple
 * budgeting period types, introduced in file version 3
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetMetaDomParserV3 extends BudgetMetaDomParserV1 {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(BudgetMetaDomParserV3.class.getName());
	
	/**
	 * List of known budgeting period types
	 */
	BudgetingPeriod[] types;
	
	/**
	 * Constructor, given the task progress
	 * 
	 * @param progress task progress
	 */
	public BudgetMetaDomParserV3(TaskProgress progress)
	{
		super(progress);
		
		types = new BudgetingPeriod[] {
			new MonthlyBudgetingPeriod(),
			new YearlyBudgetingPeriod(),
		};
	}
	
	@Override
	protected BudgetingPeriod parseBudgetingPeriod(Element element)
	throws BudgetFileException
	{
		try
		{
			// Get period element
			NodeList periodList = element.getElementsByTagName("period");
			
			if (periodList.getLength() < 1)
				return new MonthlyBudgetingPeriod();
			
			Element periodElement = (Element) periodList.item(0);
			
			for (BudgetingPeriod type : types)
			{
				BudgetingPeriod parsed = type.parse(periodElement);
				if (parsed != null)
					return parsed;
			}
			
			logger.log(Level.WARNING, "Unknown budgeting period type: " + periodElement);
			return new MonthlyBudgetingPeriod();
		}
		catch (Exception e)
		{
			logger.log(Level.WARNING, "Exception parsing budgeting period", e);
			throw new BudgetFileException("Invalid budgeting period in file");
		}
	}
	
}
