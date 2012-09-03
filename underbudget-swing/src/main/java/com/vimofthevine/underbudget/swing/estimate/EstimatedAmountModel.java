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

package com.vimofthevine.underbudget.swing.estimate;

import java.util.Currency;
import java.util.HashMap;

import javax.swing.SwingUtilities;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.date.SimpleDate;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.estimate.EstimateDefinition;
import com.vimofthevine.underbudget.core.estimate.EstimateType;
import com.vimofthevine.underbudget.core.estimate.MutableEstimate;
import com.vimofthevine.underbudget.swing.currency.CurrencyInputModel;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateModifiedEvent;

/**
 * Custom document model to display and apply
 * changes to an estimate's amount.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class EstimatedAmountModel extends CurrencyInputModel {

	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Estimate field change set
	 */
	private final HashMap<String,String> changes;
	
	/**
	 * Currently represented estimate
	 */
	private Estimate estimate;
	
	/**
	 * Constructs a new estimate amount document model.
	 * 
	 * @param bus      event bus
	 * @param currency currency in use
	 */
	EstimatedAmountModel(EventBus bus, Currency currency)
	{
		super(currency);
		
		eventBus = bus;
		changes = new HashMap<String,String>();
	}
	
	/**
	 * Updates the current estimate represented
	 * by the document.
	 * 
	 * @param newEstimate estimate represented by
	 *                     the document
	 */
	void setEstimate(Estimate newEstimate)
	{
		estimate = newEstimate;
		final CashCommodity amount = (estimate == null) ? null
			: estimate.getDefinition().getAmount();
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				setValue(amount);
			}
		});
	}
	
	/**
	 * Updates the estimate's amount according to
	 * the current text of the document.
	 */
	@Override
	public void setNewValue(final CashCommodity currency)
	{
		if ( ! (estimate instanceof MutableEstimate))
			return;
		else
		{
    		// Get off EDT
    		new Thread() {
    			public void run()
    			{
    				try
    				{
        				MutableEstimate mutable = (MutableEstimate) estimate;
        				final EstimateDefinition old = mutable.getDefinition();
    					
    					if ( ! currency.equals(old.getAmount()))
    					{
        					mutable.setDefinition(new EstimateDefinition() {
                                public String getName() { return old.getName(); }
                                public String getDescription() { return old.getDescription(); }
                                public CashCommodity getAmount() { return currency; }
                                public SimpleDate getDueDate() { return old.getDueDate(); }
                                public EstimateType getType() { return old.getType(); }
                                public boolean isComplete() { return old.isComplete(); }
        					});
        					
            				changes.put("amount", currency.formatAsString());
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
	
}
