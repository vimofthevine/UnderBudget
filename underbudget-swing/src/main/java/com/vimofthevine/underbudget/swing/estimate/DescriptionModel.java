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

import java.util.Map;

import javax.swing.SwingUtilities;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.date.SimpleDate;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.estimate.EstimateDefinition;
import com.vimofthevine.underbudget.core.estimate.EstimateField;
import com.vimofthevine.underbudget.core.estimate.EstimateType;
import com.vimofthevine.underbudget.core.estimate.MutableEstimate;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateModifiedEvent;
import com.vimofthevine.underbudget.swing.widgets.TextInputModel;

/**
 * Custom input model to display and apply
 * changes to an estimate's description.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class DescriptionModel extends TextInputModel {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Estimate field change set
	 */
	private Map<EstimateField,Object> changes;
	
	/**
	 * Currently represented estimate
	 */
	private Estimate estimate;
	
	/**
	 * Constructs a new estimate description document model.
	 * 
	 * @param bus event bus
	 */
	DescriptionModel(EventBus bus)
	{
		eventBus = bus;
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
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				String text = (estimate == null) ? ""
					: estimate.getDefinition().getDescription();
				setText(text);
				
				setEnabled((estimate instanceof MutableEstimate)
					&& ! estimate.isRoot());
			}
		});
	}
    
	/**
	 * Updates the estimate's description according to
	 * the current text of the document.
	 */
	@Override
	public void fieldChanged()
	{
		if ( ! (estimate instanceof MutableEstimate))
			return;
		else
		{
    		// Grab this while on EDT
    		final String description = getText();
    		
    		// Then get off EDT
    		new Thread() {
    			public void run()
    			{
    				MutableEstimate mutable = (MutableEstimate) estimate;
    				final EstimateDefinition old = mutable.getDefinition();
    				
    				if ( ! description.equals(old.getDescription()))
    				{
    					changes = mutable.setDefinition(new EstimateDefinition() {
                            public String getName() { return old.getName(); }
                            public String getDescription() { return description; }
                            public CashCommodity getAmount() { return old.getAmount(); }
                            public SimpleDate getDueDate() { return old.getDueDate(); }
                            public EstimateType getType() { return old.getType(); }
                            public boolean isComplete() { return old.isComplete(); }
    					});
    					
        				eventBus.post(new EstimateModifiedEvent(estimate, changes));
    				}
    			}
    		}.start();
    	}
	}

}
