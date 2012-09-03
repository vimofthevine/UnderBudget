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

package com.vimofthevine.underbudget.swing.budget;

import java.util.Map;

import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.swing.history.ModificationEvent;

/**
 * An event that represents a change to
 * the current budget.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetModifiedEvent implements ModificationEvent {
	
	/**
	 * The budget that has changed.
	 */
	private final Budget budget;
	
	/**
	 * A map of fields that were changed, where
	 * the index is the field name and the value
	 * is the old value of the field.
	 */
	private final Map<String, Object> changes;
	
	/**
	 * Constructs a new budget modification event.
	 * 
	 * @param budget  budget that has been modified
	 * @param changes list of changes that have been made
	 */
	public BudgetModifiedEvent(Budget budget,
		Map<String, Object> changes)
	{
		this.budget = budget;
		this.changes = changes;
	}
	
	/**
	 * Returns the budget that has been modified.
	 * 
	 * @return budget that has been modified
	 */
	public Budget getBudget()
	{
		return budget;
	}
	
	/**
	 * Returns the list of changes that have been
	 * made to the budget.
	 * 
	 * @return list of changes
	 */
	public Map<String, Object> getChanges()
	{
		return changes;
	}
	
	@Override
	public String toString()
	{
		return budget + " modified " + changes;
	}

}
