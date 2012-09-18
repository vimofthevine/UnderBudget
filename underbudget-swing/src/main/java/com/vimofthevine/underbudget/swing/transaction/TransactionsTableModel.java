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

package com.vimofthevine.underbudget.swing.transaction;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import javax.swing.table.AbstractTableModel;

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.date.SimpleDate;
import com.vimofthevine.underbudget.core.transaction.Transaction;
import com.vimofthevine.underbudget.core.transaction.TransactionDefinition;
import com.vimofthevine.underbudget.core.transaction.TransferAccount;

/**
 * A table model for displaying imported transactions.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionsTableModel extends AbstractTableModel {
	
	/**
	 * Transactions
	 */
	private final List<Transaction> transactions;
	
	/**
	 * Column names
	 */
	private final String[] columnNames;
	
	/**
	 * Column classes
	 */
	private final Class<?>[] columnClasses;
	
	/**
	 * Constructs a transactions table model
	 */
	public TransactionsTableModel()
	{
		transactions = new ArrayList<Transaction>();
		
		columnNames = new String[] {
			"Date",
			"Description",
			"From",
			"To",
			"Amount",
		};
		
		columnClasses = new Class<?>[] {
			SimpleDate.class,
			String.class,
			TransferAccount.class,
			TransferAccount.class,
			CashCommodity.class,
		};
	}
	
	/**
	 * Specifies the new set of imported transactions
	 * to be displayed by the table.
	 * 
	 * @param newTransactions new list of imported transactions
	 */
	public synchronized void set(Transaction[] newTransactions)
	{
		transactions.clear();
		transactions.addAll(Arrays.asList(newTransactions));
		fireTableDataChanged();
	}

	@Override
	public Class<?> getColumnClass(int column)
	{
		if (column >= 0 && column < columnClasses.length)
			return columnClasses[column];
		
		return Object.class;
	}

	@Override
	public int getColumnCount()
	{
		return columnNames.length;
	}

	@Override
	public String getColumnName(int column)
	{
		if (column >=0 && column < columnNames.length)
			return columnNames[column];
		
		return "";
	}

	@Override
	public synchronized int getRowCount()
	{
		return transactions.size();
	}

	@Override
	public synchronized Object getValueAt(int row, int column)
	{
		if (row < 0 || row >= transactions.size())
			return null;
		
		Transaction transaction = transactions.get(row);
		if (transaction != null)
		{
			TransactionDefinition definition = transaction.getDefinition();
			
			switch (column)
			{
    			case 0:
    				return definition.getPostedDate();
    				
    			case 1:
    				return definition.getPayee();
    				
    			case 2:
    				return definition.getWithdrawalAccount();
    				
    			case 3:
    				return definition.getDepositAccount();
    				
    			case 4:
    				return definition.getTransferAmount();
    				
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

}
