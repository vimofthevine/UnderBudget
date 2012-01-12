/*
 * Copyright 2012 Kyle Treubig
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.vimofthevine.underbudget.cli.writer;

import java.io.PrintStream;
import java.math.BigDecimal;

import com.vimofthevine.underbudget.analysis.AnalysisResults;
import com.vimofthevine.underbudget.budget.BudgetMeta;
import com.vimofthevine.underbudget.report.BalanceTotal;
import com.vimofthevine.underbudget.util.FormatHelper;

/**
 * Writes the summary report to the given output stream
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class SummaryReportWriter {
	
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
	public SummaryReportWriter(AnalysisResults results, boolean verbose)
	{
		this.results = results;
		this.verbose = verbose;
	}
	
	/**
	 * Writes the summary report to the output stream
	 * 
	 * @param stream print stream on which to write
	 */
	public void write(PrintStream stream)
	{
		stream.println("Summary Report");
		stream.println("--------------------------------------------------------------------------------");
		
		BudgetMeta meta = results.budget.meta;
		stream.println("Start Date:      "
			+ FormatHelper.formatDate(meta.getPeriod().getStartDate()));
		stream.println("End Date:        "
			+ FormatHelper.formatDate(meta.getPeriod().getEndDate()));
		stream.println("Initial Balance: "
			+ FormatHelper.formatCurrency(meta.getInitialBalance()));
		
		BalanceTotal total = results.total;
		
		stream.println();
		
		if (verbose)
		{
			stream.println("Total Estimated Income:  "
				+ FormatHelper.formatCurrency(total.estimatedIncome));
			stream.println("Total Estimated Expense: "
				+ FormatHelper.formatCurrency(total.estimatedExpense));
			stream.println("Total Actual Income:     "
				+ FormatHelper.formatCurrency(total.actualIncome));
			stream.println("Total Actual Expense:    "
				+ FormatHelper.formatCurrency(total.actualExpense));
			stream.println("Total Expected Income:   "
				+ FormatHelper.formatCurrency(total.expectedIncome));
			stream.println("Total Expected Expense:  "
				+ FormatHelper.formatCurrency(total.expectedExpense));
			stream.println();
		}
		
		BigDecimal initial = meta.getInitialBalance();
		stream.println("Estimated Ending Balance: "
			+ FormatHelper.formatCurrency(
				initial.add(total.getEstimatedNetChange())
			)
		);
		stream.println("Actual Ending Balance:    "
			+ FormatHelper.formatCurrency(
				initial.add(total.getActualNetChange())
			)
		);
		stream.println("Expected Ending Balance:  "
			+ FormatHelper.formatCurrency(
				initial.add(total.getExpectedNetChange())
			)
		);
		
		stream.println("--------------------------------------------------------------------------------");
		stream.println();
	}

}
