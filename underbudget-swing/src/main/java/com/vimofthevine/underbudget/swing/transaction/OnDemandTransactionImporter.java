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

package com.vimofthevine.underbudget.swing.transaction;

import java.util.logging.Level;
import java.util.logging.Logger;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.transaction.Transaction;
import com.vimofthevine.underbudget.core.transaction.source.TransactionImportException;
import com.vimofthevine.underbudget.core.transaction.source.TransactionSource;
import com.vimofthevine.underbudget.swing.status.ProgressEvent;
import com.vimofthevine.underbudget.swing.status.StatusMessageEvent;
import com.vimofthevine.underbudget.swing.transaction.events.ImportTransactionsEvent;
import com.vimofthevine.underbudget.swing.transaction.events.ImportTransactionsFromEvent;
import com.vimofthevine.underbudget.swing.transaction.events.SelectTransactionSourceEvent;
import com.vimofthevine.underbudget.swing.transaction.events.TransactionSourceSelectedEvent;
import com.vimofthevine.underbudget.swing.transaction.events.TransactionsImportedEvent;

/**
 * The <code>OnDemandTransactionImporter</code> performs
 * a transaction import, prompting the user for a
 * <code>TransactionSource</code> if no previous source
 * exists.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class OnDemandTransactionImporter {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(OnDemandTransactionImporter.class.getName());
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Budget
	 */
	private final Budget budget;
	
	/**
	 * Transaction source
	 */
	private TransactionSource source;
	
	/**
	 * Constructs a new transaction importer.
	 * 
	 * @param bus event bus
	 */
	public OnDemandTransactionImporter(EventBus bus, Budget budget)
	{
		eventBus = bus;
		eventBus.register(this);
		
		this.budget = budget;
	}
	
	@Subscribe
	public void sourceSelected(TransactionSourceSelectedEvent event)
	{
		logger.log(Level.FINE, "Transaction source selected");
		
		// Store transaction source
		source = event.getSource();
		
		// Perform import if source is valid (avoid infinite loops)
		if (source != null)
		{
			importTransactions(new ImportTransactionsEvent());
		}
	}
	
	@Subscribe
	public void importTransactions(ImportTransactionsEvent event)
	{
		// Do the import in a separate thread so we don't hold up the event bus
		new Thread()
		{
			{
				setName("Import transactions thread");
			}
			
			public void run()
			{
        		// If no source defined, prompt for source
        		if (source == null)
        		{
        			logger.log(Level.FINE, "No previous transaction source, prompting for source");
        			importTransactionsFrom(new ImportTransactionsFromEvent());
        		}
        		else
        		{
        			BudgetingPeriod period = budget.getDefinition().getPeriod();
        			
        			try
        			{
        				logger.log(Level.INFO, "Importing transactions from " + source);
        				eventBus.post(new StatusMessageEvent("Importing transactions from " + source));
        				eventBus.post(new ProgressEvent(true));
        				
        				Transaction[] transactions = source.getTransactions(period);
        				
        				eventBus.post(new ProgressEvent(false));
        				eventBus.post(new StatusMessageEvent(transactions.length
        					+ " transactions imported", 5000));
        				logger.log(Level.FINE, "Finished importing");
        				
                		if (transactions != null)
                		{
        					logger.log(Level.FINE, transactions.length + " transactions imported");
                			eventBus.post(new TransactionsImportedEvent(transactions));
                		}
        			}
        			catch (TransactionImportException tie)
        			{
        				logger.log(Level.WARNING, "Error importing transactions", tie);
        			}
        		}
			}
		}.start();
	}
	
	@Subscribe
	public void importTransactionsFrom(ImportTransactionsFromEvent event)
	{
		eventBus.post(new SelectTransactionSourceEvent());
	}

}
