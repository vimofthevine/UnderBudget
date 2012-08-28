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

package com.vimofthevine.underbudget.swing.budget;

import java.util.HashMap;

import javax.swing.SwingUtilities;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.BudgetDefinition;
import com.vimofthevine.underbudget.core.budget.MutableBudget;
import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.swing.currency.CurrencyInputModel;

/**
 * Custom document model to display and apply
 * changes to an initial balance.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class InitialBalanceModel extends CurrencyInputModel {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Budget
	 */
	private final Budget budget;
	
	/**
	 * Budget change set
	 */
	private final HashMap<String, Object> changes;
	
	/**
	 * Constructs a new initial balance document model.
	 * 
	 * @param bus     event bus
	 * @param factory currency factory
	 * @param budget  budget being modified
	 */
	InitialBalanceModel(EventBus bus, CurrencyFactory factory,
		Budget budget)
	{
		super(factory);
		
		eventBus = bus;
		this.budget = budget;
		changes = new HashMap<String, Object>();
		
		final Currency amount = budget.getDefinition()
			.getInitialBalance();
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				setValue(amount);
			}
		});
	}

	/**
	 * Updates the initial balance according to
	 * the current text of the document.
	 */
	@Override
	public void setNewValue(final Currency newBalance)
	{
		if ( ! (budget instanceof MutableBudget))
			return;
		else
		{
			// Get off EDT
			new Thread() {
				public void run()
				{
					try
					{
						MutableBudget mutable = (MutableBudget) budget;
						final BudgetDefinition old = mutable.getDefinition();
						
						if ( ! newBalance.equals(old.getInitialBalance()))
						{
							mutable.setDefinition(new BudgetDefinition() {
                                public String getName() { return old.getName(); }
                                public Currency getInitialBalance() { return newBalance; }
                                public BudgetingPeriod getPeriod() { return old.getPeriod(); }
							});
							
							changes.put("amount", newBalance.formatAsString());
							eventBus.post(new BudgetModifiedEvent(budget, changes));
						}
					}
					catch (NumberFormatException nfe)
					{
						// Do nothing
						nfe.printStackTrace();
					}
				}
			}.start();
		}
	}

}
