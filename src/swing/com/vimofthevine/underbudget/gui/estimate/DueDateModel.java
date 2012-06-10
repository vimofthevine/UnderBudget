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

import java.util.Date;
import java.util.HashMap;

import javax.swing.SwingUtilities;

import org.jdesktop.swingx.calendar.DefaultDateSelectionModel;
import org.jdesktop.swingx.event.DateSelectionEvent;
import org.jdesktop.swingx.event.DateSelectionListener;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.estimate.MutableEstimate;

/**
 * Custom date selection model to display and apply
 * changes to an estimate's due date.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class DueDateModel extends DefaultDateSelectionModel
implements DateSelectionListener {

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
	private MutableEstimate estimate;
	
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
	void setEstimate(MutableEstimate newEstimate)
	{
		estimate = newEstimate;
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				clearSelection();
				
				Date dueDate = estimate.getDueDate();
				if (dueDate != null)
				{
					setSelectionInterval(dueDate, dueDate);
				}
			}
		});
	}

	@Override
	public void valueChanged(DateSelectionEvent arg0)
	{
		// Grab this while on EDT
		final Date dueDate = getFirstSelectionDate();
		
		// Then get off EDT
		new Thread() {
			public void run()
			{
				if ( ! dueDate.equals(estimate.getDueDate()))
				{
    				estimate.setDueDate(dueDate);
    				changes.put("due-date", dueDate.toString());
    				eventBus.post(new EstimateModifiedEvent(estimate, changes));
				}
			}
		}.start();
	}

}
