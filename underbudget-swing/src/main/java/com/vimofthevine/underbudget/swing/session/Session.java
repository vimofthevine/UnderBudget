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
import com.vimofthevine.underbudget.core.currency.FixedCalculator;
import com.vimofthevine.underbudget.swing.analysis.OnDemandBalanceCalculator;
import com.vimofthevine.underbudget.swing.assignment.OnDemandTransactionAssigner;
import com.vimofthevine.underbudget.swing.assignment.ReverseLookupAssignmentRules;
import com.vimofthevine.underbudget.swing.budget.OnDemandEditBudgetDialog;
import com.vimofthevine.underbudget.swing.preferences.UserPreferences;
import com.vimofthevine.underbudget.swing.session.content.SessionContentViewFactory;
import com.vimofthevine.underbudget.swing.transaction.OnDemandTransactionImporter;
import com.vimofthevine.underbudget.swing.transaction.wizard.TransactionSourceSelectionWizard;

/**
 * A session represents a single budget opened by the
 * application.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class Session {
	
	/**
	 * Session event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Budget
	 */
	private final Budget budget;
	
	/**
	 * Budget persistence model
	 */
	private final BudgetPersistenceModel persistence;
	
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
	 * @param window    application window
	 * @param globalBus global application event bus
	 * @param prefs     user preferences
	 * @param source    budget source
	 * @throws BudgetSourceException if the budget could not be
	 *         retrieved from the given budget source
	 */
	public Session(Frame window, EventBus globalBus,
		UserPreferences prefs, BudgetSource source)
	throws BudgetSourceException
	{
		budget = source.retrieve();
		
		eventBus = new EventBus(source.toString());
		
		Currency currency = Currency.getInstance("USD");
		CurrencyCalculator currencyCalculator = new FixedCalculator(currency);
		ReverseLookupAssignmentRules rules =
			new ReverseLookupAssignmentRules(budget.getAssignmentRules());
		TransactionAssigner assigner = new DefaultTransactionAssigner(currencyCalculator);
		BalanceCalculator calculator = new DefaultBalanceCalculator();
		
		state = new SessionState(globalBus, eventBus, budget);
		persistence = new BudgetPersistenceModel(eventBus, budget, source, prefs, window);
		
		new BudgetSourceRequestBridge(globalBus, eventBus);
		new TransactionSourceSelectionWizard(eventBus, window, currency, prefs);
		new OnDemandTransactionImporter(eventBus, budget);
		new OnDemandTransactionAssigner(eventBus, rules, assigner);
		new OnDemandBalanceCalculator(eventBus, budget, calculator, currencyCalculator);
		new OnDemandEditBudgetDialog(eventBus, window, currency, budget);
		
		component = SessionContentViewFactory.build(
			window, eventBus, currency, budget, rules,
			currencyCalculator, prefs);
		
		active = false;
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
	
	/**
	 * Closes the session, relinquishing any resources.
	 */
	void close()
	{
		persistence.close();
	}
	
}
