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

package com.vimofthevine.underbudget.core.transaction;

import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.date.SimpleDate;

/**
 * Set of attributes defining an individual transaction.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface TransactionDefinition {
	
	/**
	 * Returns the date on which the transaction occurred.
	 * 
	 * @return transaction posted date
	 */
	public SimpleDate getPostedDate();
	
	/**
	 * Returns the payee of the transaction.
	 * 
	 * @return transaction payee
	 */
	public String getPayee();
	
	/**
	 * Returns the memo of the transaction.
	 * 
	 * @return transaction memo
	 */
	public String getMemo();
	
	/**
	 * Returns the account from which funds
	 * were transferred.
	 * 
	 * @return withdrawal account
	 */
	public TransferAccount getWithdrawalAccount();
	
	/**
	 * Returns the account into which funds
	 * were transferred.
	 * 
	 * @return deposit account
	 */
	public TransferAccount getDepositAccount();
	
	/**
	 * Returns the amount that was transferred.
	 * 
	 * @return transfer amount
	 */
	public Currency getTransferAmount();

}
