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

package com.vimofthevine.underbudget.file.transaction;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.core.ledger.Account;
import com.vimofthevine.underbudget.core.ledger.transaction.Transaction;
import com.vimofthevine.underbudget.core.ledger.transaction.ImportedTransaction;
import com.vimofthevine.underbudget.core.progress.ProgressMonitorInputStream;
import com.vimofthevine.underbudget.core.progress.TaskProgress;
import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * The <code>CsvImporter</code> imports transactions from a
 * comma-separated-values file, dynamically determining the
 * appropriate columns.
 * 
 * Valid comma-separated value fields are:
 * date, description, origin description, amount, transaction type,
 * category, account name, labels, notes
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CsvImporter implements TransactionImporter {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(CsvImporter.class.getName());
	
	/**
	 * Currency factory
	 */
	private final CurrencyFactory currencyFactory;
	
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
	 * Constructs a new CSV file transaction importer.
	 * 
	 * @param factory currency factory
	 */
	public CsvImporter(CurrencyFactory factory)
	{
		currencyFactory = factory;
	}
	
	@Override
    public List<Transaction> read(InputStream stream, BudgetingPeriod period,
    	TaskProgress progress) throws TransactionImportException
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
			
			return transactions;
		}
		catch (IOException ioe)
		{
			throw new TransactionImportException("Unable to read file", ioe);
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
	throws TransactionImportException
	{
		// Split into transaction fields
		String[] values = record.split(",(?=([^\"]*\"[^\"]*\")*[^\"]*$)", -1);
		
		if (values.length != fields.length)
			throw new TransactionImportException("Invalid transaction record read");
		
		// Initialize fields
		SimpleDate date = new SimpleDate();
		String payee = "";
		String memo = "";
		Currency transferAmount = currencyFactory.newCurrencyInstance();
		Account deposit = new Account("");
		Account withdrawal = new Account("");
		
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
		        		date = new SimpleDate(format.parse(value));
		        		
		        		// Check if transaction is outside of date range
		        		if ( ! period.contains(date))
		        			return;
		    		}
		    		catch (ParseException pe)
		    		{
		    			logger.log(Level.WARNING, "Error parsing date field", pe);
		    			date = new SimpleDate();
		    		}
		    		
		    		break;
				case PAYEE:
					payee = value;
					
					break;
				case MEMO:
					memo = value;
					
					break;
				case VALUE:
					transferAmount = currencyFactory.newCurrencyInstance(value.replace("-", ""));
					
					break;
				case TYPE:
					type = value;
					
					break;
				case DEPOSIT:
					if (type.equals("debit"))
						deposit = new Account(value);
					else
						withdrawal = new Account(value);
					
					break;
				case WITHDRAWAL:
					if (type.equals("debit"))
						withdrawal = new Account(value);
					else
						deposit = new Account(value);
					
					break;
			}
		}
		
		transactions.add(new ImportedTransaction(date, payee, memo, transferAmount, withdrawal, deposit));
	}

}
