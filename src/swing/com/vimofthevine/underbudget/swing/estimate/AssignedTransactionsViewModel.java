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

package com.vimofthevine.underbudget.swing.estimate;

import java.util.ArrayList;
import java.util.Arrays;

import javax.swing.DefaultListSelectionModel;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.table.TableModel;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.assignment.TransactionAssignments;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.transaction.Transaction;
import com.vimofthevine.underbudget.swing.assignment.ReverseLookupAssignmentRules;
import com.vimofthevine.underbudget.swing.assignment.events.TransactionsAssignedEvent;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateSelectedEvent;
import com.vimofthevine.underbudget.swing.transaction.AssociatedTransactionsViewModel;
import com.vimofthevine.underbudget.swing.transaction.TransactionsTableModel;

/**
 * Presentation model for views that display
 * a list of transactions assigned to the selected
 * estimate.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AssignedTransactionsViewModel implements
        AssociatedTransactionsViewModel {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Assignment rules
	 */
	private final ReverseLookupAssignmentRules rules;
	
	/**
	 * Transactions table model
	 */
	private final TransactionsTableModel tableModel;
	
	/**
	 * Transaction selection model
	 */
	private final ListSelectionModel selectionModel;
	
	/**
	 * Selected estimate
	 */
	private Estimate estimate;
	
	/**
	 * Transaction assignments
	 */
	private TransactionAssignments assignments;
	
	/**
	 * Constructs a new assigned-transactions view model.
	 * 
	 * @param bus   event bus
	 * @param rules assignment rules
	 */
	public AssignedTransactionsViewModel(EventBus bus,
		ReverseLookupAssignmentRules rules)
	{
		eventBus = bus;
		eventBus.register(this);
		
		this.rules = rules;
		
		tableModel = new TransactionsTableModel();
		
		selectionModel = new DefaultListSelectionModel();
		selectionModel.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
	}
	
	@Subscribe
	public void transactionsAssigned(TransactionsAssignedEvent event)
	{
		assignments = event.getAssignments();
		update();
	}
	
	@Subscribe
	public void estimateSelected(EstimateSelectedEvent event)
	{
		estimate = event.getEstimate();
		update();
	}
	
	private void update()
	{
		ArrayList<Transaction> transactions = new ArrayList<Transaction>();
		
		if (estimate != null && assignments != null)
		{
			AssignmentRule[] estimateRules = rules.getRules(estimate);
			
			for (AssignmentRule rule : estimateRules)
			{
				Transaction[] transactionList = assignments.getAssignedTransactions(rule);
				if (transactionList != null && transactionList.length > 0)
				{
					transactions.addAll(Arrays.asList(transactionList));
				}
			}
		}
		
		final Transaction[] transactionList =
			transactions.toArray(new Transaction[transactions.size()]);
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				selectionModel.clearSelection();
				tableModel.set(transactionList);
			}
		});
	}

	@Override
	public TableModel getTransactionsTableModel()
	{
		return tableModel;
	}

	@Override
	public ListSelectionModel getTransactionSelectionModel()
	{
		return selectionModel;
	}

}
