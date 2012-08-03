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

import com.vimofthevine.underbudget.core.currency.Currency;

/**
 * An <code>EndingBalance</code> represents the estimated,
 * actual (or to-date), or expected ending balance at the
 * end of the budgeting period. Typical ending balances are
 * reflective of an initial balance of $0. Thus, to get
 * a more useful ending balance, the user-defined initial
 * balance must be added to the ending balances.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface EndingBalance {
	
	/**
	 * Returns the initial balance value.
	 * 
	 * @return initial balance value
	 */
	public Currency getInitialValue();
	
	/**
	 * Returns the ending balance value.
	 * 
	 * @return ending balance value
	 */
	public Currency getValue();
	
	/**
	 * Returns the total amount by which the
	 * ending balance increased from the initial
	 * balance.
	 * 
	 * @return total amount of increase
	 */
	public Currency getSumIncreases();
	
	/**
	 * Returns the total amount by which the
	 * ending balance decreased from the initial
	 * balance. This value is an absolute value
	 * (i.e., $1000 of decrease is returned as
	 * $1000, not -$1000).
	 * 
	 * @return total amount of decrease
	 */
	public Currency getSumDecreases();
	
	/**
	 * Returns the total amount by which the
	 * ending balance differs from the initial
	 * balance. If the ending balance is lower
	 * than the initial balance, a negative
	 * value is returned.
	 * 
	 * @return total amount of change
	 */
	public Currency getNetChange();

}
