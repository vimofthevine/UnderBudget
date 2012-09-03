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

import org.simpleframework.xml.Element;
import org.simpleframework.xml.ElementList;

import com.vimofthevine.underbudget.core.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.assignment.Condition;
import com.vimofthevine.underbudget.core.assignment.MutableAssignmentRule;
import com.vimofthevine.underbudget.core.assignment.RuleDefinition;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.transaction.Transaction;
import com.vimofthevine.underbudget.xml.estimate.XmlEstimate;

/**
 * An <code>AssignmentRule</code> implementation using
 * Simple XML annotations for serialization.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class XmlAssignmentRule implements MutableAssignmentRule {
	
	/**
	 * Associated estimate
	 */
	@Element
	private final XmlEstimate estimate;
	
	/**
	 * Rule conditions
	 */
	@ElementList(entry="condition")
	private ArrayList<XmlCondition> conditions;
	
	/**
	 * Constructs a new XML serializable assignment
	 * rule wrapper around the given assignment rule.
	 * 
	 * @param original original assignment rule
	 */
	public XmlAssignmentRule(AssignmentRule original)
	{
		estimate = (XmlEstimate) original.getEstimate();
		conditions = XmlConditions.toList(
			original.getDefinition().getConditions());
	}
	
	/**
	 * Constructs a new XML serializable assignment
	 * rule wrapper around the given assignment rule.
	 * 
	 * @param original original assignment rule
	 * @param root     root estimate
	 */
	public XmlAssignmentRule(AssignmentRule original, XmlEstimate root)
	{
		estimate = findEstimate(root, original.getEstimate());
		conditions = XmlConditions.toList(
			original.getDefinition().getConditions());
	}
	
	XmlAssignmentRule(
		@Element(name="estimate") XmlEstimate estimate,
		@ElementList(name="conditions") ArrayList<XmlCondition> conditions
	)
	{
		this.estimate = estimate;
		this.conditions = conditions;
	}
	
	private XmlEstimate findEstimate(XmlEstimate haystack, Estimate needle)
	{
		ArrayList<Integer> indices = new ArrayList<Integer>();
		
		{
    		Estimate parent = needle.getParent();
    		Estimate child = needle;
    		while (parent != null)
    		{
    			int index = parent.indexOf(child);
    			indices.add(0, index);
    			
    			// Progress up the tree
    			child = parent;
    			parent = parent.getParent();
    		}
		}
		
		{
			XmlEstimate parent = haystack;
			XmlEstimate child = null;
    		for (Integer index : indices)
    		{
    			XmlEstimate oldParent = parent;
    			child = (XmlEstimate) parent.getChildAt(index);
    			parent = oldParent;
    		}
    		
    		if (child != null)
    			return child;
    		else
    			throw new IllegalArgumentException("Could not find equivalent estimate");
		}
	}

	@Override
    public RuleDefinition getDefinition()
    {
		return new RuleDefinition() {
			@Override
            public Condition[] getConditions()
			{
				return conditions.toArray(new Condition[conditions.size()]);
			}
		};
    }

	/**
	 * @TODO this should be somewhere where it can be
	 *       reused among implementations of AssignmentRule
	 */
	@Override
    public boolean matches(Transaction transaction)
    {
		if (conditions.size() == 0)
			return false;
		
		boolean matches = true;
		
		for (Condition condition : conditions)
		{
			matches = matches && transaction.meets(condition);
		}
		
		return matches;
    }

	@Override
    public Estimate getEstimate()
    {
		return estimate;
    }

	@Override
    public void setDefinition(RuleDefinition definition)
    {
		conditions = XmlConditions.toList(definition.getConditions());
    }
	
	@Override
	public String toString()
	{
		StringBuilder buffer = new StringBuilder();
		
		for (Condition condition : conditions)
		{
			buffer.append(condition.toString());
			buffer.append(",");
		}
		
		return "Rule {" + buffer.toString() + "}";
	}

}
