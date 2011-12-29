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
