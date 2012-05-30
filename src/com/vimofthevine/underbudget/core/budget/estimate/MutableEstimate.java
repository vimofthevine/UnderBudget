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

package com.vimofthevine.underbudget.core.budget.estimate;

import com.vimofthevine.underbudget.core.budget.estimate.behavior.EstimateBehavior;
import com.vimofthevine.underbudget.core.budget.estimate.impact.BalanceImpact;
import com.vimofthevine.underbudget.core.currency.Currency;

/**
 * An estimate that can be modified by the user
 * at runtime.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface MutableEstimate {

	/**
	 * Adds a new child estimate to this estimate.
	 * 
	 * @param child new child estimate
	 */
	public void addChild(MutableEstimate child);
	
	/**
	 * Removes the specified child from this estimate.
	 * 
	 * @param child child estimate to be removed
	 */
	public void removeChild(MutableEstimate child);
	
	/**
	 * Renames this estimate.
	 * 
	 * @param name new name for this estimate
	 */
	public void rename(String name);
	
	/**
	 * Describes this estimate.
	 * 
	 * @param description new description for this estimate
	 */
	public void describe(String description);
	
	/**
	 * Specifies the amount of this estimate.
	 * 
	 * @param amount new amount for this estimate
	 */
	public void setEstimatedAmount(Currency amount);
	
	/**
	 * Specifies that if more transactions are expected
	 * against this estimate (if all activity is final).
	 * 
	 * @param more <code>true</code> if more transactions are expected,
	 *              else <code>false</code>
	 */
	public void hasMoreTransactionsExpected(boolean more);
	
	/**
	 * Specifies the impact this estimate has on ending balances.
	 * 
	 * @param impact balance impact of this estimate
	 */
	public void specifyImpact(BalanceImpact impact);
	
	/**
	 * Specifies the behavior type of this estimate.
	 * 
	 * @param behavior behavior for this estimate
	 */
	public void specifyBehavior(EstimateBehavior behavior);
	
}
