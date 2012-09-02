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

import com.vimofthevine.underbudget.core.assignment.ActualFigures;
import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.CurrencyCalculator;
import com.vimofthevine.underbudget.core.estimate.Estimate;

/**
 * A <code>BalanceCalculator</code> iterates over the
 * estimate tree, deriving the estimated, actual, and
 * expected ending balances.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface BalanceCalculator {
	
	/**
	 * Performs the calculation of ending balances given
	 * the estimate tree and actual figures.
	 * 
	 * @param initial    initial balance
	 * @param estimate   root of the estimate tree
	 * @param actuals    actual incomes or expenses
	 * @param calculator currency calculator instance
	 * @return calculated ending balances
	 */
	public EndingBalances calculate(CashCommodity initial,
		Estimate estimate, ActualFigures actuals,
		CurrencyCalculator calculator);
	
}
