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

package com.vimofthevine.underbudget.core.budget.estimate;

import java.util.Iterator;
import java.util.Vector;

/**
 * Iterator to facilitate iteration over a tree of
 * <code>DefaultEstimate</code> items.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class DefaultEstimateIterator implements Iterator<Estimate> {
	
	/**
	 * Internal estimate iterator
	 */
	private Iterator<Estimate> iterator;
	
	/**
	 * Constructs a new iterator for the
	 * given estimate tree root.
	 * 
	 * @param root estimate tree root
	 */
	public DefaultEstimateIterator(DefaultEstimate root)
	{
		iterator = createDepthFirstVector(root).iterator();
	}
	
	/**
	 * Creates a vector containing the given estimate and all
	 * children the estimate, recursively.
	 * 
	 * @param estimate sub-tree root from which to create a vector
	 * @return vector representing the given estimate sub-tree
	 */
	private Vector<Estimate> createDepthFirstVector(DefaultEstimate estimate)
	{
		Vector<Estimate> vector = new Vector<Estimate>();
		vector.add(estimate);
		
		if (estimate.getChildren() != null)
		{
    		Iterator<DefaultEstimate> iterator = estimate.getChildren().iterator();
    		while (iterator.hasNext())
    		{
    			vector.addAll(createDepthFirstVector(iterator.next()));
    		}
		}
		
		return vector;
	}

	@Override
    public boolean hasNext()
    {
		return iterator.hasNext();
    }

	@Override
    public Estimate next()
    {
		return iterator.next();
    }

	@Override
    public void remove()
    {
		throw new UnsupportedOperationException();
    }

}
