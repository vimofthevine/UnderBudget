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

package com.vimofthevine.underbudget.swing.assignment.events;

import com.vimofthevine.underbudget.swing.history.ModificationEvent;

/**
 * Event indicating that a rule has been moved in
 * the assignment rules list.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class RuleMovedEvent implements ModificationEvent {
	
	/**
	 * Row from which the rule was moved
	 */
	private final int fromRow;
	
	/**
	 * Row to which the rule was moved
	 */
	private final int toRow;
	
	/**
	 * Constructs a new rule-moved event.
	 * 
	 * @param srcRow  source row index
	 * @param destRow destination row index
	 */
	public RuleMovedEvent(int srcRow, int destRow)
	{
		fromRow = srcRow;
		toRow = destRow;
	}
	
	/**
	 * Returns the row index from which
	 * the rule was moved.
	 * 
	 * @return source row index
	 */
	public int getSourceRow()
	{
		return fromRow;
	}
	
	/**
	 * Returns the destination index
	 * to which the rule was moved.
	 * 
	 * @return destination row index
	 */
	public int getDestinationRow()
	{
		return toRow;
	}

}
