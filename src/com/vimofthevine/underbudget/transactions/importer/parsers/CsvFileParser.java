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
 * Parser for reading generic CSV files
 * 
 * Valid comma-separated value fields are:
 * date, description, origin description, amount, transaction type,
 * category, account name, labels, notes
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CsvFileParser implements ImportFileParser {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(CsvFileParser.class.getName());
	
	/**
	 * Regex pattern for splitting CSV fields/values
	 */
	protected static final String REGEX = ",(?=([^\"]*\"[^\"]*\")*[^\"]*$)";
	
	/**
	 * Enumeration for transaction fields
	 */
	protected enum TransactionField {
		NONE,
		DATE,
		VALUE,
		MEMO,
		PAYEE,
		WITHDRAWAL,
		DEPOSIT,
		TYPE;
	}

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
	 * Mapping of values to transaction fields
	 */
	protected TransactionField[] fields;
	
	/**
	 * Default constructor
	 */
	public CsvFileParser()
	{
		logger.log(Level.FINE, "Initializing CSV file parser");
		
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
			
			createFieldMapping(reader.readLine()); // First line is field descriptions
			
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
	 * Strips the given string of any containing quotation marks
	 * 
	 * @param string string to be stripped
	 * @return string stripped of containing quotation marks
	 */
	protected String strip(String string)
	{
		String tmp = string.trim().replaceFirst("^\"", "");
		tmp = tmp.replaceFirst("\"$", "");
		return tmp.replace("\"\"", "\"");
	}
	
	/**
	 * Creates the mapping between CSV values
	 * and transaction fields
	 * 
	 * @param definition the CSV line containing the field names
	 * @throws ImportFileException if any error occurs
	 */
	protected void createFieldMapping(String definition)
	{
		String[] fieldNames = definition.split(REGEX, -1);
		fields = new TransactionField[fieldNames.length];
		
		for (int i=0; i<fields.length; i++)
		{
			String field = strip(fieldNames[i]).toLowerCase();
			
			if (field.contains("date"))
				fields[i] = TransactionField.DATE;
			else if (field.contains("value") || field.contains("amount"))
				fields[i] = TransactionField.VALUE;
			else if (field.contains("memo") || field.contains("notes"))
				fields[i] = TransactionField.MEMO;
			else if (field.contains("payee") || field.startsWith("description"))
				fields[i] = TransactionField.PAYEE;
			else if (field.contains("category"))
				fields[i] = TransactionField.DEPOSIT;
			else if (field.contains("account"))
				fields[i] = TransactionField.WITHDRAWAL;
			else if (field.contains("type"))
				fields[i] = TransactionField.TYPE;
			else
				fields[i] = TransactionField.NONE;
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
		// Split into transaction fields
		String[] values = record.split(",(?=([^\"]*\"[^\"]*\")*[^\"]*$)", -1);
		
		if (values.length != fields.length)
			throw new ImportFileException("Invalid transaction record read");
		
		Transaction transaction = new Transaction();
		
		String type = "";
		
		for (int i=0; i<fields.length; i++)
		{
			String value = strip(values[i]);
			
			switch (fields[i])
			{
				case DATE:
		    		try
		    		{
		        		// Set the date
		        		SimpleDateFormat format = new SimpleDateFormat("M/dd/yyyy");
		        		transaction.date = format.parse(value);
		        		
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
		    		
		    		break;
				case PAYEE:
					transaction.payee = value;
					
					break;
				case MEMO:
					transaction.memo = value;
					
					break;
				case VALUE:
					transaction.value = new BigDecimal(value).abs();
					
					break;
				case TYPE:
					type = value;
					
					break;
				case DEPOSIT:
					if (type.equals("debit"))
						transaction.deposit = new Account(value);
					else
						transaction.withdrawal = new Account(value);
					
					break;
				case WITHDRAWAL:
					if (type.equals("debit"))
						transaction.withdrawal = new Account(value);
					else
						transaction.deposit = new Account(value);
					
					break;
			}
		}
		
		transactions.add(transaction);
	}

}
