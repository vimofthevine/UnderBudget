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

package com.vimofthevine.underbudget.stubs.transaction;

import com.vimofthevine.underbudget.core.assignment.Condition;
import com.vimofthevine.underbudget.core.assignment.TransactionField;
import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.date.SimpleDate;
import com.vimofthevine.underbudget.core.transaction.Transaction;
import com.vimofthevine.underbudget.core.transaction.TransactionDefinition;
import com.vimofthevine.underbudget.core.transaction.TransferAccount;

/**
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class StubTransaction implements Transaction {
	
	private final SimpleDate date;
	private final String payee;
	private final String memo;
	private final String withdrawal;
	private final String deposit;
	private final CashCommodity amount;
	
	public StubTransaction(SimpleDate date, String payee,
		String memo, String withdrawal, String deposit,
		CashCommodity amount)
	{
		this.date = date;
		this.payee = payee;
		this.memo = memo;
		this.withdrawal = withdrawal;
		this.deposit = deposit;
		this.amount = amount;
	}

	@Override
	public TransactionDefinition getDefinition()
	{
		return new TransactionDefinition() {

			@Override
            public SimpleDate getPostedDate()
            {
				return date;
            }

			@Override
            public String getPayee()
            {
				return payee;
            }

			@Override
            public String getMemo()
            {
				return memo;
            }

			@Override
            public TransferAccount getWithdrawalAccount()
            {
				return new TransferAccount() {
					@Override
                    public String getName()
                    {
						return withdrawal;
                    }
				};
            }

			@Override
            public TransferAccount getDepositAccount()
            {
				return new TransferAccount() {
					@Override
                    public String getName()
                    {
						return deposit;
                    }
				};
            }

			@Override
            public CashCommodity getTransferAmount()
            {
				return amount;
            }
			
		};
	}

	@Override
	public boolean meets(Condition condition)
	{
		final String field = getField(condition.getField());
		
		if ( ! field.equals(""))
		{
			final String value = condition.getValue();
			switch (condition.getOperator())
			{
				case BEGINS_WITH:
					return field.startsWith(value);
					
				case EQUALS:
					return field.equalsIgnoreCase(value);
					
				case EQUALS_CASE:
					return field.equals(value);
					
				case CONTAINS:
					return field.contains(value);
					
				case ENDS_WITH:
					return field.endsWith(value);
					
				default:
					return false;
			}
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
				return withdrawal;
				
			case DEPOSIT:
				return deposit;
				
			default:
				return "";
		}
	}

}
