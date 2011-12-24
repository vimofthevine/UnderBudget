package com.vimofthevine.underbudget.transactions;

/**
 * Enumeration for the match-able fields of
 * a transaction
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public enum TransactionField {
	NONE,
	ANY,
	MEMO,
	PAYEE,
	WITHDRAWAL,
	DEPOSIT;
	
	/**
	 * Returns an array of values, suitable for
	 * a user-selection list
	 * 
	 * @return user-selection values
	 */
	public static String[] userValues()
	{
		return new String[] {
			"Any", "Memo", "Payee",
			"Withdrawal Account", "Deposit Account"
		};
	}
	
}
