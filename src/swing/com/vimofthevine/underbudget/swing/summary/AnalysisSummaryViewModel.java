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

import java.util.HashMap;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.SwingUtilities;
import javax.swing.text.Document;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.balance.EndingBalances;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.BudgetDefinition;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.swing.analysis.BalancesCalculatedEvent;
import com.vimofthevine.underbudget.swing.budget.BudgetModifiedEvent;
import com.vimofthevine.underbudget.swing.widgets.SimpleDocument;

/**
 * Presentation model for views that display budget
 * analysis results.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class AnalysisSummaryViewModel {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(AnalysisSummaryViewModel.class.getName());
	
	/**
	 * Analysis result field enumeration
	 */
	private enum Field {
		BUDGET_NAME,
		INITIAL_BALANCE,
		ESTIMATED_ENDING_BALANCE,
		ACTUAL_ENDING_BALANCE,
		EXPECTED_ENDING_BALANCE,
	};
	
	/**
	 * Document models
	 */
	private final Map<Field,SimpleDocument> documents;
	
	/**
	 * Constructs a new analysis results view model.
	 * 
	 * @param bus     event bus
	 * @param budget  budget
	 * @param factory currency factory
	 */
	public AnalysisSummaryViewModel(EventBus bus, Budget budget,
		CurrencyFactory factory)
	{
		bus.register(this);
		
		documents = new HashMap<Field,SimpleDocument>();
		
		for (Field field : Field.values())
		{
			documents.put(field, new SimpleDocument());
		}
		
		documents.get(Field.BUDGET_NAME).setText(
			budget.getDefinition().getName());
		documents.get(Field.INITIAL_BALANCE).setText(
			budget.getDefinition().getInitialBalance().formatAsString());
		documents.get(Field.ESTIMATED_ENDING_BALANCE).setText(
			factory.newCurrencyInstance().formatAsString());
		documents.get(Field.ACTUAL_ENDING_BALANCE).setText(
			factory.newCurrencyInstance().formatAsString());
		documents.get(Field.EXPECTED_ENDING_BALANCE).setText(
			factory.newCurrencyInstance().formatAsString());
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
		final String initial = definition.getInitialBalance().formatAsString();
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				documents.get(Field.BUDGET_NAME).setText(name);
				documents.get(Field.INITIAL_BALANCE).setText(initial);
			}
		});
	}
	
	/**
	 * Updates the displayed ending balances when
	 * the balances have been recalculated.
	 * 
	 * @param event balances-calculated event
	 */
	@Subscribe
	public void balancesCalculated(BalancesCalculatedEvent event)
	{
		logger.log(Level.INFO, "New ending balances received");
		
		EndingBalances balances = event.getEndingBalances();
		final String estimated = balances.getEstimatedEndingBalance()
			.getValue().formatAsString();
		final String actual = balances.getActualEndingBalance()
			.getValue().formatAsString();
		final String expected = balances.getExpectedEndingBalance()
			.getValue().formatAsString();
		
		logger.log(Level.INFO, estimated);
		logger.log(Level.INFO, actual);
		logger.log(Level.INFO, expected);
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				documents.get(Field.ESTIMATED_ENDING_BALANCE)
					.setText(estimated);
				documents.get(Field.ACTUAL_ENDING_BALANCE)
					.setText(actual);
				documents.get(Field.EXPECTED_ENDING_BALANCE)
					.setText(expected);
			}
		});
	}

	/**
	 * Returns a document representing
	 * the budget's name.
	 * 
	 * @return budget name document
	 */
	public Document getBudgetNameDocument()
	{
		return documents.get(Field.BUDGET_NAME);
	}

	/**
	 * Returns a document representing
	 * the budget's initial balance.
	 * 
	 * @return budget initial balance document
	 */
	public Document getInitialBalanceDocument()
	{
		return documents.get(Field.INITIAL_BALANCE);
	}

	/**
	 * Returns a document representing
	 * the estimated ending balance
	 * 
	 * @return estimated ending balance document
	 */
	public Document getEstimatedEndingBalanceDocument()
	{
		return documents.get(Field.ESTIMATED_ENDING_BALANCE);
	}

	/**
	 * Returns a document representing
	 * the actual ending balance
	 * 
	 * @return actual ending balance document
	 */
	public Document getActualEndingBalanceDocument()
	{
		return documents.get(Field.ACTUAL_ENDING_BALANCE);
	}

	/**
	 * Returns a document representing
	 * the expected ending balance
	 * 
	 * @return expected ending balance document
	 */
	public Document getExpectedEndingBalanceDocument()
	{
		return documents.get(Field.EXPECTED_ENDING_BALANCE);
	}

}
