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

import java.util.Map;

import com.vimofthevine.underbudget.core.assignment.AssignmentRule;
import com.vimofthevine.underbudget.swing.history.ModificationEvent;

/**
 * An event that represents a change to an
 * assignment rule.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class RuleModifiedEvent implements ModificationEvent {

	/**
	 * The assignment rule that has been modified.
	 */
	private final AssignmentRule rule;
	
	/**
	 * A map of fields that were changes, where
	 * the index is the field name and the value
	 * is the old value of the field.
	 */
	private final Map<String, String> changes;
	
	/**
	 * Constructs a new assignment rule modification event.
	 * 
	 * @param rule    assignment rule that has been modified
	 * @param changes list of changes that have been made
	 */
	public RuleModifiedEvent(AssignmentRule rule,
		Map<String, String> changes)
	{
		this.rule = rule;
		this.changes = changes;
	}
	
	/**
	 * Returns the assignment rule that has been modified.
	 * 
	 * @return assignment rule that has been modified
	 */
	public AssignmentRule getAssignmentRule()
	{
		return rule;
	}
	
	/**
	 * Returns the list of changes that have been
	 * made to the assignment rule.
	 * 
	 * @return list of changes
	 */
	public Map<String, String> getChanges()
	{
		return changes;
	}
	
}
