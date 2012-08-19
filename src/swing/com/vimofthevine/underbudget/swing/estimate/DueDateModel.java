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
import java.util.HashMap;

import javax.swing.SwingUtilities;

import org.jdesktop.swingx.calendar.DefaultDateSelectionModel;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.estimate.EstimateDefinition;
import com.vimofthevine.underbudget.core.estimate.EstimateType;
import com.vimofthevine.underbudget.core.estimate.MutableEstimate;
import com.vimofthevine.underbudget.core.util.SimpleDate;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateModifiedEvent;

/**
 * Custom date selection model to display and apply
 * changes to an estimate's due date.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class DueDateModel extends DefaultDateSelectionModel {

	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Estimate field change set
	 */
	private final HashMap<String,String> changes;
	
	/** * Currently represented estimate
	 */
	private Estimate estimate;
	
	/**
	 * Constructs a new estimate due date selection model.
	 * 
	 * @param bus event bus
	 */
	DueDateModel(EventBus bus)
	{
		eventBus = bus;
		changes = new HashMap<String,String>();
		setSelectionMode(SelectionMode.SINGLE_SELECTION);
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
				clearSelection(false);
				
				SimpleDate dueDate = (estimate == null) ? null
					: estimate.getDefinition().getDueDate();
				
				if (dueDate != null)
				{
					Date date = dueDate.getTime();
					setSelectionInterval(date, date, false);
				}
			}
		});
	}
	
	private void clearSelection(boolean post)
	{
		super.clearSelection();
	}
	
	private void setSelectionInterval(Date startDate, Date endDate, boolean post)
	{
		super.setSelectionInterval(startDate, endDate);
	}
	
	@Override
	public void clearSelection()
	{
		if ( ! (estimate instanceof MutableEstimate))
			return;
		else
		{
			super.clearSelection();
			update();
		}
	}
	
	@Override
	public void setSelectionInterval(Date startDate, Date endDate)
	{
		if ( ! (estimate instanceof MutableEstimate))
			return;
		else
		{
			super.setSelectionInterval(startDate, endDate);
			update();
		}
	}
	
	private void update()
	{
		if (estimate instanceof MutableEstimate)
		{
    		// Grab this while on EDT
    		final Date dueDate = getFirstSelectionDate();
    		
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
    						? null : new SimpleDate() {
                                public int compareTo(SimpleDate o) { return 0; }
                                public boolean before(SimpleDate when) { return false; }
                                public boolean after(SimpleDate when) { return false; }
                                public String formatAsString() { return dueDate.toString(); }
                                public Date getTime() { return dueDate; }
                            };
    					
    					mutable.setDefinition(new EstimateDefinition() {
                            public String getName() { return old.getName(); }
                            public String getDescription() { return old.getDescription(); }
                            public Currency getAmount() { return old.getAmount(); }
                            public SimpleDate getDueDate() { return newDate; }
                            public EstimateType getType() { return old.getType(); }
                            public boolean isComplete() { return old.isComplete(); }
    					});
        					
        				changes.put("due-date", dueDate.toString());
        				eventBus.post(new EstimateModifiedEvent(estimate, changes));
    				}
    			}
    		}.start();
		}
	}

}
