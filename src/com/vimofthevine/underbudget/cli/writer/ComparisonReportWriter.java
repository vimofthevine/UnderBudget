package com.vimofthevine.underbudget.cli.writer;

import java.io.PrintStream;

import com.vimofthevine.underbudget.analysis.AnalysisResults;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.util.FormatHelper;

/**
 * Writes the comparison report to the given output stream
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ComparisonReportWriter {

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
	public ComparisonReportWriter(AnalysisResults results, boolean verbose)
	{
		this.results = results;
		this.verbose = verbose;
	}
	
	/**
	 * Writes the comparison report to the output stream
	 * 
	 * @param stream print stream on which to write
	 */
	public void write(PrintStream stream)
	{
		stream.println("Comparison Report");
		stream.println("--------------------------------------------------------------------------------");
		stream.println("Estimate                                    Estimated   Actual      Difference  ");
		stream.println("--------------------------------------------------------------------------------");
		
		writeEstimate(stream, results.budget.incomes, 0);
		writeEstimate(stream, results.rules.get(results.rules.size() - 2).estimate, 1);
		writeEstimate(stream, results.budget.expenses, 0);
		writeEstimate(stream, results.rules.get(results.rules.size() - 1).estimate, 1);
		
		stream.println("--------------------------------------------------------------------------------");
		stream.println();
	}
	
	/**
	 * Writes the given estimate to the given output stream
	 * at the given indentation level
	 * 
	 * @param stream   print stream on which to write
	 * @param estimate estimate to be written
	 * @param level    indentation level
	 */
	protected void writeEstimate(PrintStream stream, Estimate estimate, int level)
	{
		StringBuffer buffer = new StringBuffer();
		
		// Add indentation
		for (int i=0; i<level; i++)
		{
			buffer.append("  ");
		}
		
		int indent = 2 * level;
		
		// Add estimate name
		if (estimate.getName().length() > (44 - indent))
		{
			buffer.append(estimate.getName().substring(0, (44 - indent)));
		}
		else
		{
			buffer.append(estimate.getName());
		}
		
		// Add filler
		for (int i=0; i<(44 - indent - estimate.getName().length()); i++)
		{
			buffer.append(" ");
		}
		
		// Add estimated amount
		String estimated = FormatHelper.formatCurrency(estimate.getAmount());
		buffer.append(estimated);
		
		// Add filler
		for (int i=0; i<(12 - estimated.length()); i++)
		{
			buffer.append(" ");
		}
		
		// Add actual amount
		String actual = FormatHelper.formatCurrency(estimate.getActualAmount());
		buffer.append(actual);
		
		// Add filler
		for (int i=0; i<(12 - actual.length()); i++)
		{
			buffer.append(" ");
		}
		
		// Add difference amount
		String difference = FormatHelper.formatCurrency(estimate.getDifferenceAmount());
		buffer.append(difference);
		
		stream.println(buffer.toString());
		
		// Print out estimate sub-tree
		for (int i=0; i<estimate.getChildCount(); i++)
		{
			writeEstimate(stream, estimate.getChildAt(i), level + 1);
		}
	}
	
}
