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

package com.vimofthevine.underbudget.core.assignment;

import com.vimofthevine.underbudget.core.transaction.Transaction;

/**
 * A <code>TransactionAssigner</code> iterates over the given
 * list of transactions, attempting to assign to an estimate
 * by iterating over the prioritized list of assignment rules.
 * If a matching rule is found for a transaction, it is assigned
 * by recording the assignment. If no match was found, the
 * transaction is assigned to a special, "no match found"
 * assignment rule.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface TransactionAssigner {

	/**
	 * Performs assignment of the given transactions using
	 * the given assignment rules.
	 * 
	 * @param transactions transactions to be assigned
	 * @param rules        assignment rules to be used for assigning
	 * @return record of transactions assignments
	 */
	public TransactionAssignments assign(Transaction[] transactions,
		AssignmentRules[] rules);
	
}
