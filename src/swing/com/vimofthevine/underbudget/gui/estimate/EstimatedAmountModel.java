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

package com.vimofthevine.underbudget.gui.estimate;

import java.util.HashMap;

import javax.swing.SwingUtilities;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.currency.Currency;
import com.vimofthevine.underbudget.currency.CurrencyFactory;
import com.vimofthevine.underbudget.estimate.MutableEstimate;
import com.vimofthevine.underbudget.gui.currency.CommittableDocument;
import com.vimofthevine.underbudget.gui.widgets.SimpleDocument;

/**
 * Custom document model to display and apply
 * changes to an estimate's amount.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class EstimatedAmountModel extends SimpleDocument
implements CommittableDocument {

	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Currency factory
	 */
	private final CurrencyFactory factory;
	
	/**
	 * Estimate field change set
	 */
	private final HashMap<String,String> changes;
	
	/**
	 * Currently represented estimate
	 */
	private MutableEstimate estimate;
	
	/**
	 * Constructs a new estimate amount document model.
	 * 
	 * @param bus event bus
	 */
	EstimatedAmountModel(EventBus bus, CurrencyFactory factory)
	{
		eventBus = bus;
		this.factory = factory;
		changes = new HashMap<String,String>();
	}
	
	/**
	 * Updates the current estimate represented
	 * by the document.
	 * 
	 * @param newEstimate estimate represented by
	 *                     the document
	 */
	void setEstimate(MutableEstimate newEstimate)
	{
		estimate = newEstimate;
		final String amount = estimate.getAmount().formatAsString();
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				setText(amount);
			}
		});
	}
	
	@Override
	public void commit()
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				update();
			}
		});
	}

	/**
	 * Updates the estimate's amount according to
	 * the current text of the document.
	 */
	private void update()
	{
		// Grab this while on EDT
		final String amount = getText();
		
		// Then get off EDT
		new Thread() {
			public void run()
			{
				try
				{
					Currency currency = factory.newCurrencyInstance(amount);
					
					if ( ! currency.equals(estimate.getAmount()))
					{
    					estimate.setAmount(currency);
        				changes.put("amount", amount);
        				eventBus.post(new EstimateModifiedEvent(estimate, changes));
					}
				}
				catch (NumberFormatException nfe)
				{
					// Do nothing
				}
			}
		}.start();
	}
	
}
