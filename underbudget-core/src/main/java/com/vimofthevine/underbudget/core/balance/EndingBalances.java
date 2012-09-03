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

package com.vimofthevine.underbudget.core.balance;

/**
 * The set of estimated (based on user-defined values),
 * actual (based on imported transactions), and expected
 * (the extrapolated) ending balances.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface EndingBalances {
	
	/**
	 * Returns the estimated ending balance. This balance
	 * represents the ending balance as calculated with
	 * the user-supplied income and expense estimated values.
	 * 
	 * @return estimated ending balance
	 */
	public EndingBalance getEstimatedEndingBalance();
	
	/**
	 * Returns the actual ending balance. This balance
	 * represents the sum of income and expense transactions
	 * that have been imported. During a budgeting period,
	 * the actual ending balance represents the current,
	 * actual balance to-date.
	 * 
	 * @return actual ending balance
	 */
	public EndingBalance getActualEndingBalance();
	
	/**
	 * Returns the expected ending balance. This balance
	 * represents the extrapolated ending balance, taking
	 * into consideration current estimate progress (e.g.,
	 * over-budget estimates mean a decrease of the expected
	 * ending balance while final under-budget estimates mean
	 * an increase of the expected ending balance).
	 * 
	 * @return expected ending balance
	 */
	public EndingBalance getExpectedEndingBalance();

}
