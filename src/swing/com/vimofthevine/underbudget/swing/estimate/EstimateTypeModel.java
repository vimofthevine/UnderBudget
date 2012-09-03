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

import java.util.HashMap;

import javax.swing.DefaultComboBoxModel;
import javax.swing.SwingUtilities;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.date.SimpleDate;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.estimate.EstimateDefinition;
import com.vimofthevine.underbudget.core.estimate.EstimateType;
import com.vimofthevine.underbudget.core.estimate.MutableEstimate;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateModifiedEvent;

/**
 * Custom combo box model to display and apply
 * changes to an estimate's type.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class EstimateTypeModel extends DefaultComboBoxModel {
	
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
	 * Constructs a new estimate type model.
	 * 
	 * @param bus event bus
	 */
	EstimateTypeModel(EventBus bus)
	{
		super(EstimateType.values());
		eventBus = bus;
		changes = new HashMap<String,String>();
	}
	
	/**
	 * Updates the current estimate represented
	 * by the model.
	 * 
	 * @param newEstimate estimate represented by
	 *                     the model
	 */
	void setEstimate(Estimate newEstimate)
	{
		estimate = newEstimate;
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				setSelectedItem((estimate == null) ? null
					: estimate.getDefinition().getType(), false);
			}
		});
	}
	
	private void setSelectedItem(Object item, boolean post)
	{
		super.setSelectedItem(item);
	}
	
	@Override
	public void setSelectedItem(Object item)
	{
		if ( ! (estimate instanceof MutableEstimate))
			return;
		else
		{
    		super.setSelectedItem(item);
    		
    		if (item instanceof EstimateType)
    		{
    			final EstimateType type = (EstimateType) item;
    			
    			// Get off EDT
    			new Thread() {
    				public void run()
    				{
        				MutableEstimate mutable = (MutableEstimate) estimate;
        				final EstimateDefinition old = mutable.getDefinition();
        				
    					if ( ! type.equals(old.getType()))
    					{
        					mutable.setDefinition(new EstimateDefinition() {
                                public String getName() { return old.getName(); }
                                public String getDescription() { return old.getDescription(); }
                                public CashCommodity getAmount() { return old.getAmount(); }
                                public SimpleDate getDueDate() { return old.getDueDate(); }
                                public EstimateType getType() { return type; }
                                public boolean isComplete() { return old.isComplete(); }
        					});
        					
        					changes.put("type", type.toString());
        					eventBus.post(new EstimateModifiedEvent(estimate, changes));
    					}
    				}
    			}.start();
    		}
		}
	}

}
