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

package com.vimofthevine.underbudget.core.transaction.source;

import com.vimofthevine.underbudget.core.assignment.Condition;
import com.vimofthevine.underbudget.core.assignment.TransactionField;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.date.SimpleDate;
import com.vimofthevine.underbudget.core.transaction.Transaction;
import com.vimofthevine.underbudget.core.transaction.TransactionDefinition;
import com.vimofthevine.underbudget.core.transaction.TransferAccount;

/**
 * Immutable imported transaction.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class ImportedTransaction implements Transaction,
Comparable<ImportedTransaction> {
	
	/**
	 * Transaction posted date
	 */
	private final SimpleDate date;
	
	/**
	 * Transaction payee
	 */
	private final String payee;
	
	/**
	 * Transaction memo
	 */
	private final String memo;
	
	/**
	 * Transfer amount
	 */
	private final Currency amount;
	
	/**
	 * Withdrawal account
	 */
	private final TransferAccount withdrawal;
	
	/**
	 * Deposit account
	 */
	private final TransferAccount deposit;
	
	/**
	 * Constructs a new imported transaction.
	 * 
	 * @param date       transaction posted date
	 * @param payee      transaction payee
	 * @param memo       transaction memo
	 * @param value      transfer amount
	 * @param withdrawal withdrawal account
	 * @param deposit    deposit account
	 */
	ImportedTransaction(SimpleDate date, String payee,
		String memo, Currency value, TransferAccount withdrawal,
		TransferAccount deposit)
	{
		this.date = date;
		this.payee = payee;
		this.memo = memo;
		this.amount = value;
		this.withdrawal = withdrawal;
		this.deposit = deposit;
	}

	@Override
	public TransactionDefinition getDefinition()
	{
		return new TransactionDefinition() {

			@Override
            public SimpleDate getPostedDate() { return date; }

			@Override
            public String getPayee() { return payee; }

			@Override
            public String getMemo() { return memo; }

			@Override
            public TransferAccount getWithdrawalAccount() { return withdrawal; }

			@Override
            public TransferAccount getDepositAccount() { return deposit; }

			@Override
            public Currency getTransferAmount() { return amount; }
			
		};
	}

	@Override
	public boolean meets(Condition condition)
	{
		final String field = getField(condition.getField());
		
		if ( ! field.equals(""))
		{
			return condition.satisfies(field);
		}
		
		return false;
	}
	
	private String getField(TransactionField field)
	{
		switch (field)
		{
			case PAYEE:
				return payee;
				
			case MEMO:
				return memo;
				
			case WITHDRAWAL:
				return withdrawal.getName();
				
			case DEPOSIT:
				return deposit.getName();
				
			default:
				return "";
		}
	}

	@Override
    public int compareTo(ImportedTransaction that)
    {
		return this.date.compareTo(that.date);
    }

}
