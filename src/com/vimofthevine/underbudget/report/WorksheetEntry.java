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
	 * The rationale for the use of estimated vs. actual
	 */
	public String rationele;
	
	/**
	 * Constructor
	 * 
	 * @param estimate  entry estimate
	 * @param totals    entry totals
	 * @param rationale effective amount rationale
	 */
	public WorksheetEntry(Estimate estimate, BalanceTotal totals, String rationale)
	{
		this.estimate  = estimate;
		this.totals    = totals.clone();
		this.rationele = rationale;
	}

}
