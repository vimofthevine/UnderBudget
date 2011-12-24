package com.vimofthevine.underbudget.budget.file;

import com.vimofthevine.underbudget.budget.file.parsers.BudgetFileDomParser;
import com.vimofthevine.underbudget.budget.file.parsers.BudgetFileParser;

/**
 * Factory class for creating budget file parsers
 * and writers
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class BudgetFileFactory {
	
	/**
	 * Create a new budget file parser
	 * 
	 * @return BudgetFileParser implementation
	 */
	public static BudgetFileParser createParser()
	{
		return new BudgetFileDomParser();
	}

}
