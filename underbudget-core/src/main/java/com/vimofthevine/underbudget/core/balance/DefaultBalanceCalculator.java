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
import com.vimofthevine.underbudget.core.estimate.BalanceImpact;
import com.vimofthevine.underbudget.core.estimate.Estimate;

/**
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class DefaultBalanceCalculator implements BalanceCalculator {
	
	@Override
	public EndingBalances calculate(CashCommodity initial,
		Estimate estimate, ActualFigures actuals, CurrencyCalculator calculator)
	{
		DefaultEndingBalances balances =
			new DefaultEndingBalances(initial, calculator);
		calculate(balances, estimate, actuals, calculator);
		return balances;
	}
	
	/**
	 * Recursively add the impact of each leaf estimate
	 * to the estimated ending balance, actual of each
	 * leaf estimate to the actual balance, and the
	 * calculate expected ending balance.
	 * 
	 * @param balances   ending balances
	 * @param estimate   estimate to be recursively added
	 * @param actuals    actual figures
	 * @param calculator currency calculator
	 */
	private void calculate(DefaultEndingBalances balances,
		Estimate estimate, ActualFigures actuals, CurrencyCalculator calculator)
	{
		// Iterate over children (don't count parent estimates)
		if (estimate.getChildCount() > 0)
		{
			for (int i=0; i<estimate.getChildCount(); i++)
			{
				calculate(balances, estimate.getChildAt(i), actuals, calculator);
			}
		}
		else
		{
			// Get impact of the estimate
			BalanceImpact impact = estimate.getImpact(
				actuals.getActual(estimate), calculator);
			
			// Add to ending balances
			balances.estimated.apply(impact.getEstimatedImpact());
			balances.actual.apply(impact.getActualImpact());
			balances.expected.apply(impact.getExpectedImpact());
		}
	}

}
