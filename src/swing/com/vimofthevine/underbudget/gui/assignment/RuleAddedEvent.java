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

package com.vimofthevine.underbudget.gui.assignment;

import com.vimofthevine.underbudget.assignment.AssignmentRule;
import com.vimofthevine.underbudget.gui.history.ModificationEvent;

/**
 * An event that represents the addition of a new
 * assignment rule into the rule list.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class RuleAddedEvent implements ModificationEvent {

	/**
	 * The assignment rule that has been added.
	 */
	private final AssignmentRule rule;
	
	/**
	 * The index at which the rule was inserted.
	 */
	private final int index;
	
	/**
	 * Constructs a new assignment rule added event.
	 * 
	 * @param rule  assignment rule that has been added
	 * @param index index at which the rule was inserted into the list
	 */
	public RuleAddedEvent(AssignmentRule rule, int index)
	{
		this.rule = rule;
		this.index = index;
	}
	
	/**
	 * Returns the assignment rule that has
	 * been added.
	 * 
	 * @return assignment rule that was added
	 */
	public AssignmentRule getRule()
	{
		return rule;
	}
	
	/**
	 * Returns the index in the list at which
	 * the rule was inserted.
	 * 
	 * @return rule list index of insertion
	 */
	public int getIndex()
	{
		return index;
	}
	
}
