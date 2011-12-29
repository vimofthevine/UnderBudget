package com.vimofthevine.underbudget.cli.writer;

import java.io.PrintStream;

import com.vimofthevine.underbudget.analysis.AnalysisResults;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.report.BalanceTotal;
import com.vimofthevine.underbudget.report.WorksheetEntry;
import com.vimofthevine.underbudget.util.FormatHelper;

/**
 * Writes the worksheet report to the given output stream
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class WorksheetReportWriter {

	/**
	 * Analysis results
	 */
	protected AnalysisResults results;
	
	/**
	 * Verbose reporting
	 */
	protected boolean verbose;
	
	/**
	 * Constructor
	 * 
	 * @param results analysis results
	 * @param verbose enable verbose reporting
	 */
	public WorksheetReportWriter(AnalysisResults results, boolean verbose)
	{
		this.results = results;
		this.verbose = verbose;
	}
	
	/**
	 * Writes the worksheet report to the output stream
	 * 
	 * @param stream print stream on which to write
	 */
	public void write(PrintStream stream)
	{
		stream.println("Worksheet Report");
		stream.println("--------------------------------------------------------------------------------");
		stream.println("Estimate            Estimated   Actual      Est. Total  Act. Total  Exp. Total  ");
		stream.println("--------------------------------------------------------------------------------");
		
		for (WorksheetEntry entry : results.worksheet)
		{
			stream.println(createLine1(entry));
			
			if (verbose)
			{
				stream.println(createLine2(entry));
			}
		}
		
		stream.println("--------------------------------------------------------------------------------");
		stream.println();
	}
	
	/**
	 * Creates the line 1 of the worksheet entry
	 * 
	 * @param entry worksheet entry
	 * @return line 1 string
	 */
	protected String createLine1(WorksheetEntry entry)
	{
		StringBuffer buffer = new StringBuffer();
		
		Estimate estimate = entry.estimate;
		BalanceTotal total = entry.totals;
		
		// Add estimate name (19 chars)
		buffer.append(FormatHelper.truncate(estimate.getName(), 19));
		
		// Separator (1 char)
		buffer.append(" ");
		
		// Add estimated amount (11 chars)
		String estAmt = FormatHelper.formatCurrency(estimate.getAmount());
		buffer.append(FormatHelper.truncate(estAmt, 11));
		
		// Separator (1 char)
		buffer.append(" ");
		
		// Add actual amount (11 chars)
		String actAmt = FormatHelper.formatCurrency(estimate.getActualAmount());
		buffer.append(FormatHelper.truncate(actAmt, 11));
		
		// Separator (1 char)
		buffer.append(" ");
		
		// Add estimated total (11 chars)
		String estTot = FormatHelper.formatCurrency(total.getEstimatedNetChange());
		buffer.append(FormatHelper.truncate(estTot, 11));
		
		// Separator (1 char)
		buffer.append(" ");
		
		// Add actual total (11 chars)
		String actTot = FormatHelper.formatCurrency(total.getActualNetChange());
		buffer.append(FormatHelper.truncate(actTot, 11));
		
		// Separator (1 char)
		buffer.append(" ");
		
		// Add expected total (12 chars)
		String expTot = FormatHelper.formatCurrency(total.getExpectedNetChange());
		buffer.append(FormatHelper.truncate(expTot, 12));
		
		return buffer.toString();
	}
	
	/**
	 * Creates line 2 of the worksheet entry
	 * 
	 * @param entry worksheet entry
	 * @return line 2 string
	 */
	protected String createLine2(WorksheetEntry entry)
	{
		StringBuffer buffer = new StringBuffer();
		
		buffer.append("  ");
		buffer.append(FormatHelper.truncate(entry.rationele, 78));
		
		return buffer.toString();
	}
	
}
