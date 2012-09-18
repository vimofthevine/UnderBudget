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

import javax.swing.DefaultListSelectionModel;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.TableModel;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.assignment.TransactionAssignments;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.transaction.Transaction;
import com.vimofthevine.underbudget.swing.assignment.events.RuleSelectedEvent;
import com.vimofthevine.underbudget.swing.assignment.events.TransactionsAssignedEvent;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateSelectedEvent;
import com.vimofthevine.underbudget.swing.transaction.events.TransactionSelectedEvent;
import com.vimofthevine.underbudget.swing.transaction.events.TransactionsImportedEvent;

/**
 * Presentation model for views that display
 * a list of imported transactions.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ImportedTransactionListViewModel
implements ListSelectionListener {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(ImportedTransactionListViewModel.class.getName());
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Imported transactions table model
	 */
	private final TransactionsTableModel tableModel;
	
	/**
	 * Table selection model
	 */
	private final ListSelectionModel selectionModel;
	
	/**
	 * Current transactions
	 */
	private Transaction[] transactions;
	
	/**
	 * Currently selected transaction
	 */
	private Transaction selectedTransaction;
	
	/**
	 * Transaction assignments
	 */
	private TransactionAssignments assignments;

	/**
	 * Constructs a new imported transactions table view model.
	 * 
	 * @param bus event bus
	 */
	public ImportedTransactionListViewModel(EventBus bus)
	{
		eventBus = bus;
		eventBus.register(this);
		
		tableModel = new TransactionsTableModel();
		
		selectionModel = new DefaultListSelectionModel();
		selectionModel.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		selectionModel.addListSelectionListener(this);
		
		transactions = new Transaction[0];
	}
	/**
	 * Returns the table model representing
	 * the transaction data.
	 * 
	 * @return transactions table model
	 */
	TableModel getTableModel()
	{
		return tableModel;
	}
	
	/**
	 * Returns the table selection model.
	 * 
	 * @return imported transactions table selection model.
	 */
	ListSelectionModel getSelectionModel()
	{
		return selectionModel;
	}
	
	@Subscribe
	public synchronized void transactionsImported(final TransactionsImportedEvent event)
	{
		logger.log(Level.FINE, "Received new imported transactions list");
		
		transactions = event.getTransactions();
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				tableModel.set(transactions);
				selectionModel.clearSelection();
			}
		});
	}
	
	@Subscribe
	public synchronized void transactionAssigned(final TransactionsAssignedEvent event)
	{
		logger.log(Level.FINE, "Received new transaction assignments");
		assignments = event.getAssignments();
		
		if (selectedTransaction != null && assignments != null)
		{
			AssignmentRule rule = assignments.getAssigningRule(selectedTransaction);
			eventBus.post(new RuleSelectedEvent(rule));
			
			Estimate estimate = (rule == null) ? null
				: rule.getEstimate();
			eventBus.post(new EstimateSelectedEvent(estimate, null));
		}
	}
	
	@Override
    public synchronized void valueChanged(ListSelectionEvent arg0)
    {
		logger.log(Level.FINEST, "Transaction selection changed");
		
		if (selectionModel.isSelectionEmpty())
		{
			
		}
		else
		{
			// Grab this while on EDT
			int index = selectionModel.getLeadSelectionIndex();
			if (index >= 0 && index < transactions.length)
			{
				selectedTransaction = transactions[index];
				
				// Get off EDT
				new Thread() {
					public void run()
					{
						eventBus.post(new TransactionSelectedEvent(selectedTransaction));
						
						if (assignments != null)
						{
							AssignmentRule rule = assignments.getAssigningRule(selectedTransaction);
							
							logger.log(Level.FINEST, "Selecting assigning rule, " + rule);
							eventBus.post(new RuleSelectedEvent(rule));
							
                			Estimate estimate = (rule == null) ? null
                				: rule.getEstimate();
                			eventBus.post(new EstimateSelectedEvent(estimate, null));
						}
					}
				}.start();
			}
		}
    }
	
}
