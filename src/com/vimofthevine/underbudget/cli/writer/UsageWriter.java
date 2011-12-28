package com.vimofthevine.underbudget.cli.writer;

import java.io.PrintStream;

/**
 * Writes application usage to the given output stream
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class UsageWriter {
	
	/**
	 * Writes application usage to the output stream
	 * 
	 * @param stream print stream on which to write
	 */
	public void write(PrintStream stream)
	{
		stream.println("Usage: underbudgetcli [-options] -b <budget-file> -i <import-file>");
		stream.println("           (to perform budget analysis)");
		stream.println("   or  underbudgetcli -t|--template <budget-file>");
		stream.println("           (to specify the template budget");
		stream.println("   or  underbudgetcli -n|--new <new-budget-file-name>");
		stream.println("           (to create a new budget file");
		stream.println();
		stream.println("where options include:");
		stream.println("    --version     print product version and exit");
		stream.println("    --help        print this help message");
		stream.println("    -v|--verbose  enable verbose output");
		stream.println("    -l|--long     enable verbose reporting");
		stream.println("    -o <output-file>");
		stream.println("                  write output to specified file instead of standard out");
		stream.println("    -r <report-types>");
		stream.println("                  specify the reports to be printed as a comma-separated");
		stream.println("                  list of types (comp, alloc, or work)");
		stream.println("    -tr <transaction-file>");
		stream.println("                  exports the imported transactions to a transaction file");
		stream.println("    -e <export-file>");
		stream.println("                  exports the reports to the specified file (xml, html, or pdf)");
		stream.println();
	}

}
