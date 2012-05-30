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

package com.vimofthevine.underbudget.core.ledger;

import java.util.List;

import org.simpleframework.xml.ElementList;
import org.simpleframework.xml.Root;
import org.simpleframework.xml.Version;

import com.vimofthevine.underbudget.core.ledger.transaction.Transaction;
import com.vimofthevine.underbudget.core.ledger.transaction.TransactionList;

/**
 * Ledger of transactions
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
@Root
public class Ledger {
	
	/**
	 * Ledger definition version
	 */
	@Version(revision=1.0)
	private final static double version = 1;
	
	/**
	 * List of transactions
	 */
	@ElementList(type=Transaction.class)
	private TransactionList transactions;
	
	/**
	 * Constructs a new ledger
	 */
	public Ledger()
	{
		transactions = new TransactionList();
	}
	
	/**
	 * Returns a reference to the transaction list
	 * 
	 * @return transaction list
	 */
	public List<Transaction> getTransactionList()
	{
		return transactions;
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj instanceof Ledger)
		{
			Ledger that = (Ledger) obj;
			
			return this.transactions.equals(that.transactions);
		}
		else
			return super.equals(obj);
	}

}
