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

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.text.Document;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.swing.currency.CurrencyInputModel;

/**
 * Presentation model for views that display
 * modifiable details about a budget.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class EditBudgetViewModel {
	
	/**
	 * Budget name document model
	 */
	private final NameModel nameDocument;
	
	/**
	 * Initial balance document model
	 */
	private final InitialBalanceModel initialDocument;
	
	/**
	 * Close dialog action
	 */
	private final Action closeAction;
	
	/**
	 * Constructs a new edit-budget view model.
	 * 
	 * @param bus     event bus
	 * @param factory currency factory
	 * @param budget  budget
	 */
	EditBudgetViewModel(EventBus bus, CurrencyFactory factory,
		Budget budget)
	{
		nameDocument = new NameModel(bus, budget);
		initialDocument = new InitialBalanceModel(bus, factory, budget);
		
		closeAction = new AbstractAction() {
			{
				putValue(NAME, "Close");
			}
			
			@Override
			public void actionPerformed(ActionEvent event)
			{
				// Do nothing
			}
		};
	}

	/**
	 * Returns a document representing
	 * the budget's name.
	 * 
	 * @return budget name document
	 */
	Document getNameDocument()
	{
		return nameDocument;
	}
	
	/**
	 * Returns a document representing
	 * the budget's initial balance.
	 * 
	 * @return budget name document
	 */
	CurrencyInputModel getInitialBalanceDocument()
	{
		return initialDocument;
	}
	
	/**
	 * Returns an action for closing the
	 * budget modification dialog.
	 * 
	 * @return close dialog action
	 */
	public Action getCloseAction()
	{
		return closeAction;
	}
	
}
