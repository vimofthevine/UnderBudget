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

package com.vimofthevine.underbudget.swing.summary;

import java.util.Currency;

import javax.swing.SwingUtilities;
import javax.swing.table.AbstractTableModel;

import com.vimofthevine.underbudget.core.balance.EndingBalance;
import com.vimofthevine.underbudget.core.balance.EndingBalances;

/**
 * A table model for displaying ending balances and their
 * increases and decreases.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class EndingBalancesTableModel extends AbstractTableModel {
	
	/**
	 * Ending balances
	 */
	private EndingBalance[] balances;
	
	/**
	 * Constructs a new ending balances table model.
	 * 
	 * @param currency currency in use
	 */
	EndingBalancesTableModel(Currency currency)
	{
		balances = new EndingBalance[3];
		
		for (int i=0; i<3; i++)
		{
			balances[i] = new UninitEndingBalance(currency);
		}
	}
	
	/**
	 * Updates the table with new ending balances results.
	 * 
	 * @param results ending balances analysis results
	 */
	void setResults(final EndingBalances results)
	{
		SwingUtilities.invokeLater(new Runnable() {
			public void run()
			{
        		balances[0] = results.getEstimatedEndingBalance();
        		balances[1] = results.getActualEndingBalance();
        		balances[2] = results.getExpectedEndingBalance();
        		
        		fireTableDataChanged();
			}
		});
	}

	@Override
	public Class<?> getColumnClass(int column)
	{
		switch (column)
		{
			case 0:
				return String.class;
				
			default:
				return Currency.class;
		}
	}

	@Override
	public int getColumnCount()
	{
		return 4; // Labels, est, act, exp
	}

	@Override
	public String getColumnName(int column)
	{
		switch (column)
		{
			case 1:
				return "Estimated";
				
			case 2:
				return "Actual/Current";
				
			case 3:
				return "Expected";
				
			default:
				return "";
		}
	}

	@Override
	public int getRowCount()
	{
		return 5; // initial, incr, decr, net, ending
	}

	@Override
	public Object getValueAt(int row, int column)
	{
		if (column == 0)
		{
			switch (row)
			{
				case 0:
					return "Initial Balance";
					
				case 1:
					return "Total Increase";
					
				case 2:
					return "Total Decrease";
					
				case 3:
					return "Net Change";
					
				case 4:
					return "Ending Balance";
			}
		}
		else if (column < 4)
		{
			EndingBalance balance = balances[column-1];
			
			switch (row)
			{
				case 0:
					return balance.getInitialValue();
					
				case 1:
					return balance.getSumIncreases();
					
				case 2:
					return balance.getSumDecreases();
					
				case 3:
					return balance.getNetChange();
					
				case 4:
					return balance.getValue();
			}
		}
		
		return null;
	}

	@Override
	public boolean isCellEditable(int row, int column)
	{
		return false;
	}

	@Override
	public void setValueAt(Object value, int row, int column)
	{
		// Do nothing
	}

}
