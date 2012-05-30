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

package com.vimofthevine.underbudget.core.report;

import com.vimofthevine.underbudget.core.budget.assignment.AssignmentRule;
import com.vimofthevine.underbudget.core.ledger.transaction.DisplayableTransaction;

/**
 * A transaction assignment report records the assignment
 * rule each transaction was matched against.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface TransactionAssignmentReport {
	
	/**
	 * Records the assignment rule to which a transaction
	 * met the required criteria.
	 * 
	 * @param transaction transaction that has been assigned
	 * @param rule        assignment rule the transaction matched
	 */
	public void recordAssignment(DisplayableTransaction transaction, AssignmentRule rule);
	
}
