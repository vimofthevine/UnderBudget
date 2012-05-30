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

import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.ledger.Account;
import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * A transaction that can be modified by the
 * user at runtime.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface MutableTransaction {

	/**
	 * Specifies the posted date of
	 * this transaction.
	 * 
	 * @param date new posted date for this transaction
	 */
	public void setPostedDate(SimpleDate date);
	
	/**
	 * Specifies the payee of this transaction.
	 * 
	 * @param payee new payee for this transaction
	 */
	public void setPayee(String payee);
	
	/**
	 * Specifies the memo of this transaction.
	 * 
	 * @param memo new memo for this transaction
	 */
	public void setMemo(String memo);
	
	/**
	 * Specifies the transfer amount of this transaction.
	 * 
	 * @param amount new transfer amount for this transaction
	 */
	public void setTransferAmount(Currency amount);
	
	/**
	 * Specifies the withdrawal account of this transaction.
	 * 
	 * @param account new withdrawal account
	 */
	public void setWithdrawalAccount(Account account);
	
	/**
	 * Specifies the deposit account of this transaction.
	 * 
	 * @param account new deposit account
	 */
	public void setDepositAccount(Account account);
	
}
