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

import java.awt.LayoutManager;
import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ComboBoxModel;
import javax.swing.InputVerifier;
import javax.swing.JFormattedTextField.AbstractFormatterFactory;
import javax.swing.text.Document;
import javax.swing.text.LayoutQueue;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.swing.currency.CurrencyFormatterFactory;
import com.vimofthevine.underbudget.swing.currency.CurrencyInputVerifier;

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
	 * Initial balance currency formatter factory
	 */
	private final CurrencyFormatterFactory currencyFormatter;
	
	/**
	 * Initial balance input verifier
	 */
	private final InputVerifier amountVerifier;
	
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
		
		currencyFormatter = new CurrencyFormatterFactory(factory);
		amountVerifier = new CurrencyInputVerifier(factory);
		
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
	Document getInitialBalanceDocument()
	{
		return initialDocument;
	}
	
	/**
	 * Returns a currency input formatter factory.
	 * 
	 * @return currency formatter factory
	 */
	AbstractFormatterFactory getInitialBalanceFormatter()
	{
		return currencyFormatter;
	}

	/**
	 * Returns an input verifier for the initial
	 * balance field.
	 * 
	 * @return initial balance input verifier
	 */
	InputVerifier getInitialBalanceInputVerifier()
	{
		return amountVerifier;
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
