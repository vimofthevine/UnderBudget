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
import com.vimofthevine.underbudget.core.estimate.EstimateType;
import com.vimofthevine.underbudget.core.estimate.MutableEstimate;

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
	private MutableEstimate estimate;

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
	void setEstimate(MutableEstimate newEstimate)
	{
		estimate = newEstimate;
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				setSelectedItem(estimate.getType(), false);
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
		super.setSelectedItem(item);
		
		if (item instanceof EstimateType)
		{
			final EstimateType type = (EstimateType) item;
			
			// Get off EDT
			new Thread() {
				public void run()
				{
					if ( ! type.equals(estimate.getType()))
					{
    					estimate.setType(type);
    					changes.put("type", type.toString());
    					eventBus.post(new EstimateModifiedEvent(estimate, changes));
					}
				}
			}.start();
		}
	}

}
