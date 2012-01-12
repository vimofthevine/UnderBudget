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

package com.vimofthevine.underbudget.report.export;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.math.BigDecimal;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.vimofthevine.underbudget.Application;
import com.vimofthevine.underbudget.analysis.AnalysisResults;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.report.AllocationEntry;
import com.vimofthevine.underbudget.report.WorksheetEntry;
import com.vimofthevine.underbudget.transactions.Transaction;
import com.vimofthevine.underbudget.util.FormatHelper;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * HTML writer for exporting analysis results to an HTML file.
 * 
 * I wish there was a better way to do this other than printing
 * line-by-line, but using the DOM API was cumbersome to get the
 * HTML the way I wanted it.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class HtmlReportWriter implements ReportWriter {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(HtmlReportWriter.class.getName());
	
	/**
	 * Task progress	
	 */
	protected TaskProgress progress;
	
	/**
	 * Buffered writer
	 */
	protected BufferedWriter writer;
	
	/**
	 * Default constructor
	 */
	public HtmlReportWriter()
	{
		logger.log(Level.FINE, "Creating HTML report writer");
		progress = new TaskProgress();
	}
	
	@Override
	public TaskProgress getProgress()
	{
		return progress;
	}
	
	@Override
	public void write(OutputStream stream, AnalysisResults results)
	throws ReportExportException
	{
		try
		{
			progress.reset();
			
			writer = new BufferedWriter(new OutputStreamWriter(stream));
			
			writer.write("<!DOCTYPE html>");
			writer.newLine();
			writer.write("<html lang=\"en\">");
			writer.newLine();
			
			writeHeadSection();
			
			writer.write("\t<body>");
			writer.newLine();
			writer.write("\t\t<div id=\"wrapper\">");
			writer.newLine();
			
			writeHeader();
			writeTableOfContents();
			
			writer.write("\t\t\t<section id=\"content\">");
			writer.newLine();
			
			writeSummaryReport(results, 20);
			writeComparisonReport(results, 20);
			writeAllocationReport(results, 20);
			writeWorksheetReport(results, 20);
			
			writer.write("\t\t\t</section>");
			writer.newLine();
			
			writeFooter();
			
			writer.write("\t\t</div>");
			writer.newLine();
			writer.write("\t</body>");
			writer.newLine();
			writer.write("</html>");
			
			writer.close();
			
			progress.complete();
		}
		catch (Exception e)
		{
			if (e instanceof ReportExportException)
				throw (ReportExportException) e;
			else
			{
				logger.log(Level.WARNING, "Error exporting HTML report", e);
				throw new ReportExportException("Unable to export report to file");
			}
		}
	}
	
	/**
	 * Writes the head section of the HTML report
	 */
	protected void writeHeadSection() throws IOException
	{
		writer.write("\t<head>");
		writer.newLine();
		writer.write("\t\t<meta charset=\"utf-8\">");
		writer.newLine();
		writer.write("\t\t<title>Budget Analysis Report</title>");
		writer.newLine();
		writer.newLine();
		writer.write("\t\t<link rel=\"stylesheet\" href=\"style.css\" type=\"text/css\" />");
		writer.newLine();
		writer.newLine();
		writer.write("\t\t<!--[if IE]>");
		writer.newLine();
		writer.write("\t\t<script src=\"http://html5shiv.googlecode.com/svn/trunk/html5.js\"></script>");
		writer.newLine();
		writer.write("\t\t<![endif]-->");
		writer.newLine();
		writer.write("\t</head>");
		writer.newLine();
	}
	
	/**
	 * Writes the report header
	 */
	protected void writeHeader() throws IOException
	{
		writer.write("\t\t\t<header>");
		writer.newLine();
		writer.write("\t\t\t\t<hgroup>");
		writer.newLine();
		writer.write("\t\t\t\t\t<h1>Budget Analysis Report</h1>");
		writer.newLine();
		writer.write("\t\t\t\t</hgroup>");
		writer.newLine();
		writer.write("\t\t\t</header>");
		writer.newLine();
	}
	
	/**
	 * Writes the table of contents
	 */
	protected void writeTableOfContents() throws IOException
	{
		writer.write("\t\t\t<nav>");
		writer.newLine();
		writer.write("\t\t\t\t<ul>");
		writer.newLine();
		writer.write("\t\t\t\t\t<li><a href=\"#summary\">Report Summary</a></li>");
		writer.newLine();
		writer.write("\t\t\t\t\t<li><a href=\"#comparison\">Comparison Summary</a></li>");
		writer.newLine();
		writer.write("\t\t\t\t\t<li><a href=\"#allocation\">Allocation Summary</a></li>");
		writer.newLine();
		writer.write("\t\t\t\t\t<li><a href=\"#worksheet\">Worksheet Summary</a></li>");
		writer.newLine();
		writer.write("\t\t\t\t</ul>");
		writer.newLine();
		writer.write("\t\t\t</nav>");
		writer.newLine();
	}
	
	/**
	 * Writes the footer
	 */
	protected void writeFooter() throws IOException
	{
		SimpleDateFormat format = new SimpleDateFormat("M/dd/yy h:mm a");
		
		writer.write("\t\t\t<footer>");
		writer.newLine();
		writer.write("\t\t\t\t<p>");
		writer.newLine();
		writer.write("\t\t\t\t\tThis report generated by " + Application.TITLE + " "
			+ Application.VERSION + " at " + format.format(new Date()));
		writer.newLine();
		writer.write("\t\t\t\t</p>");
		writer.newLine();
		writer.write("\t\t\t</footer>");
	}
	
	/**
	 * Writes the summary report
	 * 
	 * @param results   analysis results
	 * @param allocated allocated percentage of the overall task
	 */
	protected void writeSummaryReport(AnalysisResults results, float allocated)
	throws IOException
	{
		writer.write("\t\t\t\t<article>");
		writer.newLine();
		writer.write("\t\t\t\t\t<header>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<h1 id=\"summary\">Report Summary</h1>");
		writer.newLine();
		writer.write("\t\t\t\t\t</header>");
		writer.newLine();
		writer.write("\t\t\t\t\t<dl>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dt>Start Date</dt>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dd>" + FormatHelper.formatDate(results.budget.meta.getPeriod().getStartDate()) + "</dd>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dt>End Date</dt>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dd>" + FormatHelper.formatDate(results.budget.meta.getPeriod().getEndDate()) + "</dd>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dt>Initial Balance</dt>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dd>" + FormatHelper.formatCurrency(results.budget.meta.getInitialBalance()) + "</dd>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dt>Estimated Income</dt>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dd>" + FormatHelper.formatCurrency(results.total.estimatedIncome) + "</dd>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dt>Estimated Expense</dt>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dd>" + FormatHelper.formatCurrency(results.total.estimatedExpense) + "</dd>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dt>Actual Income</dt>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dd>" + FormatHelper.formatCurrency(results.total.actualIncome) + "</dd>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dt>Actual Expense</dt>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dd>" + FormatHelper.formatCurrency(results.total.actualExpense) + "</dd>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dt>Expected Income</dt>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dd>" + FormatHelper.formatCurrency(results.total.expectedIncome) + "</dd>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dt>Expected Expense</dt>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dd>" + FormatHelper.formatCurrency(results.total.expectedExpense) + "</dd>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dt>Estimated Net Change</dt>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dd>" + FormatHelper.formatCurrency(results.total.getEstimatedNetChange()) + "</dd>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dt>Actual Net Change</dt>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dd>" + FormatHelper.formatCurrency(results.total.getActualNetChange()) + "</dd>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dt>Expected Net Change</dt>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dd>" + FormatHelper.formatCurrency(results.total.getExpectedNetChange()) + "</dd>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dt>Estimated Ending Balance</dt>");
		writer.newLine();
		BigDecimal initial = results.budget.meta.getInitialBalance();
		writer.write("\t\t\t\t\t\t<dd>" + FormatHelper.formatCurrency(initial.add(results.total.getEstimatedNetChange())) + "</dd>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dt>Actual Ending Balance</dt>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dd>" + FormatHelper.formatCurrency(initial.add(results.total.getActualNetChange())) + "</dd>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dt>Expected Ending Balance</dt>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<dd>" + FormatHelper.formatCurrency(initial.add(results.total.getExpectedNetChange())) + "</dd>");
		writer.newLine();
		writer.write("\t\t\t\t\t</dl>");
		writer.newLine();
		writer.write("\t\t\t\t</article>");
		writer.newLine();
		writer.newLine();
	}
	
	/**
	 * Writes the comparison report
	 * 
	 * @param results   analysis results
	 * @param allocated allocated percentage of the overall task
	 */
	protected void writeComparisonReport(AnalysisResults results, float allocated)
	throws IOException
	{
		writer.write("\t\t\t\t<article>");
		writer.newLine();
		writer.write("\t\t\t\t\t<header>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<h1 id=\"comparison\">Comparison Report</h1>");
		writer.newLine();
		writer.write("\t\t\t\t\t</header>");
		writer.newLine();
		writer.write("\t\t\t\t\t<ul>");
		writer.newLine();
		
		writer.write("\t\t\t\t\t\t<li>Income Estimates");
		writer.newLine();
		writer.write("\t\t\t\t\t\t\t<ul>");
		writer.newLine();
		
		writeEstimateList(results.budget.incomes, 8);
		writeEstimateList(results.rules.get(results.rules.size() - 2).estimate, 8);
		
		writer.write("\t\t\t\t\t\t\t</ul>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t</li>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<li>Expense Estimates");
		writer.newLine();
		writer.write("\t\t\t\t\t\t\t<ul>");
		writer.newLine();
		
		writeEstimateList(results.budget.expenses, 8);
		writeEstimateList(results.rules.get(results.rules.size() - 1).estimate, 8);
		
		writer.write("\t\t\t\t\t\t\t</ul>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t</li>");
		writer.newLine();
		
		writer.write("\t\t\t\t\t</ul>");
		writer.newLine();
		writer.write("\t\t\t\t</article>");
		writer.newLine();
		writer.newLine();
		
		progress.add(allocated);
	}
	
	/**
	 * Writes an estimate and its child estimates as an HTML UL list
	 * 
	 * @param estimate the estimate to print
	 * @param indent   current indentation level
	 */
	protected void writeEstimateList(Estimate estimate, int indent)
	throws IOException
	{
		StringBuffer buffer = new StringBuffer();
		for (int i=0; i<indent; i++)
		{
			buffer.append("\t");
		}
		String indentation = buffer.toString();
		
		buffer = new StringBuffer(indentation);
		buffer.append("<li>");
		buffer.append(estimate.getName() + " (" + estimate.getNotes() + ") ");
		buffer.append("Est:" + FormatHelper.formatCurrency(estimate.getAmount()) + ", ");
		buffer.append("Act:" + FormatHelper.formatCurrency(estimate.getActualAmount()) + ", ");
		buffer.append("Diff:" + FormatHelper.formatCurrency(estimate.getDifferenceAmount()));
		writer.write(buffer.toString());
		writer.newLine();
		
		for (Transaction transaction : estimate.getTransactions())
		{
			buffer = new StringBuffer(indentation + "\t");
			buffer.append("<br />");
			buffer.append(transaction.toString());
			writer.write(buffer.toString());
			writer.newLine();
		}
		
		if (estimate.isCategory())
		{
			writer.write(indentation + "\t<ul>");
			writer.newLine();
			
			for (int i=0; i<estimate.getChildCount(); i++)
			{
				writeEstimateList(estimate.getChildAt(i), indent + 2);
			}
			
			writer.write(indentation + "\t</ul>");
			writer.newLine();
		}
		
		writer.write(indentation + "</li>");
		writer.newLine();
	}
	
	/**
	 * Writes the allocation report
	 * 
	 * @param results   analysis results
	 * @param allocated allocated percentage of the overall task
	 */
	protected void writeAllocationReport(AnalysisResults results, float allocated)
	throws IOException
	{
		writer.write("\t\t\t\t<article>");
		writer.newLine();
		writer.write("\t\t\t\t\t<header>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<h1 id=\"allocation\">Allocation Report</h1>");
		writer.newLine();
		writer.write("\t\t\t\t\t</header>");
		writer.newLine();
		writer.write("\t\t\t\t\t<table>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<thead>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t\t<tr>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t\t\t<th>Transaction</th>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t\t\t<th>Estimate</th>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t\t\t<th>Rationale</th>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t\t</tr>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t</thead>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<tbody>");
		writer.newLine();
		
		for (AllocationEntry entry : results.allocation)
		{
			writer.write("\t\t\t\t\t\t\t<tr>");
			writer.newLine();
			writer.write("\t\t\t\t\t\t\t\t<td>" + entry.transaction.toString() + "</td>");
			writer.newLine();
			writer.write("\t\t\t\t\t\t\t\t<td>" + entry.estimate.toString() + "</td>");
			writer.newLine();
			writer.write("\t\t\t\t\t\t\t\t<td>" + entry.rationale + "</td>");
			writer.newLine();
			writer.write("\t\t\t\t\t\t\t</tr>");
			writer.newLine();
		}
		
		writer.write("\t\t\t\t\t\t</tbody>");
		writer.newLine();
		writer.write("\t\t\t\t\t</table>");
		writer.newLine();
		writer.write("\t\t\t\t</article>");
		writer.newLine();
		writer.newLine();
		
		progress.add(allocated);
	}
	
	/**
	 * Writes the worksheet report
	 * 
	 * @param results   analysis results
	 * @param allocated allocated percentage of the overall task
	 */
	protected void writeWorksheetReport(AnalysisResults results, float allocated)
	throws IOException
	{
		writer.write("\t\t\t\t<article>");
		writer.newLine();
		writer.write("\t\t\t\t\t<header>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<h1 id=\"worksheet\">Worksheet Report</h1>");
		writer.newLine();
		writer.write("\t\t\t\t\t</header>");
		writer.newLine();
		writer.write("\t\t\t\t\t<table>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<thead>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t\t<tr>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t\t\t<th>Estimate</th>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t\t\t<th>Estimated</th>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t\t\t<th>Actual</th>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t\t\t<th>Est. Total</th>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t\t\t<th>Act. Total</th>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t\t\t<th>Exp. Total</th>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t\t\t<th>Rationale</th>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t\t</tr>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t</thead>");
		writer.newLine();
		writer.write("\t\t\t\t\t\t<tbody>");
		writer.newLine();
		
		for (WorksheetEntry entry : results.worksheet)
		{
			writer.write("\t\t\t\t\t\t\t<tr>");
			writer.newLine();
			writer.write("\t\t\t\t\t\t\t\t<td>" + entry.estimate.getName() + "</td>");
			writer.newLine();
			writer.write("\t\t\t\t\t\t\t\t<td>" + FormatHelper.formatCurrency(entry.estimate.getAmount()) + "</td>");
			writer.newLine();
			writer.write("\t\t\t\t\t\t\t\t<td>" + FormatHelper.formatCurrency(entry.estimate.getActualAmount()) + "</td>");
			writer.newLine();
			writer.write("\t\t\t\t\t\t\t\t<td>" + FormatHelper.formatCurrency(entry.totals.getEstimatedNetChange()) + "</td>");
			writer.newLine();
			writer.write("\t\t\t\t\t\t\t\t<td>" + FormatHelper.formatCurrency(entry.totals.getActualNetChange()) + "</td>");
			writer.newLine();
			writer.write("\t\t\t\t\t\t\t\t<td>" + FormatHelper.formatCurrency(entry.totals.getExpectedNetChange()) + "</td>");
			writer.newLine();
			writer.write("\t\t\t\t\t\t\t\t<td>" + entry.rationale + "</td>");
			writer.newLine();
			writer.write("\t\t\t\t\t\t\t</tr>");
			writer.newLine();
		}
		
		writer.write("\t\t\t\t\t\t</tbody>");
		writer.newLine();
		writer.write("\t\t\t\t\t</table>");
		writer.newLine();
		writer.write("\t\t\t\t</article>");
		writer.newLine();
		writer.newLine();
		
		progress.add(allocated);
	}

}
