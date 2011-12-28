package com.vimofthevine.underbudget.budget.file.parsers;

/**
 * Factory class for creating budget file parsers
 * and writers
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class BudgetFileParserFactory {
	
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
