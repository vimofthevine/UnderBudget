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

import java.util.Arrays;
import java.util.LinkedList;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.SwingUtilities;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.TreePath;
import javax.swing.tree.TreeSelectionModel;

import org.jdesktop.swingx.treetable.TreeTableModel;

import com.google.common.eventbus.EventBus;
import com.google.common.eventbus.Subscribe;
import com.vimofthevine.underbudget.core.estimate.Estimate;

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
	private final AbstractEstimateTreeTableModel treeTableModel;
	
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
	public EstimateTreeViewModel(EventBus bus, AbstractEstimateTreeTableModel model)
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
	TreeTableModel getTreeTableModel()
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
    					Estimate estimate = (Estimate) currentSelectionPath.getLastPathComponent();
    					eventBus.post(new EstimateSelectedEvent(estimate, currentSelectionPath));
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
	public synchronized void estimateSelected(final EstimateSelectedEvent event)
	{
		logger.log(Level.INFO, log + "Estimate selected: " + event);
		
		// Don't do anything if already selected (avoid infinite loop)
		if (currentSelectionPath == null ||
			! currentSelectionPath.equals(event.getTreePath()))
		{
			logger.log(Level.INFO, log + "Selecting estimate");
			currentSelectionPath = event.getTreePath();
			
			// Select the new estimate
			if (selectionModel != null)
			{
				SwingUtilities.invokeLater(new Runnable() {
					public void run()
					{
						selectionModel.setSelectionPath(event.getTreePath());
					}
				});
			}
		}
		else
		{
			logger.log(Level.INFO, log + "Estimate is already selected");
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
		
		logger.log(Level.FINE, log + "Estimate modified event, determining path...");
		// Determine path for estimate
		final LinkedList<Estimate> path = new LinkedList<Estimate>();
		for (Estimate node = event.getEstimate(); node != null; node = node.getParent())
		{
			path.add(0, node);
		}
		logger.log(Level.FINE, log + "Got path: " + Arrays.toString(path.toArray()));
		
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
				System.out.println("Updating tree-table to reflect modified estimate");
				// Update tree-table
				treeTableModel.valueForPathChanged(
					new TreePath(path.toArray()), event.getEstimate());
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
		logger.log(Level.INFO, log + "Estimate added: " + event);
		
		final Estimate parent = event.getParent();
		final Estimate child = event.getChild();
		
		// If the estimate was added to the currently selected estimate
		if (parent.equals(currentSelectionPath.getLastPathComponent()))
		{
			final TreePath childPath = currentSelectionPath.pathByAddingChild(child);
			final TreePath parentPath = currentSelectionPath;
			logger.log(Level.INFO, log + "New selection path of added estimate: " + childPath);
			
			SwingUtilities.invokeLater(new Runnable() {
				public void run()
				{
					// Redraw the table
					logger.log(Level.INFO, log + "Updating tree-table model to reflect added estimate");
					treeTableModel.fireEstimateAdded(parentPath, parent, child);
					
					if (selectionModel != null)
					{
						logger.log(Level.INFO, log + "Updating tree selection to added estimate");
						selectionModel.setSelectionPath(childPath);
					}
				}
			});
		}
		else
		{
			throw new IllegalArgumentException("Estimate added to unselected path");
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
		logger.log(Level.INFO, log + "Estimate removed: " + event);
		
		final Estimate parent = event.getParent();
		final Estimate child = event.getChild();
		final int index = event.getIndex();
		
		// If the estimate removed was the currently selected estimate
		if (parent.equals(currentSelectionPath.getParentPath()
				.getLastPathComponent()))
		{
			final TreePath parentPath = currentSelectionPath.getParentPath();
			logger.log(Level.INFO, log + "New selection path of removed estimate's parent: " + parentPath);
			
			SwingUtilities.invokeLater(new Runnable() {
				public void run()
				{
					// Redraw the table
					logger.log(Level.INFO, log + "Updating tree-table model to reflect removed estimate");
					treeTableModel.fireEstimateRemoved(
						parentPath, index, child);
					
					if (selectionModel != null)
					{
						logger.log(Level.INFO, log + "Updating tree selection to parent of removed estimate");
						selectionModel.setSelectionPath(parentPath);
					}
				}
			});
		}
		else
		{
			throw new IllegalArgumentException("Estimate removed from unselected path");
		}
	}
	
}
