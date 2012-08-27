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

import java.awt.Component;
import java.awt.Frame;

import javax.swing.SwingUtilities;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.swing.budget.period.BudgetingPeriodFormViewFactory;
import com.vimofthevine.underbudget.swing.session.content.SessionContent;
import com.vimofthevine.underbudget.swing.session.events.SessionContentEvent;

/**
 * The <code>OnDemandEditBudgetDialog</code> displays
 * a modal dialog with a form for modifying parameters
 * of the current budget.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class OnDemandEditBudgetDialog {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;

	/**
	 * Application window frame
	 */
	private final Frame parent;
	
	/**
	 * Currency factory
	 */
	private final CurrencyFactory currency;
	
	/**
	 * The current budget
	 */
	private final Budget budget;
	
	public OnDemandEditBudgetDialog(EventBus bus, Frame window,
		CurrencyFactory factory, Budget budget)
	{
		eventBus = bus;
		eventBus.register(this);
		
		parent = window;
		currency = factory;
		this.budget = budget;
	}
	
	@Subscribe
	public void showDialog(SessionContentEvent event)
	{
		if (event.getContent().equals(SessionContent.BUDGET))
		{
			SwingUtilities.invokeLater(new Runnable() {
				public void run()
				{
        			EditBudgetViewModel model =
        				new EditBudgetViewModel(eventBus, currency, budget);
        			Component period = BudgetingPeriodFormViewFactory.build(eventBus, budget);
        			EditBudgetView.display(parent, model, period);
				}
			});
		}
	}
		
}
