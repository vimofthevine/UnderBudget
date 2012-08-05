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

import javax.swing.table.AbstractTableModel;

import com.vimofthevine.underbudget.core.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.assignment.AssignmentRules;
import com.vimofthevine.underbudget.core.assignment.Condition;

/**
 * A table model for displaying the prioritized list
 * of assignment rules.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AssignmentRulesTableModel extends AbstractTableModel {
	
	/**
	 * Assiginment rules
	 */
	private final AssignmentRules rules;
	
	/**
	 * Constructs an assignment rules table model.
	 * 
	 * @param rules assignment rules
	 */
	public AssignmentRulesTableModel(AssignmentRules rules)
	{
		this.rules = rules;
	}

	@Override
    public Class<?> getColumnClass(int column)
    {
		return String.class;
    }

	@Override
    public int getColumnCount()
    {
	    return 2;
    }

	@Override
    public String getColumnName(final int column)
    {
		switch (column)
		{
			case 0:
				return "Conditions";
				
			case 1:
				return "Assign To";
				
			default:
				return "";
		}
    }

	@Override
    public int getRowCount()
    {
		return rules.size();
    }

	@Override
    public Object getValueAt(int row, int column)
    {
		AssignmentRule rule = rules.getAt(row);
		
		if (rule != null)
		{
			switch (column)
			{
				case 0:
					return createConditionDescription(rule);
					
				case 1:
					return rule.getEstimate().getDefinition().getName();
					
				default:
					return null;
			}
		}
		
	    return null;
    }

	@Override
    public boolean isCellEditable(int arg0, int arg1)
    {
	    return false;
    }

	@Override
    public void setValueAt(Object arg0, int arg1, int arg2)
    {
		// Do nothing
    }
	
	private String createConditionDescription(AssignmentRule rule)
	{
		Condition[] conditions = rule.getDefinition().getConditions();
		
		if (conditions.length == 0)
			return "No conditions";
		else if (conditions.length == 1)
		{
			Condition condition = conditions[0];
    		StringBuilder buffer = new StringBuilder();
    		buffer.append("Where ");
			buffer.append(condition.getField());
			buffer.append(" ");
			buffer.append(condition.getOperator());
			buffer.append(" ");
			buffer.append(condition.getValue());
			return buffer.toString();
		}
		else
		{
			StringBuilder buffer = new StringBuilder();
			buffer.append("Compound conditions (");
			boolean first = true;
			
    		for (Condition condition : conditions)
    		{
    			if ( ! first)
    			{
    				buffer.append(", ");
    			}
    			first = false;
    			
    			buffer.append(condition.getField().toString().charAt(0));
    			buffer.append(":");
    			buffer.append(condition.getValue());
    		}
    		
    		buffer.append(")");
    		
    		return buffer.toString();
		}
	}
	
	void reorder(int fromRow, int toRow)
	{
		rules.reorder(fromRow, toRow);
	}

}
