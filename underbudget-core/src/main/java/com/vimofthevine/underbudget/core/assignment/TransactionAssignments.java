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
 * Map of all transactions that have been assigned
 * and their assigning assignment rules.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface TransactionAssignments {
	
	/**
	 * Retrieves the assignment rule that assigned the given transaction.
	 * 
	 * @param transaction transaction for which to retrieve the
	 *        assigning assignment rule
	 * @return assignment rule that assigned the transaction
	 */
	public AssignmentRule getAssigningRule(Transaction transaction);
	
	/**
	 * Retrieves all transactions that have been assigned by
	 * the given assignment rule.
	 * 
	 * @param rule assignment rule for which to retrieve all
	 *        assigned transactions
	 * @return transactions that were assigned by the assignment rule
	 */
	public Transaction[] getAssignedTransactions(AssignmentRule rule);
	
}
