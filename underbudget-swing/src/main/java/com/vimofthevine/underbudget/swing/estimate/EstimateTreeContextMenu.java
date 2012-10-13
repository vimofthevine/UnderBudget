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

import javax.swing.Action;
import javax.swing.SwingUtilities;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.swing.assignment.ReverseLookupAssignmentRules;

/**
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class EstimateTreeContextMenu {
	
	/**
	 * Add-child-to-root action
	 */
	private final AddChildAction addChildToRootAction;
	
	/**
	 * Add-child-to-selected action
	 */
	private final AddChildAction addChildAction;
	
	/**
	 * Delete-selected action
	 */
	private final DeleteAction deleteAction;
	
	/**
	 * Constructs a new estimate tree context menu.
	 * 
	 * @param bus    event bus
	 * @param root   root estimate
	 * @param rules  assignment rules (used when deleting estimates)
	 * @param parent parent component (usually the main window)
	 */
	EstimateTreeContextMenu(EventBus bus, Estimate root,
		ReverseLookupAssignmentRules rules, Component parent)
	{
		addChildToRootAction = new AddChildAction(bus);
		addChildToRootAction.putValue(Action.NAME, "Create top-level estimate");
		addChildToRootAction.setEstimate(root);
		
		addChildAction = new AddChildAction(bus);
		addChildAction.setEnabled(false);
		deleteAction = new DeleteAction(bus, parent, rules);
		deleteAction.setEnabled(false);
	}
	
	/**
	 * Updates the context menu to reflect the currently
	 * selected estimates.
	 * 
	 * @param estimate currently selected estimate
	 */
	void setEstimate(final Estimate estimate)
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				addChildAction.setEstimate(estimate);
				deleteAction.setEstimate(estimate);
			}
		});
	}
	
	/**
	 * Returns the action model for adding new
	 * estimates to the root estimate.
	 * 
	 * @return add-child-to-root action model
	 */
	Action getAddChildToRootAction()
	{
		return addChildToRootAction;
	}
	
	/**
	 * Returns the action model for adding new
	 * estimates to the selected estimate.
	 * 
	 * @return add-child-to-selected action model
	 */
	Action getAddChildToSelectedAction()
	{
		return addChildAction;
	}
	
	/**
	 * Returns the action model for deleting
	 * the selected estimate.
	 * 
	 * @return delete-selected action model
	 */
	Action getDeleteSelectedAction()
	{
		return deleteAction;
	}

}
