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
	
	/**
	 * Converts a given string to the
	 * appropriate corresponding enumeration
	 * 
	 * @param string string representation of a field
	 * @return transaction field enumeration
	 */
	public static TransactionField getValueOf(String string)
	{
		string = string.toLowerCase();
		
		if (string.contains("any"))
			return ANY;
		else if (string.contains("memo"))
			return MEMO;
		else if (string.contains("payee"))
			return PAYEE;
		else if (string.contains("withdrawal"))
			return WITHDRAWAL;
		else if (string.contains("deposit"))
			return DEPOSIT;
		else
			return NONE;
	}
	
}
