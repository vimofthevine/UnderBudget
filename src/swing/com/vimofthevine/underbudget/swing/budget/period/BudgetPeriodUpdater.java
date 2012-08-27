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

package com.vimofthevine.underbudget.swing.budget.period;

import java.util.HashMap;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.BudgetDefinition;
import com.vimofthevine.underbudget.core.budget.MutableBudget;
import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.swing.budget.BudgetModifiedEvent;

/**
 * Convenience object to encapsulate common budgeting-
 * period updating logic.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class BudgetPeriodUpdater {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Budget being updated
	 */
	private final Budget budget;
	
	/**
	 * Budget field change set
	 */
	private final HashMap<String, Object> changes;
	
	/**
	 * Constructs a new budgeting period updater.
	 * 
	 * @param bus    event bus
	 * @param budget budget being updated
	 */
	BudgetPeriodUpdater(EventBus bus, Budget budget)
	{
		eventBus = bus;
		this.budget = budget;
		changes = new HashMap<String, Object>();
	}

	/**
	 * Updates the definition of the budget with
	 * a new budgeting period.
	 * 
	 * @param newPeriod new budgeting period
	 */
	void update(final BudgetingPeriod newPeriod)
	{
		if (budget instanceof MutableBudget)
		{
			MutableBudget mutable = (MutableBudget) budget;
			final BudgetDefinition old = mutable.getDefinition();
			
			if ( ! newPeriod.equals(old.getPeriod()))
			{
				mutable.setDefinition(new BudgetDefinition() {
                    public String getName() { return old.getName(); }
                    public Currency getInitialBalance() { return old.getInitialBalance(); }
                    public BudgetingPeriod getPeriod() { return newPeriod; }
					@Override
                    public CurrencyFactory getCurrency() { return old.getCurrency(); }
				});
				
				changes.put("period", newPeriod);
				eventBus.post(new BudgetModifiedEvent(budget, changes));
			}
		}
	}
}
