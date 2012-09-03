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

package com.vimofthevine.underbudget.core.assignment;

/**
 * User-specified, prioritized list of assignment rules.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface AssignmentRules {
	
	/**
	 * Returns the number of assignment rules.
	 * 
	 * @return number of assignment rules
	 */
	public int size();
	
	/**
	 * Returns the assignment rule at the specified index.
	 * 
	 * @param index index of the assignment rule to be
	 *        retrieved
	 * @return the assignment rule at the specified index,
	 *         else <code>null</code> if the index is invalid
	 */
	public AssignmentRule getAt(int index);
	
	/**
	 * Returns the first index at which the given
	 * assignment rule is located. If the rule occurs
	 * in the list multiple times (cloned), only the
	 * first instance's index is returned.
	 * 
	 * @param rule assignment rule for which to
	 *        retrieve its index
	 * @return index of the assignment rule, else
	 *         <code>-1</code> if the rule could not be found
	 */
	public int indexOf(AssignmentRule rule);
	
	/**
	 * Moves the assignment rule located at the original
	 * index to the new index.
	 * 
	 * @param originalIndex original index of the rule
	 * @param newIndex      new index of the rule
	 */
	public void reorder(int originalIndex, int newIndex);
	
	/**
	 * Appends the given assignment rule to the end of
	 * the assignment rules list.
	 * 
	 * @param rule assignment rule to be added to the
	 *        end of the list
	 */
	public void append(AssignmentRule rule);
	
	/**
	 * Inserts the given assignment rule into the list
	 * at the specified index.
	 * 
	 * @param rule  assignment rule to be inserted into the list
	 * @param index insertion point of the assignment rule
	 */
	public void insert(AssignmentRule rule, int index);
	
	/**
	 * Removes the first instance of the given
	 * assignment rule from the list.
	 * 
	 * @param rule assignment rule to be removed from the list
	 */
	public void remove(AssignmentRule rule);

}
