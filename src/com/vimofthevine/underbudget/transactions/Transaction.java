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
	 * Constructor
	 */
	public Transaction()
	{
		withdrawal = new Account("");
		deposit = new Account("");
	}
	
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
