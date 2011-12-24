package com.vimofthevine.underbudget.transactions;

/**
 * Representation of an actual financial transfer of
 * funds from a withdrawal account into a deposit
 * account
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class Transaction {
	
	/**
	 * Returns the requested field text
	 * 
	 * @param field requested transaction field
	 * @return requested field text
	 */
	public String getFieldText(TransactionField field)
	{
		switch (field)
		{
			case ANY:
				return "";
			case MEMO:
				return "";
			case PAYEE:
				return "";
			case WITHDRAWAL:
				return "";
			case DEPOSIT:
				return "";
			default:
				return "";
		}
	}
	
}
