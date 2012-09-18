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

import java.util.Date;
import java.util.Map;

import javax.swing.SwingUtilities;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.date.DateTime;
import com.vimofthevine.underbudget.core.date.SimpleDate;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.estimate.EstimateDefinition;
import com.vimofthevine.underbudget.core.estimate.EstimateField;
import com.vimofthevine.underbudget.core.estimate.EstimateType;
import com.vimofthevine.underbudget.core.estimate.MutableEstimate;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateModifiedEvent;
import com.vimofthevine.underbudget.swing.widgets.DateInputModel;

/**
 * Custom date selection model to display and apply
 * changes to an estimate's due date.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class DueDateModel extends DateInputModel {

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
	 * Constructs a new estimate due date selection model.
	 * 
	 * @param bus event bus
	 */
	DueDateModel(EventBus bus)
	{
		super();
		eventBus = bus;
	}
	
	/**
	 * Updates the current estimate represented
	 * by the model.
	 * 
	 * @param newEstimate estimate represented by
	 *                    the model
	 */
	void setEstimate(Estimate newEstimate)
	{
		estimate = newEstimate;
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				clear();
				
				SimpleDate dueDate = (estimate == null) ? null
					: estimate.getDefinition().getDueDate();
				
				if (dueDate != null)
				{
					Date date = dueDate.getTime();
					setDate(date);
				}
				
				setEnabled((estimate == null) ? false
					: ! estimate.getDefinition().getType()
						.equals(EstimateType.CATEGORY));
			}
		});
	}
	
	@Override
	public void dateSelectionChanged()
	{
		if (estimate instanceof MutableEstimate)
		{
    		// Grab this while on EDT
    		final Date dueDate = getDate();
    		
    		// Then get off EDT
    		new Thread() {
    			public void run()
    			{
    				MutableEstimate mutable = (MutableEstimate) estimate;
    				final EstimateDefinition old = mutable.getDefinition();
    				
    				if (dueDate == null && old.getDueDate() == null)
    					return;
        				
    				if ((dueDate == null && old.getDueDate() != null)
    					|| (dueDate != null && old.getDueDate() == null)
    					|| ! dueDate.equals(old.getDueDate().getTime()))
    				{
    					final SimpleDate newDate = (dueDate == null)
    						? null : new DateTime(dueDate);
    					
    					changes = mutable.setDefinition(new EstimateDefinition() {
                            public String getName() { return old.getName(); }
                            public String getDescription() { return old.getDescription(); }
                            public CashCommodity getAmount() { return old.getAmount(); }
                            public SimpleDate getDueDate() { return newDate; }
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
