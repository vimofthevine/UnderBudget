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

import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.BudgetDefinition;
import com.vimofthevine.underbudget.core.budget.MutableBudget;
import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.swing.widgets.SimpleDocument;

/**
 * Custom document model to display and apply
 * changes to a budget's name.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class NameModel extends SimpleDocument {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Budget
	 */
	private final Budget budget;
	
	/**
	 * Budget field change set
	 */
	private final HashMap<String, Object> changes;
	
	/**
	 * Constructs a new budget name document model.
	 * 
	 * @param bus    event bus
	 * @param budget budget
	 */
	NameModel(EventBus bus, Budget budget)
	{
		eventBus = bus;
		changes = new HashMap<String, Object>();
		
		this.budget = budget;
		
		setText(budget.getDefinition().getName());
	}
    
	@Override
	public void insertString(int offset, String string,
		AttributeSet attributes) throws BadLocationException
	{
		super.insertString(offset, string, attributes);
		update();
	}
	
	@Override
	public void remove(int offset, int length)
	throws BadLocationException
	{
		super.remove(offset, length);
		update();
	}
	
	private void update()
	{
		if ( ! (budget instanceof MutableBudget))
			return;
		else
		{
    		// Grab this while on EDT
    		final String newName = getText();
    		
    		// Then get off EDT
    		new Thread() {
    			public void run()
    			{
    				MutableBudget mutable = (MutableBudget) budget;
    				final BudgetDefinition old = mutable.getDefinition();
    				
    				if ( ! newName.equals(old.getName()))
    				{
    					mutable.setDefinition(new BudgetDefinition() {
                            public String getName() { return newName; }
                            public Currency getInitialBalance() { return old.getInitialBalance(); }
                            public BudgetingPeriod getPeriod() { return old.getPeriod(); }
    					});
    					
    					changes.put("name", newName);
    					eventBus.post(new BudgetModifiedEvent(budget, changes));
    				}
    			}
    		}.start();
		}
	}

}
