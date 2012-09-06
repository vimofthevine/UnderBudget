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

import java.util.LinkedList;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.SwingUtilities;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.TreePath;
import javax.swing.tree.TreeSelectionModel;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.assignment.ActualFigures;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.swing.assignment.events.TransactionsAssignedEvent;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateAddedEvent;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateModifiedEvent;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateRemovedEvent;
import com.vimofthevine.underbudget.swing.estimate.events.EstimateSelectedEvent;

/**
 * A presentation model for views that display
 * estimate trees in a hierarchical model.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateTreeViewModel {
	
	/**
	 * Log handle
	 */
	private final static Logger logger = Logger.getLogger(EstimateTreeViewModel.class.getName());
	
	/**
	 * Log string to differentiate between different instances
	 * of an EstimateTreeViewModel, according to the tree-table model
	 */
	private final String log;
	
	/**
	 * Event bus
	 */
	private final EventBus eventBus;
	
	/**
	 * Estimate tree table model
	 */
	private final EstimateTreeTableModel treeTableModel;
	
	/**
	 * Tree selection model
	 */
	private TreeSelectionModel selectionModel;
	
	/**
	 * The currently selected path
	 */
	private TreePath currentSelectionPath;
	
	/**
	 * Constructs a new estimate tree-table view model.
	 * 
	 * @param bus   event bus
	 * @param model estimate tree table model
	 */
	public EstimateTreeViewModel(EventBus bus, EstimateTreeTableModel model)
	{
		log = model.getClass().getSimpleName() + ": ";
		
		eventBus = bus;
		eventBus.register(this);
		
		treeTableModel = model;
	}
	
	/**
	 * Returns the tree-table model that represents
	 * the hierarchical estimate tree.
	 * 
	 * @return estimate tree table model
	 */
	EstimateTreeTableModel getTreeTableModel()
	{
		return treeTableModel;
	}
	
	/**
	 * Specifies the tree selection model for the displayed
	 * estimate tree-table.
	 * 
	 * @param model estimate tree-table selection model
	 */
	void setTreeSelectionModel(TreeSelectionModel model)
	{
		selectionModel = model;
	}
	
	/**
	 * Constructs a TreePath for the given estimate.
	 * 
	 * @param estimate estimate whose path is to be created
	 * @return tree path to the given estimate
	 */
	private TreePath getPath(Estimate estimate)
	{
		logger.log(Level.FINE, log + "Determining path to " + estimate);
		
		LinkedList<Estimate> path = new LinkedList<Estimate>();
		
		for (Estimate node = estimate; node != null; node = node.getParent())
		{
			path.add(0, node);
		}
		
		logger.log(Level.FINE, log + "Got path: " + path);
		
		return new TreePath(path.toArray());
	}
	
	/**
	 * Creates a tree selection listener to receive notification
	 * of estimate selections. A <code>EstimateSelectionEvent</code>
	 * is fired upon notification of a selection change.
	 * 
	 * @return estimate tree-table selection listener
	 */
	public TreeSelectionListener getSelectionListener()
	{
		return new TreeSelectionListener() {
			@Override
			public void valueChanged(TreeSelectionEvent event)
			{
				// Don't do anything if already selected (avoid infinite loop)
				if (currentSelectionPath == null ||
					! currentSelectionPath.equals(event.getNewLeadSelectionPath()))
				{
    				currentSelectionPath = event.getNewLeadSelectionPath();
    				if (currentSelectionPath != null &&
    					currentSelectionPath.getLastPathComponent() instanceof Estimate)
    				{
    					final Estimate estimate = (Estimate) currentSelectionPath.getLastPathComponent();
    					
    					// Get off EDT
    					new Thread() {
    						public void run()
    						{
    							eventBus.post(new EstimateSelectedEvent(estimate, currentSelectionPath));
    						}
    					}.start();
    				}
				}
			}
		};
	}
	
	/**
	 * Updates the estimate tree-table to reflect the
	 * currently selected estimate.
	 * 
	 * @param event estimate selection event
	 */
	@Subscribe
	public synchronized void estimateSelected(EstimateSelectedEvent event)
	{
		logger.log(Level.FINEST, log + "Estimate selected: " + event);
		
		if (event.getEstimate() == null)
		{
			currentSelectionPath = null;
			SwingUtilities.invokeLater(new Runnable() {
				public void run()
				{
					selectionModel.clearSelection();
				}
			});
			
			return;
		}
		
		TreePath selectionPath = (event.getTreePath() == null)
			? getPath(event.getEstimate()) : event.getTreePath();
		
		// Don't do anything if already selected (avoid infinite loop)
		if (currentSelectionPath == null ||
			! currentSelectionPath.equals(selectionPath))
		{
			logger.log(Level.FINEST, log + "Selecting estimate, " + event.getTreePath());
			currentSelectionPath = selectionPath;
			
			// Select the new estimate
			if (selectionModel != null)
			{
				final TreePath newSelection = selectionPath;
				
				SwingUtilities.invokeLater(new Runnable() {
					public void run()
					{
						logger.log(Level.FINEST, log + "Updating tree selection to selected estimate");
						selectionModel.setSelectionPath(newSelection);
					}
				});
			}
		}
		else
		{
			logger.log(Level.FINER, log + "Estimate is already selected");
		}
	}
	
	/**
	 * Updates the estimate tree-table to reflect a change
	 * to a particular estimate.
	 * 
	 * @param event estimate modification event
	 */
	@Subscribe
	public synchronized void estimateModified(final EstimateModifiedEvent event)
	{
		logger.log(Level.FINE, log + "Estimate modified: " + event);
		
		final TreePath path = getPath(event.getEstimate());
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				// Update tree-table
				logger.log(Level.FINE, log + "Updating tree-table to reflect modified estimate");
				treeTableModel.valueForPathChanged(path, event.getEstimate());
			}
		});
	}
	
	/**
	 * Updates the estimate tree-table to reflect the fact
	 * that a new estimate has been added to the tree.
	 * 
	 * @param event estimate added event
	 */
	@Subscribe
	public synchronized void estimateAdded(EstimateAddedEvent event)
	{
		logger.log(Level.FINE, log + "Estimate added: " + event);
		
		final Estimate parent = event.getParent();
		final Estimate child = event.getChild();
		final TreePath parentPath = getPath(parent);
		final TreePath childPath = getPath(child);
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				// Redraw the table
				logger.log(Level.FINER, log + "Updating tree-table model to reflect added estimate");
				treeTableModel.fireEstimateAdded(parentPath, parent, child);
			}
		});
		
		// If the estimate was added to the currently selected estimate
		if (parent.equals(currentSelectionPath.getLastPathComponent()))
		{
			logger.log(Level.FINER, log + "New selection path of added estimate: " + childPath);
			
			SwingUtilities.invokeLater(new Runnable() {
				public void run()
				{
					if (selectionModel != null)
					{
						logger.log(Level.FINEST, log + "Updating tree selection to added estimate");
						selectionModel.setSelectionPath(childPath);
					}
				}
			});
		}
	}
	
	/**
	 * Updates the estimate tree-table to reflect the fact
	 * that an estimate has been removed from the tree.
	 * 
	 * @param event estimate removed event
	 */
	@Subscribe
	public synchronized void estimateRemoved(EstimateRemovedEvent event)
	{
		logger.log(Level.FINE, log + "Estimate removed: " + event);
		
		final Estimate parent = event.getParent();
		final Estimate child = event.getChild();
		final int index = event.getIndex();
		final TreePath parentPath = getPath(parent);
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				// Redraw the table
				logger.log(Level.FINER, log + "Updating tree-table model to reflect removed estimate");
				treeTableModel.fireEstimateRemoved(parentPath, index, child);
			}
		});
		
		// If the estimate removed was the currently selected estimate
		if (parent.equals(currentSelectionPath.getParentPath()
				.getLastPathComponent()))
		{
			logger.log(Level.FINER, log + "New selection path of removed estimate's parent: " + parentPath);
			
			SwingUtilities.invokeLater(new Runnable() {
				public void run()
				{
					if (selectionModel != null)
					{
						logger.log(Level.FINEST, log + "Updating tree selection to parent of removed estimate");
						selectionModel.setSelectionPath(parentPath);
					}
				}
			});
		}
	}
	
	@Subscribe
	public void transactionsAssigned(TransactionsAssignedEvent event)
	{
		// Store transaction assignments if they provide a valid actuals source
		if (event.getAssignments() instanceof ActualFigures)
		{
			final ActualFigures actuals = (ActualFigures) event.getAssignments();
			
			SwingUtilities.invokeLater(new Runnable() {
				public void run()
				{
					treeTableModel.setActuals(actuals);
				}
			});
		}
	}
	
}
