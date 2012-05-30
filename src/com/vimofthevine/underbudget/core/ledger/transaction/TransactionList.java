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

package com.vimofthevine.underbudget.core.ledger.transaction;

import java.util.ArrayList;

import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;

/**
 * List of transactions
 */
public class TransactionList extends ArrayList<Transaction> {
	
	/**
	 * Budgeting period to be used for filtering transactions
	 */
	private BudgetingPeriod period;
	
	/**
	 * Constructs a new transaction list
	 */
	public TransactionList()
	{
		period = null;
	}
	
	/**
	 * Constructs a new transaction list to be filtered
	 * with the given budgeting period.
	 * 
	 * @param period budgeting period to use for filtering
	 */
	public TransactionList(BudgetingPeriod period)
	{
		this.period = period;
	}
	
	@Override
	public boolean add(Transaction transaction)
	{
		if (period == null || transaction.isWithin(period))
			return super.add(transaction);
		else
			return false;
	}

}
