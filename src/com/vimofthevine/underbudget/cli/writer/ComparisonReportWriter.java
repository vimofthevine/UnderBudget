package com.vimofthevine.underbudget.cli.writer;

import java.io.PrintStream;
import java.text.SimpleDateFormat;

import com.vimofthevine.underbudget.analysis.AnalysisResults;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.transactions.Transaction;
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
		
		if (verbose)
		{
			writeTransactions(stream, estimate, level);
		}
		
		// Print out estimate sub-tree
		for (int i=0; i<estimate.getChildCount(); i++)
		{
			writeEstimate(stream, estimate.getChildAt(i), level + 1);
		}
	}
	
	/**
	 * Writes the transactions for the given estimate to the
	 * given output stream at the given indentation level
	 * 
	 * @param stream   print stream on which to write
	 * @param estimate estimate whose transactions are to be written
	 * @param level    indentation level
	 */
	protected void writeTransactions(PrintStream stream, Estimate estimate, int level)
	{
		for (Transaction transaction : estimate.getTransactions())
		{
			StringBuffer buffer = new StringBuffer();
			
			// Add indentation
			for (int i=0; i<(level + 1); i++)
			{
				buffer.append("  ");
			}
			
			int indent = 2 * (level + 1);
			
			buffer.append("+");
			
			// Add date (8 chars)
			SimpleDateFormat dateFormat = new SimpleDateFormat("MM/dd/yy");
			buffer.append(dateFormat.format(transaction.date));
			
			// Separator (1 char)
			buffer.append(" ");
			
			// Add payee (16 chars)
			if (transaction.payee.length() > 16)
			{
				buffer.append(transaction.payee.substring(0, 16));
			}
			else
			{
				buffer.append(transaction.payee);
			}
			
			// Add filler
			for (int i=0; i<(16 - transaction.payee.length()); i++)
			{
				buffer.append(" ");
			}
			
			// Separator (1 char)
			buffer.append(" ");
			
			// Add memo (16 chars)
			if (transaction.memo.length() > 16)
			{
				buffer.append(transaction.memo.subSequence(0, 16));
			}
			else
			{
				buffer.append(transaction.memo);
			}
			
			// Add filler
			for (int i=0; i<(16 - transaction.memo.length()); i++)
			{
				buffer.append(" ");
			}
			
			// Separator (1 char)
			buffer.append(" ");
			
			// Add deposit (24 chars)
			String deposit = transaction.deposit.name;
			if (deposit.length() > (24 - indent))
			{
				buffer.append(deposit.substring(0, (24 - indent)));
			}
			else
			{
				buffer.append(deposit);
			}
			
			// Add filler
			for (int i=0; i<(24 - indent - deposit.length()); i++)
			{
				buffer.append(" ");
			}
			
			// Separator (1 char)
			buffer.append(" ");
			
			// Add amount (10 chars)
			String amount = FormatHelper.formatCurrency(transaction.value);
			if (amount.length() > 10)
			{
				buffer.append(amount.subSequence(0, 10));
			}
			else
			{
				buffer.append(amount);
			}
			
			stream.println(buffer.toString());
		}
	}
	
}
