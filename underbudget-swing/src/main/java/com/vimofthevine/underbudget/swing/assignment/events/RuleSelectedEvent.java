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

import com.vimofthevine.underbudget.core.assignment.AssignmentRule;
import com.vimofthevine.underbudget.swing.history.SelectionEvent;

/**
 * An event that represents the selection
 * of an assignment rule.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class RuleSelectedEvent implements SelectionEvent {

	/**
	 * The assignment rule that has been selected
	 */
	private final AssignmentRule rule;
	
	/**
	 * Constructs a new assignment rule selection event.
	 * 
	 * @param rule assignment rule that has been selected
	 */
	public RuleSelectedEvent(AssignmentRule rule)
	{
		this.rule = rule;
	}
	
	/**
	 * Returns the assignment rule that has been selected.
	 * 
	 * @return assignment rule that has been selected
	 */
	public AssignmentRule getAssignmentRule()
	{
		return rule;
	}
	
}
