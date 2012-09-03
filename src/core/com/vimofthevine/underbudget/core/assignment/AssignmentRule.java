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

import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.transaction.Transaction;

/**
 * An assignment rule is a set of conditions to be
 * matched against transactions and to assign the
 * transaction to a specific estimate.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface AssignmentRule {

	/**
	 * Returns the definition of this assignment rule.
	 * 
	 * @return assignment rule definition
	 */
	public RuleDefinition getDefinition();
	
	/**
	 * Checks if the given transaction matches
	 * the criteria of this assignment rule.
	 * 
	 * @param transaction transaction to be evaluated
	 * @return <code>true</code> if the transaction meets
	 *         the criteria of this rule, else <code>false</code>
	 */
	public boolean matches(Transaction transaction);
	
	/**
	 * Returns the estimate to which qualifying transactions
	 * are assigned.
	 * 
	 * @return associated estimate
	 */
	public Estimate getEstimate();
	
}
