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

import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.swing.history.ModificationEvent;

/**
 * An event that represents the removal of an
 * existing estimate from the estimate tree.
 * There is no assurance that the removed estimate
 * still exists within the estimate tree at the
 * time of receipt of this event. It is safer to
 * assume that the relationship between the parent
 * and child has already been broken as a result of
 * this event.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateRemovedEvent implements ModificationEvent {

	/**
	 * The estimate from which a child was removed.
	 */
	private final Estimate parent;
	
	/**
	 * The estimate that was removed.
	 */
	private final Estimate child;
	
	/**
	 * The index of the child estimate
	 * within the parent's child estimates
	 * before the estimate was removed
	 */
	private final int index;
	
	/**
	 * Constructs a new estimate removed event.
	 * 
	 * @param parent estimate from which a child was removed
	 * @param index  index of the child estimate within the parent
	 * @param child  estimate that was remove
	 */
	public EstimateRemovedEvent(Estimate parent, int index, Estimate child)
	{
		this.parent = parent;
		this.index = index;
		this.child = child;
	}
	
	/**
	 * Returns the parent estimate from which
	 * a child estimate was removed.
	 * 
	 * @return estimate to which a child was removed
	 */
	public Estimate getParent()
	{
		return parent;
	}
	
	/**
	 * Returns the child estimate that has
	 * been removed.
	 * 
	 * @return estimate that was removed
	 */
	public Estimate getChild()
	{
		return child;
	}
	
	/**
	 * Returns the index of the child estimate
	 * within the parent's child estimates before
	 * the estimate was removed.
	 * 
	 * @return index of the child estimate within the parent
	 */
	public int getIndex()
	{
		return index;
	}
	
	@Override
	public String toString()
	{
		return child + " removed from " + parent + "[" + index + "]";
	}
	
}
