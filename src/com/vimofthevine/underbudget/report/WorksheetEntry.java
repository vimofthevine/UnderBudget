package com.vimofthevine.underbudget.report;

import com.vimofthevine.underbudget.estimates.Estimate;

/**
 * An entry for the worksheet report, indicating the
 * effect of each estimate on the balance totals
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class WorksheetEntry {
	
	/**
	 * The estimate for this entry
	 */
	public Estimate estimate;
	
	/**
	 * The balance totals for this entry
	 */
	public BalanceTotal totals;
	
	/**
	 * Constructor
	 * 
	 * @param estimate entry estimate
	 * @param totals   entry totals
	 */
	public WorksheetEntry(Estimate estimate, BalanceTotal totals)
	{
		this.estimate = estimate;
		this.totals   = totals.clone();
	}

}
