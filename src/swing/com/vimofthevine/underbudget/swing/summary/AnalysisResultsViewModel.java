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

package com.vimofthevine.underbudget.swing.summary;

import javax.swing.SwingUtilities;
import javax.swing.table.TableModel;
import javax.swing.text.Document;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.BudgetDefinition;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.swing.analysis.BalancesCalculatedEvent;
import com.vimofthevine.underbudget.swing.budget.BudgetModifiedEvent;
import com.vimofthevine.underbudget.swing.widgets.SimpleDocument;

/**
 * Presentation model for views that display detailed
 * analysis results.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class AnalysisResultsViewModel {
	
	/**
	 * Budget name document model
	 */
	private final SimpleDocument nameDocument;
	
	/**
	 * Budgeting period document model
	 */
	private final SimpleDocument periodDocument;
	
	/**
	 * Ending balances table model
	 */
	private final EndingBalancesTableModel balancesTableModel;
	
	/**
	 * Constructs a new analysis results view model.
	 * 
	 * @param bus     event bus
	 * @param factory currency factory
	 */
	public AnalysisResultsViewModel(EventBus bus,
		CurrencyFactory factory, Budget budget)
	{
		bus.register(this);
		
		BudgetDefinition definition = budget.getDefinition();
		
		nameDocument = new SimpleDocument();
		nameDocument.setText(definition.getName());
		periodDocument = new SimpleDocument();
		periodDocument.setText(definition.getPeriod().getDescription());
		balancesTableModel = new EndingBalancesTableModel(factory);
	}
	
	/**
	 * Updates the displayed budget parameters when
	 * the budget has been modified.
	 * 
	 * @param event budget modified event
	 */
	@Subscribe
	public void budgetModified(BudgetModifiedEvent event)
	{
		BudgetDefinition definition = event.getBudget().getDefinition();
		
		final String name = definition.getName();
		final String period = definition.getPeriod().getDescription();
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				nameDocument.setText(name);
				periodDocument.setText(period);
			}
		});
	}
	
	/**
	 * Updates the ending balances table when the balances
	 * have been re-calculated.
	 * 
	 * @param event balances calculated event
	 */
	@Subscribe
	public void balancesCalculated(BalancesCalculatedEvent event)
	{
		balancesTableModel.setResults(event.getEndingBalances());
	}
	
	/**
	 * Returns a document model representing
	 * the name of the analyzed budget.
	 * 
	 * @return budget name document model
	 */
	Document getBudgetNameModel()
	{
		return nameDocument;
	}
	
	/**
	 * Returns a document model representing
	 * the budgeting period analyzed.
	 * 
	 * @return budgeting period document model
	 */
	Document getBudgetingPeriodModel()
	{
		return periodDocument;
	}
	
	/**
	 * Returns a table model representing the
	 * ending balances.
	 * 
	 * @return ending balances table model
	 */
	public TableModel getEndingBalancesTableModel()
	{
		return balancesTableModel;
	}
	
}
