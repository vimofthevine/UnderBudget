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

import javax.swing.Action;
import javax.swing.DefaultListSelectionModel;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.table.TableModel;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.swing.assignment.events.RuleAddedEvent;
import com.vimofthevine.underbudget.swing.assignment.events.RuleRemovedEvent;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateSelectedEvent;

/**
 * Presentation model for views that display a
 * list of rules associated with an individual
 * estimate.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateAssignmentRulesViewModel {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Assignment rules
	 */
	private final ReverseLookupAssignmentRules rules;
	
	/**
	 * Assignment rules table model
	 */
	private final EstimateRulesTableModel tableModel;
	
	/**
	 * Rules selection model
	 */
	private final ListSelectionModel selectionModel;
	
	/**
	 * Create rule action
	 */
	private final CreateRuleAction createRuleAction;
	
	/**
	 * Currently selected estimate
	 */
	private Estimate estimate;
	
	/**
	 * Constructs a new estimate assignment rules
	 * view model.
	 * 
	 * @param bus   event bus
	 * @param rules assignment rules list
	 */
	public EstimateAssignmentRulesViewModel(EventBus bus,
		ReverseLookupAssignmentRules rules)
	{
		eventBus = bus;
		eventBus.register(this);
		
		this.rules = rules;
		
		tableModel = new EstimateRulesTableModel();
		
		selectionModel = new DefaultListSelectionModel();
		selectionModel.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		
		createRuleAction = new CreateRuleAction(eventBus, rules);
	}

	/**
	 * Returns the table model representing
	 * the associated assignment rule data.
	 * 
	 * @return associated rules table model
	 */
	public TableModel getTableModel()
	{
		return tableModel;
	}
	
	/**
	 * Returns the assignment rule table selection model.
	 * 
	 * @return rule selection model
	 */
	public ListSelectionModel getSelectionModel()
	{
		return selectionModel;
	}
	
	/**
	 * Returns an action for creating a new
	 * rule associated with the current estimate.
	 * 
	 * @return create rule action
	 */
	public Action getAddRuleAction()
	{
		return createRuleAction;
	}
	
	private void update()
	{
		final AssignmentRule[] assocRules = (estimate == null)
			? new AssignmentRule[0] : rules.getRules(estimate);
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				tableModel.setRules(assocRules);
			}
		});
	}
	
	@Subscribe
	public void estimateSelected(EstimateSelectedEvent event)
	{
		estimate = event.getEstimate();
		
		update();
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				createRuleAction.setEstimate(estimate);
			}
		});
	}
	
	@Subscribe
	public void ruleAdded(RuleAddedEvent event)
	{
		if (event.getRule().getEstimate().equals(estimate))
		{
			update();
		}
	}
	
	@Subscribe
	public void ruleRemoved(RuleRemovedEvent event)
	{
		if (event.getRule().getEstimate().equals(estimate))
		{
			update();
		}
	}
	
}
