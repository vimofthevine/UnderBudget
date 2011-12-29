package com.vimofthevine.underbudget.analysis;

import java.util.List;

import com.vimofthevine.underbudget.report.AllocationEntry;

/**
 * Representation of all analysis artifacts
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AnalysisResults {
	
	/**
	 * List of sorted estimate rules
	 */
	public List<EstimateRule> rules;
	
	/**
	 * List of allocation entries
	 */
	public List<AllocationEntry> allocation;

}
