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
 * An event that represents the removal of an
 * assignment rule from the rules list.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class RuleRemovedEvent implements ModificationEvent {

	/**
	 * The assignment rule that has been removed.
	 */
	private final AssignmentRule rule;
	
	/**
	 * The index at which the rule was removed.
	 */
	private final int index;
	
	/**
	 * Constructs a new assignment rule removed event.
	 * 
	 * @param rule  assignment rule that has been removed
	 * @param index index at which the rule was removed from the list
	 */
	public RuleRemovedEvent(AssignmentRule rule, int index)
	{
		this.rule = rule;
		this.index = index;
	}
	
	/**
	 * Returns the assignment rule that has
	 * been removed.
	 * 
	 * @return assignment rule that was removed
	 */
	public AssignmentRule getRule()
	{
		return rule;
	}
	
	/**
	 * Returns the index in the list at which
	 * the rule was inserted.
	 * 
	 * @return rule list index of deletion
	 */
	public int getIndex()
	{
		return index;
	}
	
}
