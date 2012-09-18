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

package com.vimofthevine.underbudget.swing.transaction.events;

import com.vimofthevine.underbudget.core.transaction.Transaction;

/**
 * Event generated when transactions have been imported.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionsImportedEvent {
	
	/**
	 * Imported transactions
	 */
	private final Transaction[] transactions;
	
	/**
	 * Constructs a new transactions-imported event.
	 * 
	 * @param transactions imported transactions
	 */
	public TransactionsImportedEvent(Transaction[] transactions)
	{
		this.transactions = transactions;
	}
	
	/**
	 * Get the imported transactions.
	 * 
	 * @return imported transactions
	 */
	public Transaction[] getTransactions()
	{
		return transactions;
	}

}
