package com.vimofthevine.underbudget.budget.file;

/**
 * Exception thrown when parsing or writing
 * budget files
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetFileException extends Exception {
	
	/**
	 * Class constructor, specifying the exception text
	 * 
	 * @param msg exception detail message
	 */
	public BudgetFileException(String msg)
	{
		super(msg);
	}

}
