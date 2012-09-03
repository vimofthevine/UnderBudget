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

import java.awt.Component;
import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.JOptionPane;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.assignment.AssignmentRules;
import com.vimofthevine.underbudget.core.assignment.MutableAssignmentRule;
import com.vimofthevine.underbudget.swing.assignment.events.RuleRemovedEvent;

/**
 * Action to delete the selected assignment rule.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class DeleteRuleAction extends AbstractAction {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Parent component
	 */
	private final Component parent;
	
	/**
	 * Assignment rules list
	 */
	private final AssignmentRules rules;
	
	/**
	 * Currently represented assignment rule
	 */
	private MutableAssignmentRule rule;
	
	/**
	 * Constructs a new delete-rule action.
	 * 
	 * @param bus       event bus
	 * @param component parent Swing component
	 * @param ruleList  assignment rules list
	 */
	DeleteRuleAction(EventBus bus, Component component,
		AssignmentRules ruleList)
	{
		eventBus = bus;
		parent = component;
		rules = ruleList;
		
		putValue(NAME, "Delete");
	}
	
	/**
	 * Updates the current rule to be acted on
	 * by the action.
	 * 
	 * @param newRule rule to be acted on
	 */
	void setRule(AssignmentRule newRule)
	{
		if (newRule instanceof MutableAssignmentRule)
		{
			rule = (MutableAssignmentRule) newRule;
			setEnabled(true);
		}
		else
		{
			rule = null;
			setEnabled(false);
		}
	}

	@Override
	public void actionPerformed(ActionEvent e)
	{
		if (rule == null)
			return;
		
		int result = JOptionPane.showConfirmDialog(parent,
			"Delete " + rule.toString() + "?",
			"Delete Rule?",
			JOptionPane.OK_CANCEL_OPTION);
		
		if (result == JOptionPane.OK_OPTION)
		{
			// Get off EDT
			new Thread() {
				public void run()
				{
					int index = rules.indexOf(rule);
					rules.remove(rule);
					eventBus.post(new RuleRemovedEvent(rule, index));
				}
			}.start();
		}
	}

}
