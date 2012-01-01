package com.vimofthevine.underbudget.transactions.importer;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.vimofthevine.underbudget.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.transactions.Transaction;
import com.vimofthevine.underbudget.transactions.importer.parsers.ImportFileParser;
import com.vimofthevine.underbudget.transactions.importer.parsers.ImportFileParserFactory;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Representation of an import file as it is
 * stored on a file system
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ImportFile {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(ImportFile.class.getName());
	
	/**
	 * Transactions extracted from the file
	 */
	protected List<Transaction> transactions;
	
	/**
	 * Import file path
	 */
	protected String path;
	
	/**
	 * Import file parser
	 */
	protected ImportFileParser parser;
	
	/**
	 * Constructor, specifying the path
	 * to the import file
	 * 
	 * @param path file path
	 * @throws ImportFileException if the file is invalid/unsupported
	 */
	public ImportFile(String path) throws ImportFileException
	{
		this.path = path;
		
		try
		{
			parser = ImportFileParserFactory.createParser(new FileInputStream(path));
			
			if (parser == null)
				throw new ImportFileException("Unsupported import file type");
		}
		catch (FileNotFoundException e)
		{
			logger.log(Level.WARNING, "Error opening file", e);
			throw new ImportFileException("Import file not found");
		}
	}
	
	/**
	 * Returns the transactions defined in the file
	 * 
	 * @return extracted transactions
	 */
	public List<Transaction> getTransactions()
	{
		return transactions;
	}
	
	/**
	 * Returns a reference to the file parser progress
	 * 
	 * @return file parser task progress
	 */
	public TaskProgress getParserProgress()
	{
		return parser.getProgress();
	}
	
	/**
	 * Parses the import file, extracting the
	 * transactions defined in the file
	 * 
	 * @param period budgeting period
	 * @throws ImportFileException if any error occurs
	 */
	public void parse(BudgetingPeriod period) throws ImportFileException
	{
		try
		{
			InputStream stream = new FileInputStream(path);
			parser.parse(stream, period);
			transactions = parser.getTransactions();
		}
		catch (IOException ioe)
		{
			logger.log(Level.WARNING, "Error opening file", ioe);
			throw new ImportFileException("Unable to import from file, " + path);
		}
	}

}
