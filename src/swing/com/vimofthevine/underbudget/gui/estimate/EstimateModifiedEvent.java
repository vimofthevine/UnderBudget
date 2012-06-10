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

package com.vimofthevine.underbudget.gui.estimate;

import java.util.Map;

import com.vimofthevine.underbudget.estimate.Estimate;
import com.vimofthevine.underbudget.gui.history.ModificationEvent;

/**
 * An event that represents a change to an
 * estimate.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateModifiedEvent implements ModificationEvent {

	/**
	 * The estimate that has been modified.
	 */
	private final Estimate estimate;
	
	/**
	 * A map of fields that were changes, where
	 * the index is the field name and the value
	 * is the old value of the field.
	 */
	private final Map<String, String> changes;
	
	/**
	 * Constructs a new estimate modification event.
	 * 
	 * @param estimate estimate that has been modified
	 * @param changes  list of changes that have been made
	 */
	public EstimateModifiedEvent(Estimate estimate,
		Map<String, String> changes)
	{
		this.estimate = estimate;
		this.changes = changes;
	}
	
	/**
	 * Returns the estimate that has been modified.
	 * 
	 * @return estimate that has been modified
	 */
	public Estimate getEstimate()
	{
		return estimate;
	}
	
	/**
	 * Returns the list of changes that have been
	 * made to the estimate.
	 * 
	 * @return list of changes
	 */
	public Map<String, String> getChanges()
	{
		return changes;
	}
	
}
