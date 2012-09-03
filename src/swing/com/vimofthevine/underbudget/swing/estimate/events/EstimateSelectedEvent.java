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

package com.vimofthevine.underbudget.swing.estimate.events;

import javax.swing.tree.TreePath;

import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.swing.history.SelectionEvent;

/**
 * An event that represents the selection
 * of an estimate.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateSelectedEvent implements SelectionEvent {

	/**
	 * The estimate that has been selected
	 */
	private final Estimate estimate;
	
	/**
	 * Tree path to the selected estimate
	 */
	private final TreePath path;
	
	/**
	 * Constructs a new estimate selection event.
	 * 
	 * @param estimate estimate that has been selected
	 * @param path     tree path to the selected estimate
	 */
	public EstimateSelectedEvent(Estimate estimate, TreePath path)
	{
		this.estimate = estimate;
		this.path = path;
	}
	
	/**
	 * Returns the estimate that has been selected.
	 * 
	 * @return estimate that has been selected
	 */
	public Estimate getEstimate()
	{
		return estimate;
	}
	
	/**
	 * Returns the tree path to the selected estimate.
	 * 
	 * @return tree path to the selected estimate
	 */
	public TreePath getTreePath()
	{
		return path;
	}
	
	@Override
	public String toString()
	{
		return estimate + " selected at " + path;
	}
	
}
