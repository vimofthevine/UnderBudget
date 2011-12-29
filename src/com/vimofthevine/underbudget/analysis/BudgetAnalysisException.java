package com.vimofthevine.underbudget.analysis;

/**
 * Exception thrown during budget analysis
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetAnalysisException extends Exception {

	/**
	 * Class constructor, specifying the exception text
	 * 
	 * @param msg exception detail message
	 */
	public BudgetAnalysisException(String msg)
	{
		super(msg);
	}
	
}
