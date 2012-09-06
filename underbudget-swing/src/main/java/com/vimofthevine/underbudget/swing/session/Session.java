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

package com.vimofthevine.underbudget.swing.session;

import java.awt.Component;
import java.awt.Frame;
import java.util.Currency;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.assignment.DefaultTransactionAssigner;
import com.vimofthevine.underbudget.core.assignment.TransactionAssigner;
import com.vimofthevine.underbudget.core.balance.BalanceCalculator;
import com.vimofthevine.underbudget.core.balance.DefaultBalanceCalculator;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.source.BudgetSource;
import com.vimofthevine.underbudget.core.budget.source.BudgetSourceException;
import com.vimofthevine.underbudget.core.currency.CurrencyCalculator;
import com.vimofthevine.underbudget.stubs.FixedCalculator;
import com.vimofthevine.underbudget.swing.analysis.OnDemandBalanceCalculator;
import com.vimofthevine.underbudget.swing.assignment.OnDemandTransactionAssigner;
import com.vimofthevine.underbudget.swing.assignment.ReverseLookupAssignmentRules;
import com.vimofthevine.underbudget.swing.budget.OnDemandEditBudgetDialog;
import com.vimofthevine.underbudget.swing.preferences.UserPreferences;
import com.vimofthevine.underbudget.swing.session.content.SessionContentViewFactory;
import com.vimofthevine.underbudget.swing.session.wizard.BudgetSourceSelectionWizard;
import com.vimofthevine.underbudget.swing.transaction.OnDemandTransactionImporter;
import com.vimofthevine.underbudget.swing.transaction.wizard.TransactionSourceSelectionWizard;

/**
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class Session {
	
	/**
	 * Global event bus
	 */
	private final EventBus globalBus;

	/**
	 * Session event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Budget source
	 */
	private final BudgetSource budgetSource;
	
	/**
	 * Budget
	 */
	private final Budget budget;
	
	/**
	 * Session state
	 */
	private final SessionState state;
	
	/**
	 * Session view component
	 */
	private final Component component;
	
	/**
	 * Session active flag
	 */
	private boolean active;
	
	/**
	 * Constructs a new session with the given
	 * budget source.
	 * 
	 * @param window application window
	 * @param bus    global application event bus
	 * @param prefs  user preferences
	 * @param source budget source
	 */
	public Session(Frame window, EventBus bus,
		UserPreferences prefs, BudgetSource source)
	{
		try
		{
    		budgetSource = source;
    		budget = budgetSource.getBudget();
    		
    		globalBus = bus;
    		eventBus = new EventBus(source.toString());
    		
    		Currency currency = Currency.getInstance("USD");
    		CurrencyCalculator currencyCalculator = new FixedCalculator(currency);
    		ReverseLookupAssignmentRules rules =
    			new ReverseLookupAssignmentRules(budget.getAssignmentRules());
    		TransactionAssigner assigner = new DefaultTransactionAssigner(currencyCalculator);
    		BalanceCalculator calculator = new DefaultBalanceCalculator();
    		
    		state = new SessionState(globalBus, eventBus, budget);
    		new BudgetPersistenceModel(eventBus, budget, budgetSource, prefs);
    		
    		new BudgetSourceSelectionWizard(eventBus, window, budget);
    		new TransactionSourceSelectionWizard(eventBus, window, currency);
    		new OnDemandTransactionImporter(eventBus, budget);
    		new OnDemandTransactionAssigner(eventBus, rules, assigner);
    		new OnDemandBalanceCalculator(eventBus, budget, calculator, currencyCalculator);
    		new OnDemandEditBudgetDialog(eventBus, window, currency, budget);
    		
    		component = SessionContentViewFactory.build(
    			window, eventBus, currency, budget, rules,
    			currencyCalculator, prefs);
    		
    		active = false;
		}
		catch (BudgetSourceException bse)
		{
			throw new IllegalArgumentException("Could not create session", bse);
		}
	}
	
	void post(Object event)
	{
		eventBus.post(event);
	}
	
	/**
	 * Returns the name identifying this session.
	 * 
	 * @return session name
	 */
	public String getName()
	{
		return budget.getDefinition().getName();
	}
	
	/**
	 * Sets this session's active flag.
	 * 
	 * @param isActive <code>true</code> if this session is
	 *        active, else <code>false</code>
	 */
	void setActive(boolean isActive)
	{
		active = isActive;
	}
	
	/**
	 * Returns whether this session is the active session.
	 * 
	 * @return <code>true</code> if active, else <code>false</code>
	 */
	public boolean isActive()
	{
		return active;
	}
	
	/**
	 * Returns the view component for this session.
	 * 
	 * @return session view component
	 */
	public Component getComponent()
	{
		return component;
	}
	
	/**
	 * Checks if the session has any unsaved modifications.
	 * 
	 * @return <code>true</code> if the session is dirty,
	 *         else <code>false</code>
	 */
	public boolean isDirty()
	{
		return state.isDirty();
	}
	
}
