package com.vimofthevine.underbudget.cli.writer;

import java.io.PrintStream;
import java.text.SimpleDateFormat;

import com.vimofthevine.underbudget.analysis.AnalysisResults;
import com.vimofthevine.underbudget.report.AllocationEntry;
import com.vimofthevine.underbudget.util.FormatHelper;

/**
 * Writes the allocation report to the given output stream
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AllocationReportWriter {
	
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
	public AllocationReportWriter(AnalysisResults results, boolean verbose)
	{
		this.results = results;
		this.verbose = verbose;
	}
	
	/**
	 * Writes the allocation report to the output stream
	 * 
	 * @param stream print stream on which to write
	 */
	public void write(PrintStream stream)
	{
		stream.println();
		stream.println("Allocation Report");
		stream.println("--------------------------------------------------------------------------------");
		stream.println("Transaction                        Estimate            Rationale                ");
		stream.println("--------------------------------------------------------------------------------");
		
		for (AllocationEntry entry : results.allocation)
		{
			stream.println(createLine1(entry));
			
			if (verbose)
			{
				stream.println(createLine2(entry));
			}
			
			stream.println();
		}
		
		stream.println("--------------------------------------------------------------------------------");
	}
	
	/**
	 * Creates line 1 of the allocation entry
	 * 
	 * @param entry allocation entry
	 * @return line 1 string
	 */
	protected String createLine1(AllocationEntry entry)
	{
		// Transaction gets 35, estimate gets 20, rationale gets 25
		StringBuffer line1 = new StringBuffer();
		
		// Add date (8 chars)
		SimpleDateFormat dateFormat = new SimpleDateFormat("MM/dd/yy");
		line1.append(dateFormat.format(entry.transaction.date));
		
		// Separator (1 char, 9 total)
		line1.append(" ");
		
		int slack = 0;
		
		// Add payee (14 chars, 23 total)
		if (entry.transaction.payee.length() > 14)
		{
			line1.append(entry.transaction.payee.substring(0, 14));
		}
		else
		{
			slack = 14 - entry.transaction.payee.length();
			line1.append(entry.transaction.payee);
		}
		
		// Separator (1 char, 24 total)
		line1.append(" ");
		
		// Add amount (10 char, 34 total)
		String amount = FormatHelper.formatCurrency(entry.transaction.value);
		if (amount.length() > (10 + slack))
		{
			line1.append(amount.substring(0, (10 + slack)));
			slack = 0;
		}
		else
		{
			line1.append(amount);
			slack += (10 - amount.length());
		}
		
		// Separator (1 char, 35 total)
		line1.append(",");
		
		// Fill up any slack
		for (int i=0; i<slack; i++)
		{
			line1.append(" ");
		}
		
		// Add estimate (19 chars, 54 total)
		String estimateStr = entry.estimate.toString();
		if (estimateStr.length() > 19)
		{
			line1.append(estimateStr.substring(0, 19));
		}
		else
		{
			slack = 19 - estimateStr.length();
			line1.append(estimateStr);
		}
		
		// Separator (1 char, 55 total)
		line1.append(",");
		
		// Fill up any slack
		for (int i=0; i<slack; i++)
		{
			line1.append(" ");
		}
		
		// Add rationale (25 chars, 80 total)
		if (entry.rationale.length() > 25)
		{
			line1.append(entry.rationale.substring(0, 25));
		}
		else
		{
			line1.append(entry.rationale);
		}
		
		return line1.toString();
	}
	
	/**
	 * Creates line 2 of the allocation entry
	 * 
	 * @param entry allocation entry
	 * @return line 2 string
	 */
	protected String createLine2(AllocationEntry entry)
	{
		// Transaction gets 35, estimate gets 20, rationale gets 25
		StringBuffer line2 = new StringBuffer();
		
		// Add indent (2 chars)
		line2.append("  ");
		
		int slack = 0;
		
		// Add withdrawal (16 chars, 18 total)
		String withdrawal = entry.transaction.withdrawal.name;
		if (withdrawal.length() > 16)
		{
			line2.append(withdrawal.substring(0, 16));
		}
		else
		{
			slack = 16 - withdrawal.length();
			line2.append(withdrawal);
		}
		
		// Add separator (1 char, 19 total)
		line2.append(">");
		
		// Add deposit (16 chars, 35 total)
		String deposit = entry.transaction.deposit.name;
		if (deposit.length() > (16 + slack))
		{
			line2.append(deposit.substring(0, (16 + slack)));
			slack = 0;
		}
		else
		{
			line2.append(deposit);
			slack += (16 - deposit.length());
		}
		
		// Fill up any slack
		for (int i=0; i<slack; i++)
		{
			line2.append(" ");
		}
		
		// Fill estimate column (20 chars, 55 total)
		line2.append("                    ");
		
		// Add rationale line 2 (25 chars, 80 total)
		if (entry.rationale.length() > 25)
		{
			String rationale = entry.rationale.substring(25);
			if (rationale.length() > 25)
			{
				line2.append(rationale.substring(0, 25));
			}
			else
			{
				line2.append(rationale);
			}
		}
		
		return line2.toString();
	}

}
