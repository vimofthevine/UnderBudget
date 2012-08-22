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

package com.vimofthevine.underbudget.swing.analysis;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.assignment.ActualFigures;
import com.vimofthevine.underbudget.core.balance.BalanceCalculator;
import com.vimofthevine.underbudget.core.balance.EndingBalances;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.swing.assignment.events.AssignTransactionsEvent;
import com.vimofthevine.underbudget.swing.assignment.events.TransactionsAssignedEvent;

/**
 * The <code>OnDemandBalanceCalculator</code> performs
 * a balance calculation via a <code>BalanceCalculator</code>,
 * initiating a transaction assignment if no assignments
 * have yet to be made.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class OnDemandBalanceCalculator {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Budget
	 */
	private final Budget budget;
	
	/**
	 * Balance calculator
	 */
	private final BalanceCalculator calculator;
	
	/**
	 * Actual figures
	 */
	private ActualFigures actuals;
	
	/**
	 * Constructs a new balance calculator.
	 * 
	 * @param bus        event bus
	 * @param budget     budget
	 * @param calculator balance calculator
	 */
	public OnDemandBalanceCalculator(EventBus bus,
		Budget budget, BalanceCalculator calculator)
	{
		eventBus = bus;
		eventBus.register(this);
		
		this.budget = budget;
		this.calculator = calculator;
	}
	
	@Subscribe
	public void transactionsAssigned(TransactionsAssignedEvent event)
	{
		// Store transaction assignments if they provide a valid actuals source
		if (event.getAssignments() instanceof ActualFigures)
		{
			actuals = (ActualFigures) event.getAssignments();
		}
		
		// Perform calculation if there are valid actual figures
		// (avoid infinite loops)
		if (actuals != null)
		{
			new Thread() {
				public void run()
				{
					calculateBalances(new CalculateBalancesEvent());
				}
			}.start();
		}
	}
	
	/**
	 * Performs a balance calculation, using the already supplied
	 * actual figures. If no actual figures have been supplied, an
	 * assign transactions event is posted to initiate an assignment
	 * of the imported transactions.
	 * 
	 * @param event calculate ending balances event
	 */
	@Subscribe
	public void calculateBalances(CalculateBalancesEvent event)
	{
		// If no actuals, perform an assignment
		if (actuals == null)
		{
			eventBus.post(new AssignTransactionsEvent());
		}
		else
		{
			EndingBalances balances = calculator.calculate(
				budget.getDefinition().getInitialBalance(),
				budget.getRootEstimate(), actuals);
			
			if (balances != null)
			{
				eventBus.post(new BalancesCalculatedEvent(balances));
			}
		}
	}

}
