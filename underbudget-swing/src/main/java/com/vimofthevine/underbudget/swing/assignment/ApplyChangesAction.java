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
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

import javax.swing.AbstractAction;

import com.google.common.eventbus.EventBus;
import com.vimofthevine.underbudget.core.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.assignment.Condition;
import com.vimofthevine.underbudget.core.assignment.MutableAssignmentRule;
import com.vimofthevine.underbudget.core.assignment.RuleDefinition;
import com.vimofthevine.underbudget.swing.assignment.events.RuleModifiedEvent;

/**
 * Action to apply changes made to an assignment rule.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class ApplyChangesAction extends AbstractAction
implements ConditionsChangedListener {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Conditions table model
	 */
	private final RuleConditionsTableModel tableModel;
	
	/**
	 * Change set
	 */
	private final Map<String,String> changes;
	
	/**
	 * Assignment rule being modified
	 */
	private MutableAssignmentRule rule;
	
	/**
	 * Constructs a new apply-changes action model.
	 * 
	 * @param bus   event bus
	 * @param table conditions table model
	 */
	ApplyChangesAction(EventBus bus, RuleConditionsTableModel table)
	{
		eventBus = bus;
		tableModel = table;
		tableModel.addListener(this);
		
		setEnabled(false);
		
		changes = new HashMap<String,String>();
		
		putValue(NAME, "Apply");
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
		}
		else
		{
			rule = null;
		}
		
		setEnabled(false);
	}

	@Override
	public void actionPerformed(ActionEvent e)
	{
		if (rule != null)
		{
			// Grab this while on EDT
			final Condition[] conditions = tableModel.getConditions();
			setEnabled(false);
			
			// Get off EDT
			new Thread() {
				public void run()
				{
        			rule.setDefinition(new RuleDefinition() {
        				@Override
                        public Condition[] getConditions()
                        {
        					return conditions;
                        }
        			});
        			
        			changes.put("conditions", Arrays.toString(conditions));
        			eventBus.post(new RuleModifiedEvent(rule,changes));
				}
			}.start();
		}
	}

	@Override
    public void conditionsChanged(Condition[] conditions)
    {
		setEnabled(true);
    }

}
