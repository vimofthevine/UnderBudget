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

import javax.swing.tree.TreePath;

import com.vimofthevine.underbudget.core.estimate.Estimate;

/**
 * Convenience instance of <code>TreePath</code> for creating
 * a tree path from an estimate. The path to the estimate is
 * created by walking up the estimate tree from the given
 * estimate, using the parent estimates.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class EstimateTreePath extends TreePath {
	
	/**
	 * Builds a path for the estimate, walking up
	 * the estimate tree.
	 * 
	 * @param estimate estimate whose path is to be created
	 * @return array of objects representing the path to the estimate
	 */
	private static Object[] getPath(Estimate estimate)
	{
		LinkedList<Estimate> path = new LinkedList<Estimate>();
		
		for (Estimate node = estimate; node != null; node = node.getParent())
		{
			path.add(0, node);
		}
		
		return path.toArray();
	}


	/**
	 * Constructs a new TreePath for the given estimate.
	 * 
	 * @param estimate estimate whose path is to be created
	 */
	EstimateTreePath(Estimate estimate)
	{
		super(getPath(estimate));
	}
	
}
