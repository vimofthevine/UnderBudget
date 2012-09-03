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

package com.vimofthevine.underbudget.swing.assignment;

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.assignment.AssignmentRules;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.swing.assignment.events.RuleAddedEvent;

/**
 * Action to create a new rule for a specific
 * estimate.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class CreateRuleAction extends AbstractAction {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Assignment rules list
	 */
	private final AssignmentRules rules;
	
	/**
	 * Currently selected estimate
	 */
	private Estimate estimate;
	
	/**
	 * Constructs a new create-rule action.
	 * 
	 * @param bus      event bus
	 * @param ruleList assignment rules list
	 */
	CreateRuleAction(EventBus bus, AssignmentRules ruleList)
	{
		eventBus = bus;
		rules = ruleList;
		
		putValue(NAME, "Create Rule");
		setEnabled(false);
	}
	
	/**
	 * Updates the current estimate to be used
	 * by this action.
	 * 
	 * @param estimate estimate to be acted with
	 */
	void setEstimate(Estimate estimate)
	{
		this.estimate = estimate;
		
		if (estimate == null)
		{
			setEnabled(false);
		}
		else
		{
			// Only allow rules on leaf estimates
			setEnabled(estimate.getChildCount() == 0);
		}
	}

	@Override
	public void actionPerformed(ActionEvent arg0)
	{
		if (estimate == null)
			return;
		
		// Get off EDT
		new Thread() {
			public void run()
			{
        		int index = rules.size();
        		AssignmentRule rule = new UserAssignmentRule(estimate);
        		rules.append(rule);
        		eventBus.post(new RuleAddedEvent(rule, index));
			}
		}.start();
	}

}
