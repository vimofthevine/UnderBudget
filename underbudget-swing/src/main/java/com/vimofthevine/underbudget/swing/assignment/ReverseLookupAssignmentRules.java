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

package com.vimofthevine.underbudget.swing.assignment;

import java.util.Collection;

import com.google.common.collect.LinkedListMultimap;
import com.google.common.collect.Multimap;
import com.vimofthevine.underbudget.core.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.assignment.AssignmentRules;
import com.vimofthevine.underbudget.core.estimate.Estimate;

/**
 * An wrapper around an actual assignment rule list that
 * keeps a map of estimates-to-rules.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ReverseLookupAssignmentRules implements AssignmentRules {
	
	/**
	 * Original assignment rules list
	 */
	private final AssignmentRules rules;
	
	/**
	 * Multi-map of estimates to rules
	 */
	private final Multimap<Estimate,AssignmentRule> map;
	
	/**
	 * Constructs a reverse-lookup variation of a
	 * given assignment rules list.
	 * 
	 * @param original original assignment rules list
	 */
	public ReverseLookupAssignmentRules(AssignmentRules original)
	{
		rules = original;
		// Use linked-list because it allows duplicates (clones)
		map = LinkedListMultimap.create(original.size());
		
		for (int i=0; i<original.size(); i++)
		{
			AssignmentRule rule = original.getAt(i);
			map.put(rule.getEstimate(), rule);
		}
	}
	
	/**
	 * Returns the list of assignment rules associated
	 * with a given estimate.
	 * 
	 * @param estimate estimate of interest
	 * @return list of assignment rules associated with
	 *         the given estimate
	 */
	public AssignmentRule[] getRules(Estimate estimate)
	{
		Collection<AssignmentRule> assocRules = map.get(estimate);
		return assocRules.toArray(new AssignmentRule[assocRules.size()]);
	}

	@Override
    public int size()
    {
		return rules.size();
    }

	@Override
    public AssignmentRule getAt(int index)
    {
		return rules.getAt(index);
    }

	@Override
    public int indexOf(AssignmentRule rule)
    {
		return rules.indexOf(rule);
    }

	@Override
    public void reorder(int originalIndex, int newIndex)
    {
		rules.reorder(originalIndex, newIndex);
    }

	@Override
    public void append(AssignmentRule rule)
    {
		rules.append(rule);
		map.put(rule.getEstimate(), rule);
    }

	@Override
    public void insert(AssignmentRule rule, int index)
    {
		rules.insert(rule, index);
		map.put(rule.getEstimate(), rule);
    }

	@Override
    public void remove(AssignmentRule rule)
    {
		rules.remove(rule);
		map.remove(rule.getEstimate(), rule);
    }

}
