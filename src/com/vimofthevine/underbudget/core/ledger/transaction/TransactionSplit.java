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

/**
 * Representation of a transaction split
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionSplit {
	
	/**
	 * Amount of money transferring into/from this split
	 */
	public Currency value;
	
	/**
	 * Memo field of this split
	 */
	public String memo = "";
	
	/**
	 * Account into/from which money is being transferred
	 */
	public Account account;

}
