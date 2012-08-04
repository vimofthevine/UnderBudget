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

import java.awt.Component;
import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.estimate.MutableEstimate;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateRemovedEvent;

/**
 * Action to delete the selected estimate.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class DeleteAction extends AbstractAction {

	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Parent component
	 */
	private final Component parent;
	
	/**
	 * Currently represented estimate
	 */
	private MutableEstimate estimate;
	
	/**
	 * Constructs a new delete action.
	 * 
	 * @param bus       event bus
	 * @param component parent component
	 */
	DeleteAction(EventBus bus, Component component)
	{
		eventBus = bus;
		parent = component;
		
		putValue(NAME, "Delete");
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
		
		if (estimate.isRoot())
		{
			setEnabled(false);
		}
		else
		{
			setEnabled(estimate.getChildCount() == 0);
		}
	}
	
	@Override
	public void actionPerformed(ActionEvent e)
	{
		if (estimate.getChildCount() != 0)
			return;
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				int result = JOptionPane.showConfirmDialog(parent,
					"Delete " + estimate.getDefinition().getName() + "?",
					"Delete Estimate",
					JOptionPane.OK_CANCEL_OPTION);
				
				if (result == JOptionPane.OK_OPTION)
				{
					// Get off EDT
					new Thread() {
						public void run()
						{
        					Estimate parent = estimate.getParent();
        					int index = parent.indexOf(estimate);
        					estimate.delete();
        					eventBus.post(new EstimateRemovedEvent(parent, index, estimate));
						}
					}.start();
				}
			}
		});
	}

}
