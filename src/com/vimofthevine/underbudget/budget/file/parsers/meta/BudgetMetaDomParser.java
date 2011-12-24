package com.vimofthevine.underbudget.budget.file.parsers.meta;

import org.w3c.dom.Document;

import com.vimofthevine.underbudget.budget.BudgetMeta;
import com.vimofthevine.underbudget.budget.file.BudgetFileException;

/**
 * Interface for budget meta DOM parsers
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface BudgetMetaDomParser {
	
	/**
	 * Extracts the budget meta info defined in the given
	 * DOM document
	 * 
	 * @param doc       parsed DOM document
	 * @param allocated percentage of the overall task allocated
	 * @return budget meta data defined in the document
	 * @throws BudgetFileException if any error occurs
	 */
	public BudgetMeta parse(Document doc, float allocated)
	throws BudgetFileException;

}
