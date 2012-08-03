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

import com.vimofthevine.underbudget.core.assignment.Condition;

/**
 * A <code>Transaction</code> represents a literal financial
 * transaction; that is, a transfer of funds from one account
 * into another.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface Transaction {

	/**
	 * Returns the definition of this transaction.
	 * 
	 * @return transaction definition
	 */
	public TransactionDefinition getDefinition();
	
	/**
	 * Checks if this transaction meets the criteria
	 * of the given condition.
	 * 
	 * @param condition criteria to be checked
	 * @return <code>true</code> if this transaction
	 *         meets the criteria of the condition,
	 *         else <code>false</code>
	 */
	public boolean meets(Condition condition);
	
}
