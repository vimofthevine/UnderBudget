package com.vimofthevine.underbudget.report;

import java.math.BigDecimal;

/**
 * Representation of calculated balance totals
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BalanceTotal {
	
	/**
	 * Estimated income total
	 */
	public BigDecimal estimatedIncome = new BigDecimal("0");
	
	/**
	 * Estimated expense total
	 */
	public BigDecimal estimatedExpense = new BigDecimal("0");
	
	/**
	 * Actual income total
	 */
	public BigDecimal actualIncome = new BigDecimal("0");
	
	/**
	 * Actual expense total
	 */
	public BigDecimal actualExpense = new BigDecimal("0");
	
	/**
	 * Expected income total
	 */
	public BigDecimal expectedIncome = new BigDecimal("0");
	
	/**
	 * Expected expense total
	 */
	public BigDecimal expectedExpense = new BigDecimal("0");
	
	/**
	 * Returns the calculated estimated net change that is the estimated
	 * income total less the estimated expense total. Estimated income and
	 * expense totals are based on the user-defined estimate values.
	 *
	 * @return net change based on estimated income and estimated expense
	 */
	public BigDecimal getEstimatedNetChange()
	{
		return estimatedIncome.subtract(estimatedExpense);
	}

	/**
	 * Returns the calculated actual net change that is the actual income
	 * total less the actual expense total. Actual income and expense totals
	 * are based on the imported transaction data and may represent
	 * real-time values.
	 *
	 * @return net change based on actual income and actual expense
	 */
	public BigDecimal getActualNetChange()
	{
		return actualIncome.subtract(actualExpense);
	}

	/**
	 * Returns the calculated expected net change that is the expected
	 * income total less the expected expense total. Expected income and
	 * expense totals are based on the effective to-date values from each
	 * comparison. That is, over-budget and final estimates use the actual
	 * values while under-budget estimates use the estimated values.
	 *
	 * @return net change based on expected income and expected expense
	 */
	public BigDecimal getExpectedNetChange()
	{
		return expectedIncome.subtract(expectedExpense);
	}
	
	/**
	 * Perform a deep copy of this BalanceTotal object
	 * 
	 * @return cloned BalanceTotal
	 */
	public BalanceTotal clone()
	{
		BalanceTotal clone = new BalanceTotal();
		
		clone.estimatedIncome  = new BigDecimal(estimatedIncome.toBigInteger());
		clone.estimatedExpense = new BigDecimal(estimatedExpense.toBigInteger());
		clone.actualIncome     = new BigDecimal(actualIncome.toBigInteger());
		clone.actualExpense    = new BigDecimal(actualExpense.toBigInteger());
		clone.expectedIncome   = new BigDecimal(expectedIncome.toBigInteger());
		clone.expectedExpense  = new BigDecimal(expectedExpense.toBigInteger());
		
		return clone;
	}

}
