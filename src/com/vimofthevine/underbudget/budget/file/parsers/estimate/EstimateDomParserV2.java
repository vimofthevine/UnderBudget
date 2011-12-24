package com.vimofthevine.underbudget.budget.file.parsers.estimate;

import java.util.logging.Logger;

import org.w3c.dom.Document;

import com.vimofthevine.underbudget.budget.file.BudgetFileException;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * DOM parser for extracting estimate definitions
 * from a version 2 budget file (nested estimates)
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateDomParserV2 implements EstimateDomParser {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(EstimateDomParserV2.class.getName());
	
	/**
	 * Task progress
	 */
	protected TaskProgress progress;
	
	/**
	 * Constructor, given the task progress
	 * 
	 * @param progress task progress
	 */
	public EstimateDomParserV2(TaskProgress progress)
	{
		this.progress = progress;
	}

	@Override
    public Estimate parseIncomes(Document doc, float allocated)
            throws BudgetFileException
    {
	    // TODO Auto-generated method stub
	    return null;
    }

	@Override
    public Estimate parseExpenses(Document doc, float allocated)
            throws BudgetFileException
    {
	    // TODO Auto-generated method stub
	    return null;
    }

}
