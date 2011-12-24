package com.vimofthevine.underbudget.budget.file.parsers.estimate;

import org.w3c.dom.Document;

import com.vimofthevine.underbudget.budget.file.BudgetFileException;
import com.vimofthevine.underbudget.estimates.Estimate;

/**
 * Interface for estimate DOM parsers
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface EstimateDomParser {
	
	/**
	 * Extracts the income estimate definitions defined
	 * in the given DOM document
	 * 
	 * @param doc       parsed DOM document
	 * @param allocated percentage of the overall task allocated
	 * @return income estimate root as defined in the document
	 * @throws BudgetFileException if any error occurs
	 */
	public Estimate parseIncomes(Document doc, float allocated)
	throws BudgetFileException;
	
	/**
	 * Extracts the expense estimate definitions defined
	 * in the given DOM document
	 * 
	 * @param doc       parsed DOM document
	 * @param allocated percentage of the overall task allocated
	 * @return expense estimate root as defined in the document
	 * @throws BudgetFileException if any error occurs
	 */
	public Estimate parseExpenses(Document doc, float allocated)
	throws BudgetFileException;

}
