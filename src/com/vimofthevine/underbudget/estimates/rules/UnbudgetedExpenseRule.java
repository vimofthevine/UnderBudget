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

package com.vimofthevine.underbudget.estimates.rules;

import com.vimofthevine.underbudget.transactions.Transaction;

/**
 * Custom rule to catch expense transactions
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class UnbudgetedExpenseRule extends Rule {
	
	@Override
	public boolean matches(Transaction transaction)
	{
		String withdrawal = transaction.withdrawal.getName().toLowerCase();
		String deposit = transaction.deposit.getName().toLowerCase();
		
		// Expense transactions
		if ((withdrawal.startsWith("asset") || withdrawal.startsWith("liabi"))
				&& deposit.startsWith("expen"))
			return true;
		
		// Else not an expense transaction
		return false;
	}
	
	@Override
	public String toString()
	{
		return "Unbudgeted expense";
	}

}
