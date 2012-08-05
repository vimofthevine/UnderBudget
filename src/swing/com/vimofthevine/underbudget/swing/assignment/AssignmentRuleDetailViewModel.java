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

import javax.swing.Action;
import javax.swing.DefaultListSelectionModel;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.table.TableModel;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.assignment.AssignmentRules;
import com.vimofthevine.underbudget.swing.assignment.events.RuleSelectedEvent;

/**
 * Presentation model for views that display
 * modifiable details about an assignment rule.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AssignmentRuleDetailViewModel {
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Conditions table model
	 */
	private final RuleConditionsTableModel tableModel;
	
	/**
	 * Condition selection model
	 */
	private final ListSelectionModel selectionModel;
	
	/**
	 * Add-condition action model
	 */
	private final AddConditionAction addConditionAction;
	
	/**
	 * Remove-condition action model
	 */
	private final RemoveConditionAction removeConditionAction;
	
	/**
	 * Apply changes action model
	 */
	private final ApplyChangesAction applyChangesAction;
	
	/**
	 * Delete rule action model
	 */
	private final DeleteRuleAction deleteRuleAction;
	
	/**
	 * Clone rule action model
	 */
	private final CloneRuleAction cloneRuleAction;
	
	/**
	 * Constructs a new assignment rule detail view model.
	 * 
	 * @param bus    event bus
	 * @param parent parent Swing component
	 * @param rules  assignment rule list
	 */
	public AssignmentRuleDetailViewModel(EventBus bus, Component parent,
		AssignmentRules rules)
	{
		eventBus = bus;
		eventBus.register(this);
		
		tableModel = new RuleConditionsTableModel();
		
		selectionModel = new DefaultListSelectionModel();
		selectionModel.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		
		applyChangesAction = new ApplyChangesAction(eventBus, tableModel);
		addConditionAction = new AddConditionAction(tableModel, applyChangesAction);
		removeConditionAction = new RemoveConditionAction(tableModel, selectionModel, applyChangesAction);
		deleteRuleAction = new DeleteRuleAction(eventBus, parent, rules);
		cloneRuleAction = new CloneRuleAction(eventBus, rules);
	}

	/**
	 * Returns the table model representing
	 * the rule conditions data.
	 * 
	 * @return rule conditions table model
	 */
	public TableModel getConditionsTableModel()
	{
		return tableModel;
	}
	
	/**
	 * Returns the selection model for the
	 * conditions table.
	 * 
	 * @return conditions table selection model
	 */
	public ListSelectionModel getConditionSelectionModel()
	{
		return selectionModel;
	}
	
	/**
	 * Returns an action for adding a new
	 * rule condition.
	 * 
	 * @return create condition action
	 */
	public Action getAddConditionAction()
	{
		return addConditionAction;
	}
	
	/**
	 * Returns an action for removing a new
	 * rule condition.
	 * 
	 * @return removing condition action
	 */
	public Action getRemoveConditionAction()
	{
		return removeConditionAction;
	}
	
	/**
	 * Returns an action for applying the
	 * changes made for the rule conditions.
	 * 
	 * @return apply conditions action
	 */
	public Action getApplyChangesAction()
	{
		return applyChangesAction;
	}
	
	/**
	 * Returns an action for deleting the
	 * assignment rule.
	 * 
	 * @return delete rule action
	 */
	public Action getDeleteAction()
	{
		return deleteRuleAction;
	}
	
	/**
	 * Returns an action for cloning the
	 * assignment rule
	 * 
	 * @return clone rule action
	 */
	public Action getCloneAction()
	{
		return cloneRuleAction;
	}
	
	/**
	 * Updates the table model and actions to reflect
	 * the currently selected assignment rule.
	 * 
	 * @param event rule selection event
	 */
	@Subscribe
	public void ruleSelected(RuleSelectedEvent event)
	{
		final AssignmentRule rule = event.getAssignmentRule();
		
		if (rule == null)
			return;
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				applyChangesAction.setRule(rule);
				deleteRuleAction.setRule(rule);
				cloneRuleAction.setRule(rule);
				
				tableModel.setConditions(rule.getDefinition().getConditions());
				selectionModel.clearSelection();
			}
		});
	}
	
}
