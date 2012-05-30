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

package com.vimofthevine.underbudget.core.ledger.transaction;

import org.simpleframework.xml.Element;
import org.simpleframework.xml.Root;

import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.ledger.Account;
import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * A transaction that has been imported from an
 * external financial data source and is immutable.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
@Root(name="transaction")
public class ImportedTransaction implements Transaction {
	
	/**
	 * Date on which this transaction
	 * was posted
	 */
	@Element(name="date-posted")
	private final SimpleDate date;
	
	/**
	 * Description of the recipient of the
	 * transfered funds
	 */
	@Element
	private final String payee;
	
	/**
	 * Description of the transaction
	 */
	@Element
	private final String memo;
	
	/**
	 * Amount transfered
	 */
	@Element
	private final Currency amount;
	
	/**
	 * Account from which funds were withdrawn
	 */
	@Element(name="withdrawal-acct")
	private final Account withdrawal;
	
	/**
	 * Account from which funds were deposited
	 */
	@Element(name="deposit-acct")
	private final Account deposit;
	
	/**
	 * Constructs a new transaction with the
	 * given fields.
	 * 
	 * @param date       transaction date
	 * @param payee      transaction payee
	 * @param memo       transaction memo
	 * @param amount     transfer amount
	 * @param withdrawal account from which currency is transfered
	 * @param deposit    account into which currency is transfered
	 */
	public ImportedTransaction(@Element(name="date-posted") SimpleDate date,
		@Element(name="payee") String payee, @Element(name="memo") String memo,
		@Element(name="amount") Currency amount,
		@Element(name="withdrawal-acct") Account withdrawal,
		@Element(name="deposit-acct") Account deposit)
	{
		this.date = date;
		this.payee = payee;
		this.memo = memo;
		this.amount = amount;
		this.withdrawal = withdrawal;
		this.deposit = deposit;
	}
	
	/**
	 * Compares the given transaction, according to date.
	 * This is appropriate for sorting transactions.
	 * 
	 * @param transaction transaction to compare
	 */
	@Override
	public int compareTo(Transaction transaction)
	{
		// Have to flip the sign
		return -1 * transaction.compareTo(date);
	}
	
	// -- Methods from the AnalyzableTransaction interface
	
	@Override
	public String getFieldText(TransactionField field)
	{
		switch (field)
		{
			case ANY:
				return memo + "," + payee + ","
					+ withdrawal.getName() + "," + deposit.getName();
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
	public void addTransferAmountTo(Currency total)
	{
		total.increaseBy(amount);
	}
	
	@Override
	public int compareTo(SimpleDate date)
	{
		return this.date.compareTo(date);
	}
	
	@Override
	public boolean isWithin(BudgetingPeriod period)
	{
		return period.contains(date);
	}
	
	// -- Methods from the DisplayableTransaction interface

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
    public Currency getTransferAmount()
    {
		return amount;
    }

	@Override
    public Account getWithdrawalAccount()
    {
		return withdrawal;
    }

	@Override
    public Account getDepositAccount()
    {
		return deposit;
    }
	
	// -- Methods from the MutableTransaction interface
	
	@Override
    public void setPostedDate(SimpleDate date)
    {
		// Do nothing (immutable)
    }

	@Override
    public void setPayee(String payee)
    {
		// Do nothing (immutable)
    }

	@Override
    public void setMemo(String memo)
    {
		// Do nothing (immutable)
    }

	@Override
    public void setTransferAmount(Currency amount)
    {
		// Do nothing (immutable)
    }

	@Override
    public void setWithdrawalAccount(Account account)
    {
		// Do nothing (immutable)
    }

	@Override
    public void setDepositAccount(Account account)
    {
		// Do nothing (immutable)
    }

	@Override
	public boolean equals(Object obj)
	{
		if (this == obj)
			return true;
		if ( ! (obj instanceof ImportedTransaction))
			return false;
		
		ImportedTransaction that = (ImportedTransaction) obj;
		return this.date.equals(that.date)
			&& this.payee.equals(that.payee)
			&& this.memo.equals(that.memo)
			&& this.amount.equals(that.amount)
			&& this.withdrawal.equals(that.withdrawal)
			&& this.deposit.equals(that.deposit);
	}
	
	@Override
	public int hashCode()
	{
		int result = 14;
		result = 31 * result + date.hashCode();
		result = 31 * result + payee.hashCode();
		result = 31 * result + memo.hashCode();
		result = 31 * result + amount.hashCode();
		result = 31 * result + deposit.hashCode();
		result = 31 * result + withdrawal.hashCode();
		return result;
	}
	
	@Override
	public String toString()
	{
		return date + "," + amount + "," + payee + " (" + memo + ")";
	}

}
