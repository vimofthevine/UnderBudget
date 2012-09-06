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

import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.DefaultListSelectionModel;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.TransferHandler;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.TableModel;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.assignment.AssignmentRules;
import com.vimofthevine.underbudget.swing.assignment.events.RuleAddedEvent;
import com.vimofthevine.underbudget.swing.assignment.events.RuleModifiedEvent;
import com.vimofthevine.underbudget.swing.assignment.events.RuleRemovedEvent;
import com.vimofthevine.underbudget.swing.assignment.events.RuleSelectedEvent;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateModifiedEvent;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateSelectedEvent;

/**
 * Presentation model for views that display
 * a reorderable list of assignment rules.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AssignmentRuleListViewModel
implements ListSelectionListener {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(AssignmentRuleListViewModel.class.getName());
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Assignment rules table model
	 */
	private final AssignmentRulesTableModel tableModel;
	
	/**
	 * Table selection model
	 */
	private final ListSelectionModel selectionModel;
	
	/**
	 * Reordering transfer handler
	 */
	private final TransferHandler transferHandler;
	
	/**
	 * Assignment rule list
	 */
	private final AssignmentRules rules;
	
	/**
	 * Currently selected assignment rule
	 */
	private AssignmentRule selectedRule;
	
	/**
	 * Constructs a new assignment rules table view model.
	 * 
	 * @param bus   event bus
	 * @param rules assignment rules
	 */
	public AssignmentRuleListViewModel(EventBus bus, AssignmentRules rules)
	{
		eventBus = bus;
		eventBus.register(this);
		
		this.rules = rules;
		
		tableModel = new AssignmentRulesTableModel(rules);
		
		selectionModel = new DefaultListSelectionModel();
		selectionModel.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		selectionModel.addListSelectionListener(this);
		
		transferHandler = new AssignmentRuleReorderHandler(
			eventBus, tableModel, selectionModel);
	}
	
	/**
	 * Returns the table model representing
	 * the assignment rule data.
	 * 
	 * @return assignment rules table model
	 */
	TableModel getTableModel()
	{
		return tableModel;
	}
	
	/**
	 * Returns the table selection model.
	 * 
	 * @return assignment rule table selection model
	 */
	ListSelectionModel getSelectionModel()
	{
		return selectionModel;
	}
	
	/**
	 * Returns a transfer handler to execute
	 * drag-and-drop style reordering of rules
	 * within the list.
	 * 
	 * @return rule reordering transfer handler
	 */
	public TransferHandler getTransferHandler()
	{
		return transferHandler;
	}

	@Override
    public void valueChanged(ListSelectionEvent event)
    {
		logger.log(Level.FINEST, "Rule selection changed");
		
		if (selectionModel.isSelectionEmpty())
		{
			
		}
		else
		{
    		int index = selectionModel.getLeadSelectionIndex();
    		final AssignmentRule rule = rules.getAt(index);
    		
    		if (selectedRule == null ||
    			! selectedRule.equals(rule))
    		{
        		logger.log(Level.FINER, "Selected " + rule + " at index " + index);
        		
        		if (rule != null)
        		{
        			// Get off EDT
        			new Thread() {
        				public void run()
        				{
        					eventBus.post(new RuleSelectedEvent(rule));
        					eventBus.post(new EstimateSelectedEvent(rule.getEstimate(), null));
        				}
        			}.start();
        		}
    		}
		}
    }
	
	@Subscribe
	public void ruleSelected(RuleSelectedEvent event)
	{
		// Don't do anything if already selected (avoid infinite loop)
		if (selectedRule == null ||
			! selectedRule.equals(event.getAssignmentRule()))
		{
			selectedRule = event.getAssignmentRule();
			final int index = (selectedRule == null) ?
				-1 : rules.indexOf(selectedRule);
			
			SwingUtilities.invokeLater(new Runnable() {
				public void run()
				{
					if (index == -1)
					{
						logger.log(Level.FINEST, "Clearing selection");
						selectionModel.clearSelection();
					}
					else
					{
						selectionModel.setSelectionInterval(index, index);
					}
				}
			});
		}
	}
	
	/**
	 * Updates the rule table to reflect a change
	 * to a particular assignment rule.
	 * 
	 * @param event rule modified event
	 */
	@Subscribe
	public void ruleModified(RuleModifiedEvent event)
	{
		AssignmentRule changedRule = event.getAssignmentRule();
		final int index = rules.indexOf(changedRule);
		
		if (index < 0)
			return;
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				tableModel.fireTableRowsUpdated(index, index);
			}
		});
	}
	
	@Subscribe
	public void ruleAdded(RuleAddedEvent event)
	{
		final int index = event.getIndex();
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				tableModel.fireTableRowsInserted(index, index);
			}
		});
	}
	
	@Subscribe
	public void ruleRemoved(RuleRemovedEvent event)
	{
		final int index = event.getIndex();
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				tableModel.fireTableRowsDeleted(index, index);
			}
		});
	}
	
	/**
	 * Updates the rule table to reflect a change
	 * to an estimate's name.
	 * 
	 * @param event estimate modified event
	 */
	@Subscribe
	public void estimateModified(EstimateModifiedEvent event)
	{
		// If an estimate name changed
		if (event.getChanges().containsKey("name"))
		{
			SwingUtilities.invokeLater(new Runnable() {
				public void run()
				{
					// Redraw the whole table (many rows may have been affected)
					tableModel.fireTableDataChanged();
				}
			});
		}
	}

}
