package com.vimofthevine.underbudget.estimates.rules;

import com.vimofthevine.underbudget.transactions.Transaction;

/**
 * Custom rule to catch income transactions
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class UnbudgetedIncomeRule extends Rule {

	@Override
	public boolean matches(Transaction transaction)
	{
		String withdrawal = transaction.withdrawal.getName().toLowerCase();
		String deposit = transaction.deposit.getName().toLowerCase();
		
		// Income transactions
		if (withdrawal.startsWith("incom") && deposit.startsWith("asset"))
			return true;
		// Refund transactions
		if (withdrawal.startsWith("expen") &&
				(deposit.startsWith("asset") || deposit.startsWith("liabi")))
			return true;
		// Loan transactins
		if (withdrawal.startsWith("liabi") && deposit.startsWith("asset"))
			return true;
		
		// Else not an income transaction
		return false;
	}
	
	@Override
	public String toString()
	{
		return "Unbudgeted income";
	}
	
}
