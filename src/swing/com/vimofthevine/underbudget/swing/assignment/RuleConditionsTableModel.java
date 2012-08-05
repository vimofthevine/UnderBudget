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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import javax.swing.table.AbstractTableModel;

import com.vimofthevine.underbudget.core.assignment.Condition;
import com.vimofthevine.underbudget.core.assignment.EvaluationOperator;
import com.vimofthevine.underbudget.core.assignment.TransactionField;

/**
 * A table model for displaying the conditions
 * of an individual assignment rule.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class RuleConditionsTableModel extends AbstractTableModel {
	
	/**
	 * Rule conditions
	 */
	private final List<Condition> conditions;
	
	/**
	 * Registered conditions-changed listeners
	 */
	private final List<ConditionsChangedListener> listeners;
	
	/**
	 * Constructs a new conditions table model.
	 */
	RuleConditionsTableModel()
	{
		conditions = new ArrayList<Condition>();
		listeners = new ArrayList<ConditionsChangedListener>();
	}
	
	/**
	 * Specifies the list of conditions to be displayed
	 * in the table (i.e., a new rule has been selected).
	 * 
	 * @param newConditions list of conditions
	 */
	void setConditions(Condition[] newConditions)
	{
		conditions.clear();
		conditions.addAll(Arrays.asList(newConditions));
		fireTableDataChanged();
	}
	
	/**
	 * Adds a new condition to the list of conditions.
	 * 
	 * @param condition new condition
	 */
	void addCondition(Condition condition)
	{
		int addedRow = conditions.size();
		conditions.add(condition);
		fireTableRowsInserted(addedRow, addedRow);
	}
	
	/**
	 * Removes the condition located at the specified
	 * row.
	 * 
	 * @param row row of the condition to be removed
	 */
	void removeCondition(int row)
	{
		if (row < 0 || row >= conditions.size())
			return;
		
		conditions.remove(row);
		fireTableRowsDeleted(row, row);
	}
	
	/**
	 * Returns the conditions currently defined
	 * in the table, including all user modifications.
	 * 
	 * @return rule conditions in the table
	 */
	Condition[] getConditions()
	{
		return conditions.toArray(new Condition[conditions.size()]);
	}
	
	/**
	 * Registers a listener to receive notification when
	 * the conditions have been modified by the user.
	 * 
	 * @param listener conditions-changed event listener
	 */
	void addListener(ConditionsChangedListener listener)
	{
		synchronized(listeners)
		{
			listeners.add(listener);
		}
	}
	
	/**
	 * Unregisters a listener from receiving notification when
	 * the conditions have been modified by the user.
	 * 
	 * @param listener conditions-changed event listener
	 */
	void removeListener(ConditionsChangedListener listener)
	{
		synchronized(listeners)
		{
			listeners.remove(listener);
		}
	}

	@Override
    public Class<?> getColumnClass(int column)
    {
		switch (column)
		{
    		case 0:
    			return TransactionField.class;
    		case 1:
    			return EvaluationOperator.class;
    		case 2:
    			return String.class;
    		default:
    			return Object.class;
		}
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
		return conditions.size();
    }

	@Override
    public Object getValueAt(int row, int column)
    {
		if (row < 0 || row >= conditions.size())
			return null;
		
		Condition condition = conditions.get(row);
		if (condition == null)
			return null;
		
		switch (column)
		{
    		case 0:
    			return condition.getField();
    		case 1:
    			return condition.getOperator();
    		case 2:
    			return condition.getValue();
    		default:
    			return null;
		}
    }

	@Override
    public boolean isCellEditable(int arg0, int arg1)
    {
		return true;
    }

	@Override
    public void setValueAt(Object value, int row, int column)
    {
		if (row < 0 || row >= conditions.size())
			return;
		
		final Condition condition = conditions.get(row);
		if (condition == null)
			return;
		
		switch (column)
		{
    		case 0:
    			if ( ! (value instanceof TransactionField))
    				break;
    			
    			conditions.remove(row);
    			conditions.add(row, new UserCondition(condition,
    				(TransactionField) value));
    			notifyListeners();
    			
    			break;
    			
    		case 1:
    			if ( ! (value instanceof EvaluationOperator))
    				break;
    			
    			conditions.remove(row);
    			conditions.add(row, new UserCondition(condition,
    				(EvaluationOperator) value));
    			notifyListeners();
    			
    			break;
    			
    		case 2:
    			if ( ! (value instanceof String))
    				break;
    			
    			conditions.remove(row);
    			conditions.add(row, new UserCondition(condition,
    				(String) value));
    			notifyListeners();
    			
    			break;
		}
    }
	
	/**
	 * Notifies all registered change listeners
	 * of the new rule conditions.
	 */
	private void notifyListeners()
	{
		synchronized(listeners)
		{
			Condition[] conditions = getConditions();
			
			for (ConditionsChangedListener listener : listeners)
			{
				listener.conditionsChanged(conditions);
			}
		}
	}

}
