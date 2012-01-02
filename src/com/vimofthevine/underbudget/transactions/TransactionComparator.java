package com.vimofthevine.underbudget.transactions;

import java.util.Comparator;

/**
 * Comparator for sorting transactions by date
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionComparator implements Comparator<Transaction> {

	/**
	 * Compares two given transactions by date-posted
	 */
	@Override
    public int compare(Transaction arg0, Transaction arg1)
    {
		return arg0.date.compareTo(arg1.date);
    }

}
