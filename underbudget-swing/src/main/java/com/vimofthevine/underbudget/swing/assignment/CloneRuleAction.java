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
import com.vimofthevine.underbudget.core.assignment.MutableAssignmentRule;
import com.vimofthevine.underbudget.core.assignment.RuleDefinition;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.transaction.Transaction;
import com.vimofthevine.underbudget.swing.assignment.events.RuleAddedEvent;

/**
 * Action to clone the selected assignment rule.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CloneRuleAction extends AbstractAction {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Assignment rules list
	 */
	private final AssignmentRules rules;
	
	/**
	 * Currently represented assignment rule
	 */
	private MutableAssignmentRule rule;
	
	/**
	 * Constructs a new clone-rule action.
	 * 
	 * @param bus      event bus
	 * @param ruleList assignment rules list
	 */
	CloneRuleAction(EventBus bus, AssignmentRules ruleList)
	{
		eventBus = bus;
		rules = ruleList;
		
		putValue(NAME, "Clone");
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
		
		// Get off EDT
		new Thread() {
			public void run()
			{
				int index = rules.indexOf(rule);
				final AssignmentRule clonedRule = rule;
				AssignmentRule newRule = new AssignmentRule() {
					@Override
                    public RuleDefinition getDefinition()
                    {
						return clonedRule.getDefinition();
                    }

					@Override
                    public boolean matches(Transaction transaction)
                    {
	                    return false;
                    }

					@Override
                    public Estimate getEstimate()
                    {
						return clonedRule.getEstimate();
                    }
				};
				
				rules.insert(newRule, index);
				eventBus.post(new RuleAddedEvent(newRule, index));
			}
		}.start();
	}

}
