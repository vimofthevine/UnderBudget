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

package com.vimofthevine.underbudget.core.transaction.source;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.Commodity;
import com.vimofthevine.underbudget.core.date.DateRange;
import com.vimofthevine.underbudget.core.date.DateTime;
import com.vimofthevine.underbudget.core.date.SimpleDate;
import com.vimofthevine.underbudget.core.transaction.Transaction;
import com.vimofthevine.underbudget.core.transaction.source.csv.CsvProfile;
import com.vimofthevine.underbudget.core.transaction.source.csv.Field;

/**
 * <code>TransactionSource</code> implementation, importing
 * transactions from a specified comma-separated-value file
 * with a given CSV profile.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CsvFileSource implements TransactionSource {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(CsvFileSource.class.getName());
	
	/**
	 * Regex pattern for splitting CSV fields/values
	 */
	private static final String REGEX = ",(?=([^\"]*\"[^\"]*\")*[^\"]*$)";
	
	/**
	 * CSV file
	 */
	private final File file;
	
	/**
	 * CSV profile
	 */
	private final CsvProfile profile;

	/**
	 * List of imported transaction records
	 */
	private final List<ImportedTransaction> transactions;
	
	/**
	 * Current currency used in the file
	 */
	private final String currency;
	
	/**
	 * Date format instance
	 */
	private final DateFormat dateFormat;
	
	/**
	 * Constructs a new CSV file model for
	 * a file with the given name and CSV
	 * profile.
	 * 
	 * @param file    CSV file
	 * @param profile CSV profile
	 * @throws FileNotFoundException if the given
	 *         file does not exist
	 */
	public CsvFileSource(File file, CsvProfile profile)
	throws FileNotFoundException
	{
		logger.log(Level.FINER, "Creating CSV file source, " + file);
		
		if ( ! file.exists())
			throw new FileNotFoundException();
		
		this.file = file;
		this.profile = profile;
		
		currency = profile.getCurrencyCode();
		dateFormat = new SimpleDateFormat(profile.getDateFormat());
		transactions = new ArrayList<ImportedTransaction>();
	}

	@Override
	public Transaction[] getTransactions(DateRange range)
	throws TransactionImportException
	{
		transactions.clear();
		
		try
		{
			BufferedReader reader = new BufferedReader(
				new InputStreamReader(new FileInputStream(file)));
			
			if (profile.firstLineContainsHeaders())
			{
				reader.readLine();
			}
			
			ImportedTransaction transaction;
			String entryLine;
			while ((entryLine = reader.readLine()) != null)
			{
				transaction = readTransactionRecord(entryLine);
				
				if (transaction == null)
					continue;
				
				if (range == null
					|| range.contains(transaction.getDefinition().getPostedDate()))
				{
					transactions.add(transaction);
				}
			}
			
			reader.close();
			
			return transactions.toArray(new Transaction[transactions.size()]);
		}
		catch (IOException ioe)
		{
			throw new TransactionImportException("Unable to import transactions", ioe);
		}
	}
	
	/**
	 * Parses a given CSV record as a transaction.
	 * 
	 * @param record CSV transaction record
	 * @return the imported transaction
	 */
	private ImportedTransaction readTransactionRecord(final String record)
	{
		// Split into transaction fields
		String[] values = record.split(REGEX, -1);
		
		if (values.length != profile.getColumnCount())
			return null;
		
		SimpleDate date = null;
		String payee = null;
		String memo = null;
		CashCommodity amount = null;
		ImportedTransferAccount withdrawal = null;
		ImportedTransferAccount deposit = null;
		String type = null;
		ImportedTransferAccount noAccount = new ImportedTransferAccount("", null, "");
		
		for (int i=0; i<values.length; i++)
		{
			final String value = strip(values[i]);
			final Field field = profile.getFieldForColumn(i);
			
			switch (field)
			{
				case DATE:
					try
					{
						date = new DateTime(dateFormat.parse(value));
					}
					catch (ParseException pe)
					{
						logger.log(Level.WARNING, "Error parsing date field", pe);
						date = new DateTime();
					}
					break;
					
				case PAYEE:
					payee = value;
					break;
					
				case MEMO:
					memo = value;
					break;
					
				case WITHDRAWAL:
					if (type != null && type.equalsIgnoreCase("debit"))
					{
						withdrawal = new ImportedTransferAccount(value, null, "");
					}
					else
					{
						deposit = new ImportedTransferAccount(value, null, "");
					}
					break;
					
				case DEPOSIT:
					if (type != null && type.equalsIgnoreCase("debit"))
					{
						deposit = new ImportedTransferAccount(value, null, "");
					}
					else
					{
						withdrawal = new ImportedTransferAccount(value, null, "");
					}
					break;
					
				case AMOUNT:
					amount = Commodity.create(currency, value);
					break;
					
				case TYPE:
					type = value;
					
					// Just in case the accounts have already been processed
					if (deposit != null && withdrawal != null
						&& type.equalsIgnoreCase("debit"))
					{
						// Swap the accounts when a debit transaction
						ImportedTransferAccount tmp = deposit;
						deposit = withdrawal;
						withdrawal = tmp;
					}
					break;
					
				default:
					// Do nothing
			}
		}
		
		return new ImportedTransaction(
			(date != null) ? date : new DateTime(),
			(payee != null) ? payee : "",
			(memo != null) ? memo : "",
			(amount != null) ? amount : Commodity.zero(currency),
			(withdrawal != null) ? withdrawal : noAccount,
			(deposit != null) ? deposit : noAccount
		);
	}
	
	/**
	 * Strips the given string of any containing quotation marks.
	 * 
	 * @param string string to be stripped
	 * @return string stripped of containing quotation marks
	 */
	private String strip(String string)
	{
		String tmp = string.trim().replaceFirst("^\"", "");
		tmp = tmp.replaceFirst("\"$", "");
		return tmp.replace("\"\"", "\"");
	}

}
