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

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.estimate.Estimate;
import com.vimofthevine.underbudget.estimate.MutableEstimate;

/**
 * Action to create a new estimate as a child
 * of the selected estimate.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class AddChildAction extends AbstractAction {

	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Currently represented estimate
	 */
	private MutableEstimate estimate;
	
	/**
	 * Constructs a new add-child action.
	 * 
	 * @param bus event bus
	 */
	AddChildAction(EventBus bus)
	{
		eventBus = bus;
		
		putValue(NAME, "Add Child");
	}
	
	/**
	 * Updates the current estimate to be acted on
	 * by the action.
	 * 
	 * @param newEstimate estimate to be acted on
	 */
	void setEstimate(MutableEstimate newEstimate)
	{
		estimate = newEstimate;
	}
	
	@Override
	public void actionPerformed(ActionEvent event)
	{
		Estimate child = estimate.createChild();
		eventBus.post(new EstimateAddedEvent(estimate, child));
	}

}
