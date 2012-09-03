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

import javax.swing.tree.TreePath;

import org.jdesktop.swingx.treetable.AbstractTreeTableModel;

import com.vimofthevine.underbudget.core.assignment.ActualFigures;
import com.vimofthevine.underbudget.core.estimate.Estimate;

/**
 * A tree-table model for displaying hierarchical
 * estimates in a table with multiple perspectives
 * (the columns that are displayed).
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
abstract class AbstractEstimateTreeTableModel extends AbstractTreeTableModel {
	
	/**
	 * Actual values source
	 */
	protected ActualFigures actuals;
	
	/**
	 * Constructs a new estimate tree
	 * table model.
	 * 
	 * @param root root estimate
	 */
	public AbstractEstimateTreeTableModel(Estimate root)
	{
		super(root);
		
		actuals = new UnevaluatedActuals(
			root.getDefinition().getAmount().getCurrency());
	}
	
	/**
	 * Sets the actual amounts to be used for
	 * displaying actuals.
	 * 
	 * @param actuals actual figures
	 */
	void setActuals(ActualFigures actuals)
	{
		this.actuals = actuals;
	}

	@Override
    public Object getChild(Object parent, int index)
    {
		if (parent instanceof Estimate)
			return ((Estimate) parent).getChildAt(index);
		else
			throw new IllegalArgumentException("Unknown node object type, " + parent.getClass().getName());
    }

	@Override
    public int getChildCount(Object parent)
    {
		if (parent instanceof Estimate)
			return ((Estimate) parent).getChildCount();
		else
			throw new IllegalArgumentException("Unknown node object type, " + parent.getClass().getName());
    }

	@Override
    public int getIndexOfChild(Object parent, Object child)
    {
		if ( ! (parent instanceof Estimate)
				|| ! (child instanceof Estimate))
			return -1;
		else
			return ((Estimate) parent).indexOf((Estimate) child);
    }
	
	@Override
	public void valueForPathChanged(TreePath path, Object newValue)
	{
		if (path.getPathComponent(0) != root)
			throw new IllegalArgumentException("Invalid path");
		
		modelSupport.firePathChanged(path);
	}
	
	/**
	 * Notifies registered listeners that a new child
	 * estimate has been added to the specified parent
	 * at the given path.
	 * 
	 * @param path   path to the parent estimate
	 * @param parent parent estimate to which a child was added
	 * @param child  new estimate that has been added to the tree
	 */
	public void fireEstimateAdded(TreePath path, Estimate parent, Estimate child)
	{
		modelSupport.fireChildAdded(path, parent.indexOf(child), child);
	}
	
	/**
	 * Notifies registered listeners that an estimate
	 * has been removed from the estimate tree.
	 * 
	 * @param path  path to the remove estimate's parent
	 * @param index index of the removed estimate within the parent
	 * @param child estimate that has been removed from the tree
	 */
	public void fireEstimateRemoved(TreePath path, int index, Estimate child)
	{
		modelSupport.fireChildRemoved(path, index, child);
	}
	
}
