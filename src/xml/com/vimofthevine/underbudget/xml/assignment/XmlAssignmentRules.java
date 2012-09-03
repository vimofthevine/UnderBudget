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

package com.vimofthevine.underbudget.xml.assignment;

import java.util.ArrayList;

import org.simpleframework.xml.ElementList;

import com.vimofthevine.underbudget.core.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.assignment.AssignmentRules;
import com.vimofthevine.underbudget.xml.estimate.XmlEstimate;

/**
 * An <code>AssignmentRules</code> implementation using
 * Simple XML annotations for serialization.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class XmlAssignmentRules implements AssignmentRules {
	
	/**
	 * Assignment rules
	 */
	@ElementList(entry="rule",inline=true,required=false)
	private ArrayList<XmlAssignmentRule> rules;
	
	/**
	 * Constructs a new assignment rules list.
	 */
	public XmlAssignmentRules()
	{
		rules = new ArrayList<XmlAssignmentRule>();
	}
	
	/**
	 * Constructs a new assignment rules list
	 * based on the given assignment rules list.
	 * 
	 * @param original original assignment rules list
	 */
	public XmlAssignmentRules(AssignmentRules original, XmlEstimate root)
	{
		this();
		
		for (int i=0; i<original.size(); i++)
		{
			rules.add(new XmlAssignmentRule(original.getAt(i), root));
		}
	}

	@Override
    public int size()
    {
		return rules.size();
    }

	@Override
    public AssignmentRule getAt(int index)
    {
		if (index > -1 && index < rules.size())
			return rules.get(index);
		else
			return null;
    }

	@Override
    public int indexOf(AssignmentRule rule)
    {
		return rules.indexOf(rule);
    }

	@Override
    public void reorder(int originalIndex, int newIndex)
    {
		XmlAssignmentRule rule = rules.get(originalIndex);
		
		if (newIndex > originalIndex)
		{
			rules.remove(originalIndex);
			rules.add(newIndex - 1, rule);
		}
		else
		{
			rules.remove(originalIndex);
			rules.add(newIndex, rule);
		}
    }

	@Override
    public void append(AssignmentRule rule)
    {
		XmlAssignmentRule newRule = new XmlAssignmentRule(rule);
		rules.add(newRule);
    }

	@Override
    public void insert(AssignmentRule rule, int index)
    {
		XmlAssignmentRule newRule = new XmlAssignmentRule(rule);
		rules.add(index, newRule);
    }

	@Override
    public void remove(AssignmentRule rule)
    {
		rules.remove(rule);
    }

}
