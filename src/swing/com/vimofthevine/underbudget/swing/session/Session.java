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

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.assignment.DefaultTransactionAssigner;
import com.vimofthevine.underbudget.core.assignment.TransactionAssigner;
import com.vimofthevine.underbudget.core.balance.BalanceCalculator;
import com.vimofthevine.underbudget.core.balance.DefaultBalanceCalculator;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.source.BudgetSource;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.stubs.actuals.StubActualFigures;
import com.vimofthevine.underbudget.stubs.currency.StubCurrencyFactory;
import com.vimofthevine.underbudget.swing.analysis.OnDemandBalanceCalculator;
import com.vimofthevine.underbudget.swing.assignment.OnDemandTransactionAssigner;
import com.vimofthevine.underbudget.swing.assignment.ReverseLookupAssignmentRules;
import com.vimofthevine.underbudget.swing.budget.OnDemandEditBudgetDialog;
import com.vimofthevine.underbudget.swing.session.content.SessionContentViewFactory;
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
	 * @param source budget source
	 */
	public Session(Frame window, EventBus bus, BudgetSource source)
	{
		globalBus = bus;
		eventBus = new EventBus(source.toString());
		
		budgetSource = source;
		Budget budget = budgetSource.getBudget();
		CurrencyFactory factory = new StubCurrencyFactory();
		ReverseLookupAssignmentRules rules =
			new ReverseLookupAssignmentRules(budget.getAssignmentRules());
		TransactionAssigner assigner = new DefaultTransactionAssigner(factory);
		BalanceCalculator calculator = new DefaultBalanceCalculator();
		
		state = new SessionState(globalBus, eventBus, budget);
		new BudgetPersistenceModel(eventBus, budgetSource);
		
		new TransactionSourceSelectionWizard(eventBus, window, factory);
		new OnDemandTransactionImporter(eventBus, budget);
		new OnDemandTransactionAssigner(eventBus, rules, assigner);
		new OnDemandBalanceCalculator(eventBus, budget, calculator);
		new OnDemandEditBudgetDialog(eventBus, window, factory, budget);
		
		component = SessionContentViewFactory.build(
			window, eventBus, factory, budget, new StubActualFigures(), rules);
		
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
		return budgetSource.getBudget().getDefinition().getName();
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
