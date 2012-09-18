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

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.CurrencyCalculator;

/**
 * Default <code>EndingBalances</code> implementation.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class DefaultEndingBalances implements EndingBalances {
	
	/**
	 * Estimated ending balance
	 */
	DefaultEndingBalance estimated;
	
	/**
	 * Actual ending balance
	 */
	DefaultEndingBalance actual;
	
	/**
	 * Expected ending balance
	 */
	DefaultEndingBalance expected;
	
	/**
	 * Constructs a new ending balances collection.
	 * 
	 * @param initial    initial balance
	 * @param calculator currency calculator
	 */
	DefaultEndingBalances(CashCommodity initial,
		CurrencyCalculator calculator)
	{
		estimated = new DefaultEndingBalance("estimated", initial, calculator);
		actual = new DefaultEndingBalance("actual", initial, calculator);
		expected = new DefaultEndingBalance("expected", initial, calculator);
	}
	
	@Override
	public EndingBalance getEstimatedEndingBalance()
	{
		return estimated;
	}

	@Override
	public EndingBalance getActualEndingBalance()
	{
		return actual;
	}

	@Override
	public EndingBalance getExpectedEndingBalance()
	{
		return expected;
	}

}
