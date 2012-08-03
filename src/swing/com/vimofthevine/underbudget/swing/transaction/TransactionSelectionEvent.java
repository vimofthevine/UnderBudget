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

import com.vimofthevine.underbudget.core.ledger.transaction.Transaction;
import com.vimofthevine.underbudget.swing.history.SelectionEvent;

/**
 * An event that represents the selection
 * of a transaction.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionSelectionEvent implements SelectionEvent {

	/**
	 * The transaction that has been selected
	 */
	private final Transaction transaction;
	
	/**
	 * Constructs a new transaction selection event.
	 * 
	 * @param transaction transaction that has been selected
	 */
	public TransactionSelectionEvent(Transaction transaction)
	{
		this.transaction = transaction;
	}
	
	/**
	 * Returns the transaction that has been selected.
	 * 
	 * @return transaction that has been selected
	 */
	public Transaction getEstimate()
	{
		return transaction;
	}
	
}
