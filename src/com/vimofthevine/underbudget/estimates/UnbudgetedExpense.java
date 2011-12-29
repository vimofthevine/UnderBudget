package com.vimofthevine.underbudget.estimates;

/**
 * Automatic estimate used to capture unbudgeted expense
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class UnbudgetedExpense extends ExpenseEstimate {

	/**
	 * Default constructor
	 */
	public UnbudgetedExpense()
	{
		super();
		
		name = "Unbudgeted Expense";
	}
	
}
