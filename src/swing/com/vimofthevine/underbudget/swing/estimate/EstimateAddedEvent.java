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
 * An event that represents the addition of a new
 * estimate into the estimate tree.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateAddedEvent implements ModificationEvent {

	/**
	 * The estimate to which a child was added.
	 */
	private final Estimate parent;
	
	/**
	 * The estimate that was created.
	 */
	private final Estimate child;
	
	/**
	 * Constructs a new estimate added event.
	 * 
	 * @param parent estimate to which a child was added
	 * @param child  estimate that was created
	 */
	public EstimateAddedEvent(Estimate parent, Estimate child)
	{
		this.parent = parent;
		this.child = child;
	}
	
	/**
	 * Returns the parent estimate to which
	 * a child estimate was added.
	 * 
	 * @return estimate to which a child was added
	 */
	public Estimate getParent()
	{
		return parent;
	}
	
	/**
	 * Returns the child estimate that has
	 * been created.
	 * 
	 * @return estimate that was created
	 */
	public Estimate getChild()
	{
		return child;
	}
	
}
