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
 * A transaction that can be displayed to the user.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface DisplayableTransaction {
	
	/**
	 * Returns a copy of this transaction's
	 * posted date.
	 * 
	 * @return transaction posted date
	 */
	public SimpleDate getPostedDate();
	
	/**
	 * Returns a copy of this transaction's payee.
	 * 
	 * @return transaction payee
	 */
	public String getPayee();
	
	/**
	 * Returns a copy of this transaction's memo.
	 * 
	 * @return transaction memo
	 */
	public String getMemo();
	
	/**
	 * Returns a copy of this transaction's
	 * transfer amount.
	 * 
	 * @return transaction transfer amount
	 */
	public Currency getTransferAmount();
	
	/**
	 * Returns a copy of this transaction's
	 * withdrawal account.
	 * 
	 * @return transaction withdrawal account
	 */
	public Account getWithdrawalAccount();
	
	/**
	 * Returns a copy of this transaction's
	 * deposit account.
	 * 
	 * @return transaction deposit account
	 */
	public Account getDepositAccount();

}
