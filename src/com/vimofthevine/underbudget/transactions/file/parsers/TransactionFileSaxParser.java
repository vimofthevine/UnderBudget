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

package com.vimofthevine.underbudget.transactions.file.parsers;

import java.io.InputStream;
import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import com.vimofthevine.underbudget.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.transactions.Account;
import com.vimofthevine.underbudget.transactions.Transaction;
import com.vimofthevine.underbudget.transactions.file.TransactionFileException;
import com.vimofthevine.underbudget.transactions.importer.ImportFileException;
import com.vimofthevine.underbudget.transactions.importer.parsers.ProgressMonitorInputStream;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Transaction file parser using the SAX XML API
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionFileSaxParser extends DefaultHandler
implements TransactionFileParser {

	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(TransactionFileSaxParser.class.getName());

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
	 * The current transaction being parsed
	 */
	protected Transaction currentTransaction;

	/**
	 * The current element being parsed
	 */
	protected String currentElement;
	
	/**
	 * Calendar object
	 */
	protected Calendar calendar;

	/**
	 * Default constructor
	 */
	public TransactionFileSaxParser()
	{
		logger.log(Level.FINE, "Initializing transaction file parser");

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
			parse(stream);
		}
		catch (TransactionFileException tfe)
		{
			throw new ImportFileException(tfe.getMessage());
		}
	}

	@Override
	public void parse(InputStream stream) throws TransactionFileException
	{
		try
		{
			progress.reset();

			calendar = Calendar.getInstance();
			
			transactions = new ArrayList<Transaction>();

			SAXParserFactory factory = SAXParserFactory.newInstance();
			SAXParser parser = factory.newSAXParser();
			parser.parse(new ProgressMonitorInputStream(progress, stream), this);

			progress.complete();
		}
		catch (Exception e)
		{
			logger.log(Level.WARNING, "Exception parsing file", e);
			throw new TransactionFileException("Unable to read file");
		}
	}

	@Override
	public void startElement(String uri, String localName,
		String qName, Attributes attributes) throws SAXException
	{
		logger.log(Level.FINER, "Start element: " + qName);

		// Create new current transaction
		if (qName.equalsIgnoreCase("transaction"))
		{
			logger.log(Level.FINE, "Creating new transaction");
			currentTransaction = new Transaction();
		}
		else if (qName.equalsIgnoreCase("date-posted"))
		{
			logger.log(Level.FINE, "Creating new transaction date");
			calendar.clear();
		}
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
		
		// Add transaction to list and reset the current transaction
		if (qName.equalsIgnoreCase("transaction") && currentTransaction != null)
		{
			logger.log(Level.FINE, "Finished defining transaction");
			transactions.add(currentTransaction);
			currentTransaction = null;
		}
		else if (qName.equalsIgnoreCase("date-posted"))
		{
			logger.log(Level.FINE, "Finished defining transaction date");
			currentTransaction.date = calendar.getTime();
			
			if (period != null)
			{
				// Check if transaction is outside of date range
				if (currentTransaction.date.before(period.getStartDate()))
				{
					currentTransaction = null;
				}
				else if (currentTransaction.date.after(period.getEndDate()))
				{
					currentTransaction = null;
				}
			}
		}
		else if (qName.equals(currentElement))
		{
			currentElement = null;
		}
	}
	
	@Override
	public void characters(char ch[], int start, int length) throws SAXException
	{
		// Don't do anything if the current element is unknown/undefined
		if (currentElement == null || currentTransaction == null)
			return;
		
		String value = new String(ch, start, length);
		
		if (currentElement.equalsIgnoreCase("year"))
		{
			calendar.set(Calendar.YEAR, Integer.valueOf(value));
		}
		else if (currentElement.equalsIgnoreCase("month"))
		{
			calendar.set(Calendar.MONTH, Integer.valueOf(value));
		}
		else if (currentElement.equalsIgnoreCase("date"))
		{
			calendar.set(Calendar.DAY_OF_MONTH, Integer.valueOf(value));
		}
		else if (currentElement.equalsIgnoreCase("payee"))
		{
			currentTransaction.payee = value;
		}
		else if (currentElement.equalsIgnoreCase("memo"))
		{
			currentTransaction.memo = value;
		}
		else if (currentElement.equalsIgnoreCase("amount"))
		{
			currentTransaction.value = new BigDecimal(value);
		}
		else if (currentElement.equalsIgnoreCase("deposit-acct"))
		{
			currentTransaction.deposit = new Account(value);
		}
		else if (currentElement.equalsIgnoreCase("withdrawal-acct"))
		{
			currentTransaction.withdrawal = new Account(value);
		}
		
		// Reset current element tag
		currentElement = null;
	}

}
