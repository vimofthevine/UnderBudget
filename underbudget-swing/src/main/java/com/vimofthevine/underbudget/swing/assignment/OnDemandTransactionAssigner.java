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

package com.vimofthevine.underbudget.swing.assignment;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.assignment.AssignmentRules;
import com.vimofthevine.underbudget.core.assignment.TransactionAssigner;
import com.vimofthevine.underbudget.core.assignment.TransactionAssignments;
import com.vimofthevine.underbudget.core.transaction.Transaction;
import com.vimofthevine.underbudget.swing.assignment.events.AssignTransactionsEvent;
import com.vimofthevine.underbudget.swing.assignment.events.TransactionsAssignedEvent;
import com.vimofthevine.underbudget.swing.status.ProgressEvent;
import com.vimofthevine.underbudget.swing.status.StatusMessageEvent;
import com.vimofthevine.underbudget.swing.transaction.events.ImportTransactionsEvent;
import com.vimofthevine.underbudget.swing.transaction.events.TransactionsImportedEvent;

/**
 * The <code>OnDemandTransactionAssigner</code> performs a
 * transaction assignment via a <code>TransactionAssigner</code>,
 * initiating a transaction import if no transactions have
 * yet to be imported.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class OnDemandTransactionAssigner {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Assignment rules
	 */
	private final AssignmentRules rules;
	
	/**
	 * Transaction assigner
	 */
	private final TransactionAssigner assigner;
	
	/**
	 * Imported transactions
	 */
	private Transaction[] transactions;
	
	/**
	 * Constructs a new transaction assigner
	 * 
	 * @param bus      event bus
	 * @param rules    assignment rules
	 * @param assigner transaction assigner
	 */
	public OnDemandTransactionAssigner(EventBus bus, AssignmentRules rules,
		TransactionAssigner assigner)
	{
		eventBus = bus;
		eventBus.register(this);
		
		this.rules = rules;
		this.assigner = assigner;
	}
	
	@Subscribe
	public void transactionsImported(TransactionsImportedEvent event)
	{
		// Store imported transactions
		transactions = event.getTransactions();
		
		// Perform assignment if there are transactions to be
		// assigned (avoid infinite loops)
		if (transactions != null && transactions.length > 0)
		{
			new Thread() {
				public void run()
				{
					assignTransactions(new AssignTransactionsEvent());
				}
			}.start();
		}
		// TODO send something out clearing previous assignments
	}
	
	/**
	 * Performs an assignment of the transactions that have
	 * been imported. If no transactions have been imported,
	 * an import transactions event is posted to initiate an
	 * import of transactions.
	 * 
	 * @param event assign transactions event
	 */
	@Subscribe
	public void assignTransactions(AssignTransactionsEvent event)
	{
		// Do in a separate thread so we don't hold up the event bus
		new Thread()
		{
			{
				setName("Assign transactions thread");
			}
			
			public void run()
			{
        		// If nothing to assign, perform an import
        		if (transactions == null || transactions.length == 0)
        		{
        			eventBus.post(new ImportTransactionsEvent());
        		}
        		else
        		{
        			eventBus.post(new StatusMessageEvent("Assigning imported transactions"));
        			eventBus.post(new ProgressEvent(true));
        			
        			TransactionAssignments assignments = assigner.assign(transactions, rules);
        			
        			eventBus.post(new ProgressEvent(false));
        			eventBus.post(new StatusMessageEvent("Transactions have been assigned", 5000));
        			
        			if (assignments != null)
        			{
        				eventBus.post(new TransactionsAssignedEvent(assignments));
        			}
        		}
			}
		}.start();
	}

}
