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

import java.util.Map;

import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.estimate.EstimateField;
import com.vimofthevine.underbudget.swing.history.ModificationEvent;

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
	private final Map<EstimateField, Object> changes;
	
	/**
	 * Constructs a new estimate modification event.
	 * 
	 * @param estimate estimate that has been modified
	 * @param changes  list of changes that have been made
	 */
	public EstimateModifiedEvent(Estimate estimate,
		Map<EstimateField, Object> changes)
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
	public Map<EstimateField, Object> getChanges()
	{
		return changes;
	}
	
	@Override
	public String toString()
	{
		return estimate + " modified " + changes;
	}
	
}
