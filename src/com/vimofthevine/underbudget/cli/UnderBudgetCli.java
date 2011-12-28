package com.vimofthevine.underbudget.cli;

import com.vimofthevine.underbudget.Application;
import com.vimofthevine.underbudget.budget.file.BudgetFile;
import com.vimofthevine.underbudget.transactions.importer.ImportFile;

/**
 * Command-line interface application launcher
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class UnderBudgetCli {
	
	/**
	 * Flag indicating verbose mode
	 */
	protected boolean verbose = false;
	
	/**
	 * Path to budget file
	 */
	protected String budgetFilePath;
	
	/**
	 * Path to import file
	 */
	protected String importFilePath;
	
	/**
	 * Path to output file
	 */
	protected String outputFilePath;
	
	/**
	 * Report types
	 */
	protected String reports;
	
	/**
	 * Parses given command-line arguments
	 * 
	 * @param args command-line runtim arguments
	 */
	public void parseOptions(String[] args)
	{
		for (int i=0; i<args.length; i++)
		{
			String arg = args[i];
			
			// Print version info
			if (arg.equals("--version"))
			{
				printVersion();
			}
			// Print usage
			else if (arg.equals("--help"))
			{
				printUsage();
			}
			// Enable verbose mode
			else if (arg.equals("-v") || arg.equals("--verbose"))
			{
				verbose = true;
			}
			// TODO need to check that next argument exists
			// Specify budget file
			else if (arg.equals("-b"))
			{
				budgetFilePath = args[++i];
			}
			// Specify import file
			else if (arg.equals("-i"))
			{
				importFilePath = args[++i];
			}
			// Specify output file
			else if (arg.equals("-o"))
			{
				outputFilePath = args[++i];
			}
			// Specify report types
			else if (arg.equals("-r"))
			{
				reports = args[++i];
			}
		}
	}
	
	public void execute()
	{
		BudgetFile budgetFile = new BudgetFile(budgetFilePath);
		budgetFile.parse();
		
		ImportFile importFile = new ImportFile(importFilePath);
		importFile.parse(budgetFile.getBudget().meta.getPeriod());
		
		BudgetAnalyzer analyzer = new BudgetAnalyzer();
		analyzer.setBudget(budgetFile.getBudget());
		analyzer.setTransactions(importFile.getTransactions());
		
		analyzer.run();
		
		AnalysisResults results = analyzer.getResults();
		
		printResults(results);
	}
	
	/**
	 * Prints application version info to standard out
	 */
	protected void printVersion()
	{
		System.out.println(Application.TITLE + " " + Application.VERSION);
		System.out.println("Copyright (C) + " + Application.COPY_YEAR + " " + Application.COPY_AUTHOR);
		System.out.println(Application.WEBSITE);
	}
	
	/**
	 * Prints application usage info to standard err
	 */
	protected void printUsage()
	{
		System.err.println("usage: underbudgetcli [--version] [--help] [-v|--verbose]");
		System.err.println("                      [-o <output-file>] [-r <report-types>]");
		System.err.println("                      -b <budget-file> -i <import-file>");
	}
	
	protected void printResults(AnalysisResults results)
	{
		if (reports.contains("comp"))
		{
			ReportWriter writer = new ComparisonReportWriter(results);
			writer.write(System.out);
		}
		
		if (reports.contains("alloc"))
		{
			ReportWriter writer = new AllocationReportWriter(results);
			writer.write(System.out);
		}
		
		if (reports.contains("work"))
		{
			ReportWriter writer = new WorksheetReportWriter(results);
			writer.write(System.out);
		}
	}
	
	/**
	 * Point of entry for the UnderBudget CLI application
	 * 
	 * @param args command-line runtime arguments
	 */
	public static void main(String[] args)
	{
		UnderBudgetCli app = new UnderBudgetCli();
		app.parseOptions(args);
		app.execute();
	}

}
