package com.vimofthevine.underbudget.analysis;

import java.util.List;

import com.vimofthevine.underbudget.budget.Budget;
import com.vimofthevine.underbudget.report.AllocationEntry;
import com.vimofthevine.underbudget.report.BalanceTotal;
import com.vimofthevine.underbudget.report.WorksheetEntry;

/**
 * Representation of all analysis artifacts
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AnalysisResults {
	
	/**
	 * Budget that has been analyzed
	 */
	public Budget budget;
	
	/**
	 * List of sorted estimate rules
	 */
	public List<EstimateRule> rules;
	
	/**
	 * List of allocation entries
	 */
	public List<AllocationEntry> allocation;
	
	/**
	 * List of worksheet entries
	 */
	public List<WorksheetEntry> worksheet;
	
	/**
	 * Balance totals
	 */
	public BalanceTotal total;

}
