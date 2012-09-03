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

import javax.swing.DefaultListSelectionModel;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.table.TableModel;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.assignment.TransactionAssignments;
import com.vimofthevine.underbudget.core.transaction.Transaction;
import com.vimofthevine.underbudget.swing.assignment.events.RuleSelectedEvent;
import com.vimofthevine.underbudget.swing.assignment.events.TransactionsAssignedEvent;
import com.vimofthevine.underbudget.swing.transaction.AssociatedTransactionsViewModel;
import com.vimofthevine.underbudget.swing.transaction.TransactionsTableModel;

/**
 * Presentation model for views that display
 * a list of transactions assigned by the
 * selected rule.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class QualifiedTransactionsViewModel implements
        AssociatedTransactionsViewModel {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Transactions table model
	 */
	private final TransactionsTableModel tableModel;
	
	/**
	 * Transaction selection model
	 */
	private final ListSelectionModel selectionModel;
	
	/**
	 * Selected assignment rule
	 */
	private AssignmentRule rule;
	
	/**
	 * Transaction assignments
	 */
	private TransactionAssignments assignments;
	
	/**
	 * Constructs a new qualified-transactions view model.
	 * 
	 * @param bus event bus
	 */
	public QualifiedTransactionsViewModel(EventBus bus)
	{
		eventBus = bus;
		eventBus.register(this);
		
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
	public void ruleSelected(RuleSelectedEvent event)
	{
		rule = event.getAssignmentRule();
		update();
	}
	
	private void update()
	{
		if (rule == null || assignments == null)
		{
			SwingUtilities.invokeLater(new Runnable() {
				public void run()
				{
					selectionModel.clearSelection();
					tableModel.set(new Transaction[0]);
				}
			});
		}
		else
		{
			Transaction[] qualified = assignments.getAssignedTransactions(rule);
			
			final Transaction[] transactions = (qualified == null)
				? new Transaction[0] : qualified;
			
			SwingUtilities.invokeLater(new Runnable() {
				public void run()
				{
					selectionModel.clearSelection();
					tableModel.set(transactions);
				}
			});
		}
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
