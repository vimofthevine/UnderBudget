package com.vimofthevine.underbudget.cli;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.vimofthevine.underbudget.budget.file.BudgetFile;
import com.vimofthevine.underbudget.budget.file.BudgetFileException;
import com.vimofthevine.underbudget.cli.writer.ProgressWriter;
import com.vimofthevine.underbudget.cli.writer.UsageWriter;
import com.vimofthevine.underbudget.cli.writer.VersionWriter;
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
	protected String reportTypes;
	
	/**
	 * Path to transaction file
	 */
	protected String transactionFilePath;
	
	/**
	 * Path to export file
	 */
	protected String exportFilePath;
	
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
	
	public void execute()
	{
		try
		{
			if (budgetFilePath == null || importFilePath == null)
				printUsageAndExit();
			
    		BudgetFile budgetFile = new BudgetFile(budgetFilePath);
    		budgetFile.getParserProgress().addTaskProgressListener(
    			new ProgressWriter("Opening budget file", System.out));
    		budgetFile.parse();
    		
    		ImportFile importFile = new ImportFile(importFilePath);
    		importFile.getParserProgress().addTaskProgressListener(
    			new ProgressWriter("Importing transactions", System.out));
    		importFile.parse(budgetFile.getBudget().meta.getPeriod());
    		
    		System.out.print("                                                   \r");
    		
    		BudgetAnalyzer analyzer = new BudgetAnalyzer();
    		analyzer.setBudget(budgetFile.getBudget());
    		analyzer.setTransactions(importFile.getTransactions());
    		analyzer.getProgress().addTaskProgressListener(
    			new ProgressWriter("Analyzing", System.out));
    		
    		analyzer.run();
    		
    		AnalysisResults results = analyzer.getResults();
    		
    		printResults(results);
		}
		catch (BudgetFileException bfe)
		{
			System.err.println("Unable to open budget file: " + bfe.getMessage());
		}
		catch (ImportFileException ife)
		{
			System.err.println("Unable to import transactions: " + ife.getMessage());
		}
	}
	
	protected void printResults(AnalysisResults results)
	{
		if (reportTypes.contains("comp"))
		{
			ReportWriter writer = new ComparisonReportWriter(results);
			writer.write(System.out);
		}
		
		if (reportTypes.contains("alloc"))
		{
			ReportWriter writer = new AllocationReportWriter(results);
			writer.write(System.out);
		}
		
		if (reportTypes.contains("work"))
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
		Launcher app = new Launcher();
		app.parseOptions(args);
		app.execute();
	}

}
