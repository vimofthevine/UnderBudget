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
import com.vimofthevine.underbudget.core.assignment.Condition;

/**
 * A table model for displaying assigment rules associated
 * to a particular, individual estimate.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class EstimateRulesTableModel extends AbstractTableModel {
	
	/**
	 * Assignment rules
	 */
	private AssignmentRule[] rules;
	
	/**
	 * Constructs a new estimate assignment
	 * rules table model.
	 */
	EstimateRulesTableModel()
	{
		rules = new AssignmentRule[0];
	}
	
	/**
	 * Updates the list of displayed assignment rules.
	 * 
	 * @param rules list of assignment rules
	 */
	void setRules(AssignmentRule[] rules)
	{
		this.rules = rules;
		fireTableDataChanged();
	}

	@Override
	public Class<?> getColumnClass(int arg0)
	{
		return String.class;
	}

	@Override
	public int getColumnCount()
	{
		return 3;
	}

	@Override
	public String getColumnName(int column)
	{
		switch (column)
		{
    		case 0:
    			return "Field";
    		case 1:
    			return "Operator";
    		case 2:
    			return "Value";
    		default:
    			return "";
		}
	}

	@Override
	public int getRowCount()
	{
		return rules.length;
	}

	@Override
	public Object getValueAt(int row, int column)
	{
		if (row < 0 || row >= rules.length)
			return null;
		
		AssignmentRule rule = rules[row];
		if (rule == null)
			return null;
		
		Condition[] conditions = rule.getDefinition().getConditions();
		
		switch (column)
		{
    		case 0:
    			return buildFieldString(conditions);
    			
    		case 1:
    			return buildOperatorString(conditions);
    			
    		case 2:
    			return buildValueString(conditions);
    			
    		default:
    			return null;
		}
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
	
	private String buildFieldString(Condition[] conditions)
	{
		StringBuilder buffer = new StringBuilder();
		boolean first = true;
		
		for (Condition condition : conditions)
		{
			if ( ! first)
			{
				buffer.append("/");
			}
			first = false;
			
			buffer.append(condition.getField());
		}
		
		if (conditions.length == 0)
		{
			buffer.append("None");
		}
		
		return buffer.toString();
	}
	
	private String buildOperatorString(Condition[] conditions)
	{
		StringBuilder buffer = new StringBuilder();
		boolean first = true;
		
		for (Condition condition : conditions)
		{
			if ( ! first)
			{
				buffer.append("/");
			}
			first = false;
			
			buffer.append(condition.getOperator());
		}
		
		if (conditions.length == 0)
		{
			buffer.append("None");
		}
		
		return buffer.toString();
	}
	
	private String buildValueString(Condition[] conditions)
	{
		StringBuilder buffer = new StringBuilder();
		boolean first = true;
		
		for (Condition condition : conditions)
		{
			if ( ! first)
			{
				buffer.append("/");
			}
			first = false;
			
			buffer.append(condition.getValue());
		}
		
		if (conditions.length == 0)
		{
			buffer.append("None");
		}
		
		return buffer.toString();
	}

}
