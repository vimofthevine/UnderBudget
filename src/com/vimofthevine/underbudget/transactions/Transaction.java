package com.vimofthevine.underbudget.transactions;

import java.math.BigDecimal;
import java.util.Date;

import com.vimofthevine.underbudget.util.FormatHelper;

/**
 * Representation of an actual financial transfer of
 * funds from a withdrawal account into a deposit
 * account
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class Transaction {
	
	/**
	 * Transaction date
	 */
	public Date date;
	
	/**
	 * Transaction payee
	 */
	public String payee = "";
	
	/**
	 * Transaction memo
	 */
	public String memo = "";
	
	/**
	 * Transaction amount
	 */
	public BigDecimal value;
	
	/**
	 * Transaction withdrawal account
	 */
	public Account withdrawal;
	
	/**
	 * Transaction deposit account
	 */
	public Account deposit;
	
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
				return memo + "," + payee + "," + withdrawal.getName() + "," + deposit.getName();
			case MEMO:
				return memo;
			case PAYEE:
				return payee;
			case WITHDRAWAL:
				return withdrawal.getName();
			case DEPOSIT:
				return deposit.getName();
			default:
				return "";
		}
	}
	
	@Override
	public String toString()
	{
		return FormatHelper.formatDate(date) + " " + payee + " " + FormatHelper.formatCurrency(value);
	}
	
}
