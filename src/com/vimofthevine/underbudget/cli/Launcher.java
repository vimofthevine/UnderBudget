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

package com.vimofthevine.underbudget.cli;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.vimofthevine.underbudget.analysis.AnalysisResults;
import com.vimofthevine.underbudget.analysis.BudgetAnalysisException;
import com.vimofthevine.underbudget.analysis.BudgetAnalyzer;
import com.vimofthevine.underbudget.budget.file.BudgetFile;
import com.vimofthevine.underbudget.budget.file.BudgetFileException;
import com.vimofthevine.underbudget.budget.file.TemplateFile;
import com.vimofthevine.underbudget.cli.writer.AllocationReportWriter;
import com.vimofthevine.underbudget.cli.writer.ComparisonReportWriter;
import com.vimofthevine.underbudget.cli.writer.ProgressWriter;
import com.vimofthevine.underbudget.cli.writer.SummaryReportWriter;
import com.vimofthevine.underbudget.cli.writer.UsageWriter;
import com.vimofthevine.underbudget.cli.writer.VersionWriter;
import com.vimofthevine.underbudget.cli.writer.WorksheetReportWriter;
import com.vimofthevine.underbudget.report.export.HtmlReportWriter;
import com.vimofthevine.underbudget.report.export.ReportExportException;
import com.vimofthevine.underbudget.report.export.XmlReportWriter;
import com.vimofthevine.underbudget.transactions.Transaction;
import com.vimofthevine.underbudget.transactions.file.TransactionFile;
import com.vimofthevine.underbudget.transactions.importer.ImportFile;
import com.vimofthevine.underbudget.transactions.importer.ImportFileException;

/**
 * Command-line interface application launcher
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class Launcher {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(Launcher.class.getName());
	
	/**
	 * Output stream
	 */
	protected PrintStream out = System.out;
	
	/**
	 * Flag indicating verbose mode
	 */
	protected boolean verbose = false;
	
	/**
	 * Flag indicating long mode
	 */
	protected boolean longMode = false;
	
	/**
	 * Report types
	 */
	protected String reportTypes = "";
	
	/**
	 * Path to transaction file
	 */
	protected String transactionFilePath = "";
	
	/**
	 * Path to export file
	 */
	protected String exportFilePath = "";
	
	/**
	 * Path to budget file
	 */
	protected String budgetFilePath;
	
	/**
	 * Path to import file
	 */
	protected String importFilePath;
	
	/**
	 * Parses given command-line arguments
	 * 
	 * @param args command-line runtime arguments
	 */
	public void parseOptions(String[] args)
	{
		for (int i=0; i<args.length; i++)
		{
			String arg = args[i];
			
			// Print version info
			if (arg.equals("--version"))
			{
				VersionWriter version = new VersionWriter();
				version.write(System.out);
				System.exit(0);
			}
			// Print usage
			else if (arg.equals("--help"))
			{
				UsageWriter usage = new UsageWriter();
				usage.write(System.out);
				System.exit(0);
			}
			// Enable verbose mode
			else if (arg.equals("-v") || arg.equals("--verbose"))
			{
				verbose = true;
			}
			// Enable long mode
			else if (arg.equals("-l") || arg.equals("--long"))
			{
				longMode = true;
			}
			// Specify output file
			else if (arg.equals("-o"))
			{
				if (args.length == (i+1))
				{
					printUsageAndExit();
				}
				
				String outputFilePath = args[++i];
				
				try
				{
					out = new PrintStream(new FileOutputStream(outputFilePath));
				}
				catch (IOException ioe)
				{
					logger.log(Level.WARNING, "Error opening file for writing", ioe);
					System.err.println("Unable to open " + outputFilePath + " for writing");
				}
			}
			// Specify report types
			else if (arg.equals("-r"))
			{
				if (args.length == (i+1))
				{
					printUsageAndExit();
				}
				
				reportTypes = args[++i];
			}
			// Specify transaction file
			else if (arg.equals("-tr"))
			{
				if (args.length == (i+1))
				{
					printUsageAndExit();
				}
				
				transactionFilePath = args[++i];
			}
			// Specify export file
			else if (arg.equals("-e"))
			{
				if (args.length == (i+1))
				{
					printUsageAndExit();
				}
				
				exportFilePath = args[++i];
			}
			// Specify budget file
			else if (arg.equals("-b"))
			{
				if (args.length == (i+1))
				{
					printUsageAndExit();
				}
				
				budgetFilePath = args[++i];
			}
			// Specify import file
			else if (arg.equals("-i"))
			{
				if (args.length == (i+1))
				{
					printUsageAndExit();
				}
				
				importFilePath = args[++i];
			}
			// Create a new budget file
			else if (arg.equals("-n") || arg.equals("--new"))
			{
				if (args.length == (i+1))
				{
					printUsageAndExit();
				}
				
				createBudgetFileFromTemplateAndExit(args[++i]);
			}
			// Save a budget as the new template
			else if (arg.equals("-t") || arg.equals("--template"))
			{
				if (args.length == (i+1))
				{
					printUsageAndExit();
				}
				
				saveBudgetAsTemplateAndExit(args[++i]);
			}
		}
	}
	
	/**
	 * Prints application usage and exits
	 */
	protected void printUsageAndExit()
	{
		UsageWriter usage = new UsageWriter();
		usage.write(System.err);
		System.exit(0);
	}
	
	/**
	 * Creates a new budget file, based on the template file
	 * specified in the application data directory, or as embedded
	 * in the jar file if there is none in the data directory
	 * 
	 * @param file name of the budget file to be created
	 */
	protected void createBudgetFileFromTemplateAndExit(String file)
	{
		try
		{
			TemplateFile newBudgetFile = new TemplateFile();
			
			newBudgetFile.getParserProgress().addTaskProgressListener(
				new ProgressWriter("Parsing template budget", System.out));
			newBudgetFile.getWriterProgress().addTaskProgressListener(
				new ProgressWriter("Saving budget file", System.out));
			
			newBudgetFile.parse();
			System.out.print("                                                   \r");
			newBudgetFile.write(file);
			System.out.print("                                                   \r");
			
			System.exit(0);
		}
		catch (BudgetFileException bfe)
		{
			System.err.println("Unable to create budget file: " + bfe.getMessage());
			System.exit(1);
		}
	}
	
	/**
	 * Reads the specified budget file and saves its budget contents
	 * as the template file
	 * 
	 * @param file budget to be used as the new template
	 */
	protected void saveBudgetAsTemplateAndExit(String file)
	{
		try
		{
			BudgetFile baseFile = new BudgetFile(file);
			baseFile.getParserProgress().addTaskProgressListener(
				new ProgressWriter("Opening budget file", System.out));
			baseFile.parse();
			System.out.print("                                                   \r");
			
			TemplateFile templateFile = new TemplateFile();
			templateFile.getWriterProgress().addTaskProgressListener(
				new ProgressWriter("Saving as template", System.out));
			templateFile.write(baseFile.getBudget());
			System.out.print("                                                   \r");
			
			System.exit(0);
		}
		catch (BudgetFileException bfe)
		{
			System.err.println("Unable to save budget as template: " + bfe.getMessage());
			System.exit(1);
		}
	}
	
	public void execute()
	{
		try
		{
			if (budgetFilePath == null || importFilePath == null)
				printUsageAndExit();
			
			long startTime = System.currentTimeMillis();
			
			// Read the budget file
    		BudgetFile budgetFile = new BudgetFile(budgetFilePath);
    		budgetFile.getParserProgress().addTaskProgressListener(
    			new ProgressWriter("Opening budget file", System.out));
    		
    		budgetFile.parse();
    		System.out.print("                                                   \r");
    		
    		// Read the import file
    		ImportFile importFile = new ImportFile(importFilePath);
    		importFile.getParserProgress().addTaskProgressListener(
    			new ProgressWriter("Importing transactions", System.out));
    		
    		importFile.parse(budgetFile.getBudget().meta.getPeriod());
    		System.out.print("                                                   \r");

    		// Analyze
    		BudgetAnalyzer analyzer = new BudgetAnalyzer(
    			budgetFile.getBudget(),
    			importFile.getTransactions()
    		);
    		analyzer.getProgress().addTaskProgressListener(
    			new ProgressWriter("Analyzing", System.out));
    		
    		analyzer.run();
    		System.out.print("                                                   \r");
    		System.out.println();

    		// Print results
    		AnalysisResults results = analyzer.getResults();
    		printResults(results);
    		
    		long endTime = System.currentTimeMillis();
    		
    		if (verbose)
    		{
    			System.out.println("Analysis complete in "
    				+ (endTime - startTime) + "ms");
    		}
    		
    		if ( ! exportFilePath.equals(""))
    		{
    			exportReport(results);
    		}
    		
    		if ( ! transactionFilePath.equals(""))
    		{
    			exportTransactions(importFile.getTransactions());
    		}
		}
		catch (BudgetFileException bfe)
		{
			System.err.println("Unable to open budget file: " + bfe.getMessage());
		}
		catch (ImportFileException ife)
		{
			System.err.println("Unable to import transactions: " + ife.getMessage());
		}
		catch (BudgetAnalysisException bae)
		{
			System.err.println("Error performing analysis: " + bae.getMessage());
		}
	}
	
	/**
	 * Prints analysis reports, as requested via the command-line options
	 * 
	 * @param results analysis results
	 */
	protected void printResults(AnalysisResults results)
	{
		SummaryReportWriter summary = new SummaryReportWriter(results, longMode);
		summary.write(out);
		
		if (reportTypes.contains("comp") || reportTypes.equals("all"))
		{
			ComparisonReportWriter writer = new ComparisonReportWriter(results, longMode);
			writer.write(out);
		}
		
		if (reportTypes.contains("alloc") || reportTypes.equals("all"))
		{
			AllocationReportWriter writer = new AllocationReportWriter(results, longMode);
			writer.write(out);
		}
		
		if (reportTypes.contains("work") || reportTypes.equals("all"))
		{
			WorksheetReportWriter writer = new WorksheetReportWriter(results, longMode);
			writer.write(System.out);
		}
	}
	
	/**
	 * Exports the analysis report, as requested via the command-line
	 * options, to the requested file
	 * 
	 * @param results analysis results
	 */
	protected void exportReport(AnalysisResults results)
	{
		try
		{
    		FileOutputStream stream = new FileOutputStream(exportFilePath);
    		
    		if (exportFilePath.endsWith("xml"))
    		{
    			XmlReportWriter writer = new XmlReportWriter();
    			writer.write(stream, results);
    		}
    		else if (exportFilePath.endsWith("html"))
    		{
    			HtmlReportWriter writer = new HtmlReportWriter();
    			writer.write(stream, results);
    		}
		}
		catch (ReportExportException ree)
		{
			logger.log(Level.WARNING, "Error exporting the report", ree);
		}
		catch (IOException ioe)
		{
			logger.log(Level.WARNING, "Error exporting the report", ioe);
		}
	}
	
	/**
	 * Exports all imported transactions (those that fit within the
	 * budgeting period window) to the requested file, as specified
	 * via the command-line options
	 * 
	 * @param transactions list of imported transactions
	 */
	protected void exportTransactions(List<Transaction> transactions)
	{
		try
		{
			TransactionFile transactionFile = new TransactionFile(transactionFilePath);
			transactionFile.setTransactions(transactions);
			transactionFile.write();
		}
		catch (Exception e)
		{
			logger.log(Level.WARNING, "Error exporting transactions", e);
		}
	}
	
	/**
	 * Point of entry for the UnderBudget CLI application
	 * 
	 * @param args command-line runtime arguments
	 */
	public static void main(String[] args)
	{
		Launcher app = new Launcher();
		app.parseOptions(args);
		app.execute();
	}

}
