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

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.Commodity;
import com.vimofthevine.underbudget.core.date.DateRange;
import com.vimofthevine.underbudget.core.date.DateTime;
import com.vimofthevine.underbudget.core.date.SimpleDate;
import com.vimofthevine.underbudget.core.transaction.Transaction;
import com.vimofthevine.underbudget.core.transaction.TransferAccount;

/**
 * <code>TransactionSource</code> implementation, importing
 * transactions from a specified uncompressed GnuCash XML
 * file.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class GnuCashXmlFileSource extends DefaultHandler
implements TransactionSource {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(GnuCashXmlFileSource.class.getName());
	
	/**
	 * Description of this GnuCash XML file source
	 */
	private final String description;
	
	/**
	 * Input stream
	 */
	private final InputStream stream;
	
	/**
	 * Date range with which to filter imported transactions
	 */
	private DateRange dateRange;

	/**
	 * List of imported transaction records
	 */
	private final List<ImportedTransaction> transactions;
	
	/**
	 * Accounts read from the file
	 */
	private final HashMap<String,TransferAccount> accounts;
	
	/**
	 * Current currency used in the file
	 */
	private String currency;
	
	/**
	 * The current element being parsed
	 */
	private String currentElement;
	
	/**
	 * The current account being parsed
	 */
	private AccountStruct currentAccount;
	
	/**
	 * The current transaction being parsed
	 */
	private TransactionStruct currentTransaction;
	
	/**
	 * The current split being parsed
	 */
	private TransactionSplitStruct currentSplit;
	
	/**
	 * The current transaction's splits
	 */
	private List<TransactionSplitStruct> splits;
	
	/**
	 * Constructs a new GnuCash XML file model
	 * for a file with the given filename.
	 * 
	 * @param file GnuCash XML file
	 * @throws FileNotFoundException if the given
	 *         file does not exist
	 */
	public GnuCashXmlFileSource(File file)
	throws FileNotFoundException
	{
		stream = new FileInputStream(file);
		description = file.getAbsolutePath();
		logger.log(Level.INFO, "Reading GnuCash XML file, " + file);
		
		accounts = new HashMap<String,TransferAccount>();
		transactions = new ArrayList<ImportedTransaction>();
	}
	
	/**
	 * Constructs a new GnuCash XML file model
	 * to read from the given input stream.
	 * 
	 * @param inStream input stream to be read
	 */
	public GnuCashXmlFileSource(InputStream inStream)
	{
		stream = inStream;
		description = "input stream";
		
		accounts = new HashMap<String,TransferAccount>();
		transactions = new ArrayList<ImportedTransaction>();
	}

	@Override
    public Transaction[] getTransactions(DateRange range)
	throws TransactionImportException
    {
		accounts.clear();
		transactions.clear();
		
		dateRange = range;
		
		try
		{
			SAXParserFactory factory = SAXParserFactory.newInstance();
			SAXParser parser = factory.newSAXParser();
			parser.parse(stream, this);
			
			Collections.sort(transactions);
		
			return transactions.toArray(new Transaction[transactions.size()]);
		}
		catch (SAXException saxe)
		{
			throw new TransactionImportException("Unable to import transactions", saxe);
		}
		catch (IOException ioe)
		{
			throw new TransactionImportException("Unable to read file", ioe);
		}
		catch (ParserConfigurationException pce)
        {
			throw new TransactionImportException("Unable to import transactions", pce);
        }
    }
	
	@Override
	public void startElement(String uri, String localName,
		String qName, Attributes attributes) throws SAXException
	{
		logger.log(Level.FINER, "Start element: " + qName);
		
		// Create new current account
		if (qName.equalsIgnoreCase("gnc:account"))
		{
			logger.log(Level.FINE, "Creating new account");
			currentAccount = new AccountStruct();
		}
		// Create new current transaction
		else if (qName.equalsIgnoreCase("gnc:transaction"))
		{
			logger.log(Level.FINE, "Creating new transaction");
			currentTransaction = new TransactionStruct();
			splits = new ArrayList<TransactionSplitStruct>();
		}
		// Create new current split
		else if (qName.equalsIgnoreCase("trn:split"))
		{
			logger.log(Level.FINE, "Creating new split");
			currentSplit = new TransactionSplitStruct();
		}
		// Only store transaction posted dates (ignore entered dates)
		else if (qName.equalsIgnoreCase("ts:date"))
		{
			if (currentElement.equalsIgnoreCase("trn:date-posted"))
			{
				currentElement = qName;
			}
		}
		// Store current element tag name
		else
		{
			currentElement = qName;
		}
	}
	
	@Override
	public void endElement(String uri, String localName,
		String qName) throws SAXException
	{
		logger.log(Level.FINER, "End element: " + qName);
		
		// Add account to list and reset the current account
		if (qName.equalsIgnoreCase("gnc:account") && currentAccount != null)
		{
			logger.log(Level.FINE, "Finished defining account");
			accounts.put(currentAccount.id, new ImportedTransferAccount(currentAccount.name, currentAccount.parent));
			currentAccount = null;
		}
		// Create individual transactions and add to list
		else if (qName.equalsIgnoreCase("gnc:transaction") && currentTransaction != null)
		{
			logger.log(Level.FINE, "Finished defining transaction");
			convertSplitsToTransactions(getMasterSplit(splits));
			currentTransaction = null;
			splits = null;
		}
		// Add split to current transaction's list
		else if (qName.equalsIgnoreCase("trn:split") && currentSplit != null
			&& currentTransaction != null && splits != null)
		{
			logger.log(Level.FINE, "Finished defining split");
			splits.add(currentSplit);
			currentSplit = null;
		}
	}
	
	@Override
	public void characters(char ch[], int start, int length) throws SAXException
	{
		// Don't do anything if the current element is unknown/undefined
		if (currentElement == null)
			return;
		
		if (currentElement.equals("cmdty:id"))
		{
			currency = new String(ch, start, length);
		}
		
		if (currentElement.startsWith("act"))
			handleAccountValue(new String(ch, start, length));
		
		if (currentElement.startsWith("trn") || currentElement.startsWith("ts"))
			handleTransactionValue(new String(ch, start, length));
		
		if (currentElement.startsWith("split"))
			handleSplitValue(new String(ch, start, length));
	}
	
	/**
	 * Populates account fields according to
	 * the current element with the given value
	 * 
	 * @param value XML element value
	 */
	protected void handleAccountValue(String value)
	{
		// Don't do anything if there is no current account
		if (currentAccount == null)
			return;
		
		// Read the account ID
		if (currentElement.equalsIgnoreCase("act:id"))
		{
			currentAccount.id = value;
		}
		// Read the account name
		else if (currentElement.equalsIgnoreCase("act:name"))
		{
			currentAccount.name = value;
		}
		// Read the account type
		else if (currentElement.equalsIgnoreCase("act:type"))
		{
			currentAccount.type = value;
		}
		// Read the parent account ID
		else if (currentElement.equalsIgnoreCase("act:parent"))
		{
			// If not the root account, get the parent account
			if ( ! currentAccount.type.equalsIgnoreCase("root"))
			{
				TransferAccount parent = accounts.get(value);
				
				// If parent is unknown, do not add account
				if (parent == null)
				{
					logger.log(Level.SEVERE, "Parent account not found for "
						+ currentAccount.name);
					currentAccount = null;
				}
				else
				{
					currentAccount.parent = parent;
				}
			}
		}
		
		// Reset current element tag
		currentElement = "";
	}
	
	/**
	 * Populates transaction fields according to
	 * the current element with the given value
	 * 
	 * @param value XML element value
	 */
	protected void handleTransactionValue(String value)
	{
		// Don't do anything if there is no current transaction
		if (currentTransaction == null)
			return;
		
		// Read the transaction date
		else if (currentElement.equalsIgnoreCase("trn:date-posted"))
		{
			// Skip over so the ts:date element is read
			return;
		}
		else if (currentElement.equalsIgnoreCase("ts:date"))
		{
			try
			{
				SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
				currentTransaction.date = new DateTime(format.parse(value));
				
				// Check if transaction is outside of date range
				if ((dateRange != null)
					&& ( ! dateRange.contains(currentTransaction.date)))
				{
					currentTransaction = null;
				}
			}
			catch (ParseException pe)
			{
				logger.log(Level.WARNING, "Error parsing date field", pe);
				currentTransaction.date = new DateTime();
			}
		}
		// Read the transaction description/payee
		else if (currentElement.equalsIgnoreCase("trn:description"))
		{
			currentTransaction.payee = value;
		}
		
		// Reset current element tag
		currentElement = "";
	}
	
	/**
	 * Populates split fields according to
	 * the current element with the given value
	 * 
	 * @param value XML element value
	 */
	protected void handleSplitValue(String value)
	{
		// Don't do anything if there is no current split
		if (currentSplit == null)
			return;
		
		// Read the split amount
		else if (currentElement.equalsIgnoreCase("split:value"))
		{
			int slashIndex     = value.indexOf("/");
			Float wholeValue   = Float.parseFloat(value.substring(0, slashIndex));
			Float divisor      = Float.parseFloat(value.substring(slashIndex + 1));
			Float splitValue   = wholeValue / divisor;
			currentSplit.value = Commodity.create(currency, String.valueOf(splitValue));
		}
		// Read the split memo
		else if (currentElement.equalsIgnoreCase("split:memo"))
		{
			currentSplit.memo = value;
		}
		// Read the split account
		else if (currentElement.equalsIgnoreCase("split:account"))
		{
			TransferAccount account = accounts.get(value);
			
			// If account is unknown, do not add split
			if (account == null)
			{
				logger.log(Level.SEVERE, "Account not found for split "
					+ currentSplit.memo);
				currentSplit = null;
			}
			else
			{
				currentSplit.account = account;
			}
		}
		
		// Reset current element tag
		currentElement = "";
	}
	
	/**
	 * Determines the master split from a given list of
	 * transaction splits. The master split is the single
	 * withdrawal, or deposit, account from which the other
	 * splits obtain their transfer amounts.
	 * 
	 * @param splits list of transaction splits
	 * @return the master transaction split
	 */
	protected TransactionSplitStruct getMasterSplit(List<TransactionSplitStruct> splits)
	{
		TransactionSplitStruct masterSplit = null;
		boolean lookingForMasterWithdrawal = true;
		
		// Continue until master split is found
		while (masterSplit == null)
		{
			for (TransactionSplitStruct split : splits)
			{
				// Check for a negative amount (or the inverse)
				if ((lookingForMasterWithdrawal && split.value.getValue().isNegative())
					|| ( ! lookingForMasterWithdrawal && ! split.value.getValue().isNegative()))
				{
					// If the master split has not yet been detected
					if (masterSplit == null)
					{
						// Found master split
						masterSplit = split;
					}
					else
					{
						// If we've already looked for the master withdrawal
						if (lookingForMasterWithdrawal)
						{
							// Reset master and look for a master deposit
							masterSplit = null;
							lookingForMasterWithdrawal = false;
						}
						else
						{
							// Cannot handle multiple deposits and withdrawals in a single transaction
							logger.log(Level.SEVERE, "Multiple deposits and withdrawals found in a single transaction");
							return null;
						}
					}
				}
			}
		}
		
		return masterSplit;
	}
	
	/**
	 * Converts the list of transaction splits to individual
	 * transaction records
	 * 
	 * @param master master transaction split
	 */
	protected void convertSplitsToTransactions(TransactionSplitStruct master)
	{
		boolean masterIsWithdrawal = master.value.getValue().isNegative();
		TransferAccount masterAccount = master.account;
		
		for (TransactionSplitStruct split : splits)
		{
			if (split != master)
			{
				TransferAccount splitAccount = split.account;
				TransferAccount withdrawalAccount = masterIsWithdrawal
					? masterAccount : splitAccount;
				TransferAccount depositAccount = masterIsWithdrawal
					? splitAccount : masterAccount;
				
				// Create new transaction
				transactions.add(new ImportedTransaction(
					currentTransaction.date,
					currentTransaction.payee,
					split.memo,
					split.value,
					withdrawalAccount,
					depositAccount
				));
			}
		}
	}
	
	@Override
	public String toString()
	{
		return "GnuCash XML file " + description;
	}

	/**
	 * Struct to hold transaction values.
	 */
	private class TransactionStruct {
		public SimpleDate date;
		public String payee = "";
	}
	
	/**
	 * Struct to hold transaction split values.
	 */
	private class TransactionSplitStruct {
		public CashCommodity value;
		public String memo = "";
		public TransferAccount account;
	}
	
	/**
	 * Struct to hold transfer account values.
	 */
	private class AccountStruct {
		public String id;
		public String name;
		public String type;
		public TransferAccount parent;
	}

}
