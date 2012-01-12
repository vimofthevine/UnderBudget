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

package com.vimofthevine.underbudget.transactions.importer.parsers;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.math.BigDecimal;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.vimofthevine.underbudget.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.transactions.Account;
import com.vimofthevine.underbudget.transactions.Transaction;
import com.vimofthevine.underbudget.transactions.importer.ImportFileException;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Parser for reading Mint CSV files
 * 
 * The comma-separated value fields are:
 * date, description, origin description, amount, transaction type,
 * category, account name, labels, notes
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class MintCsvFileParser implements ImportFileParser {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(MintCsvFileParser.class.getName());

	/**
	 * Task progress
	 */
	protected TaskProgress progress;
	
	/**
	 * Budgeting period within which to import transactions
	 */
	protected BudgetingPeriod period;

	/**
	 * List of imported transaction records
	 */
	protected List<Transaction> transactions;
	
	/**
	 * Default constructor
	 */
	public MintCsvFileParser()
	{
		logger.log(Level.FINE, "Initializing Mint CSV file parser");
		
		progress = new TaskProgress();
	}
	
	@Override
    public TaskProgress getProgress()
    {
		return progress;
    }
	
	@Override
    public List<Transaction> getTransactions()
    {
		return transactions;
    }
	
	@Override
    public void parse(InputStream stream, BudgetingPeriod period)
            throws ImportFileException
    {
		this.period = period;
		
		try
		{
			progress.reset();
			
			transactions = new ArrayList<Transaction>();
			
			BufferedReader reader = new BufferedReader(new InputStreamReader(
				new ProgressMonitorInputStream(progress, stream)));
			
			reader.readLine(); // First line is field descriptions
			
			String line;
			while ((line = reader.readLine()) != null)
			{
				readTransactionRecord(line);
			}
			
			progress.complete();
		}
		catch (Exception e)
		{
			if (e instanceof ImportFileException)
				throw (ImportFileException) e;
			else
			{
				logger.log(Level.WARNING, "Exception parsing file", e);
				throw new ImportFileException("Unable to read file");
			}
		}
    }
	
	/**
	 * Parses a given CSV record as a transaction
	 * 
	 * @param record CSV transaction record
	 * @throws ImportFileException if any error occurs
	 */
	protected void readTransactionRecord(String record)
	throws ImportFileException
	{
		// Make sure empty fields get picked up
		record = record.replace("\"\"", "\" \"");
		
		// Strip off first and last quotation marks
		record = record.substring(1, record.length() - 1);
		
		// Split into transaction fields
		String[] values = record.split("\",\"");
		
		if (values.length != 9)
			throw new ImportFileException("Invalid transaction record read");
		
		Transaction transaction = new Transaction();
		
		try
		{
    		// Set the date
    		SimpleDateFormat format = new SimpleDateFormat("M/dd/yyyy");
    		transaction.date = format.parse(values[0]);
    		
    		// Check if transaction is outside of date range
    		if (transaction.date.before(period.getStartDate()))
    			return;
    		else if (transaction.date.after(period.getEndDate()))
    			return;
		}
		catch (ParseException pe)
		{
			logger.log(Level.WARNING, "Error parsing date field", pe);
			transaction.date = new Date();
		}
		
		// Set all other values
		transaction.payee = values[1].trim();
		transaction.value = new BigDecimal(values[3]);
		
		// If a debit transaction
		if (values[4].equals("debit"))
		{
			transaction.deposit    = new Account(values[5]); // category
			transaction.withdrawal = new Account(values[6]); // account
		}
		// Else a credit transaction
		else
		{
			transaction.withdrawal = new Account(values[5]); // category
			transaction.deposit    = new Account(values[6]); // account
		}
		
		transaction.memo = values[8].trim();
		
		transactions.add(transaction);
	}

}
