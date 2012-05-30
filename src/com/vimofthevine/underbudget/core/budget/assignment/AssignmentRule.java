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

package com.vimofthevine.underbudget.core.budget.assignment;

import com.vimofthevine.underbudget.core.ledger.transaction.AnalyzableTransaction;

/**
 * Criterion used to assign transactions to estimates.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface AssignmentRule {

	/**
	 * Checks if a given transaction meets the
	 * criteria for this assignment rule, and
	 * assigns the transaction to the associated estimate.
	 * 
	 * @param transaction transaction to be compared
	 * @return <code>true</code> if the transaction matches
	 *          this rule's criterion and has been assigned
	 *          to the estimate, else <code>false</code>
	 */
	public boolean matches(AnalyzableTransaction transaction);
	
	/**
	 * Returns a concise string representation of the
	 * assignment rule, suitable for display to the
	 * user.
	 * 
	 * @return string representation of this rule
	 */
	public String formatAsString();
	
}
