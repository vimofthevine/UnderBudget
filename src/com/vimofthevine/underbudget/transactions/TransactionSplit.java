package com.vimofthevine.underbudget.transactions;

import java.math.BigDecimal;

/**
 * Representation of a transaction split
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionSplit {
	
	/**
	 * Amount of money transferring into/from this split
	 */
	public BigDecimal value;
	
	/**
	 * Memo field of this split
	 */
	public String memo = "";
	
	/**
	 * Account into/from which money is being transferred
	 */
	public Account account;

}
