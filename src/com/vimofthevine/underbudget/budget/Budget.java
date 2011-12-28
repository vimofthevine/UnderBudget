package com.vimofthevine.underbudget.budget;

import com.vimofthevine.underbudget.estimates.Estimate;

/**
 * Representation of a user-defined budget
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class Budget {
	
	/**
	 * Budget meta information
	 */
	public BudgetMeta meta;
	
	/**
	 * Root income estimate
	 */
	public Estimate incomes;
	
	/**
	 * Root expense estimate
	 */
	public Estimate expenses;
	
	/**
	 * Parameterized constructor
	 * 
	 * @param meta     budget meta info
	 * @param incomes  income estimate tree
	 * @param expenses expense estimate tree
	 */
	public Budget(BudgetMeta meta, Estimate incomes, Estimate expenses)
	{
		this.meta     = meta;
		this.incomes  = incomes;
		this.expenses = expenses;
	}

}
