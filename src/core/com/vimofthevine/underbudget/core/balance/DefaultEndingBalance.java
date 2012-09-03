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

import java.util.logging.Level;
import java.util.logging.Logger;

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.Commodity;
import com.vimofthevine.underbudget.core.currency.CurrencyCalculator;
import com.vimofthevine.underbudget.core.currency.Number;

/**
 * Default implementation of <code>EndingBalance</code>
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class DefaultEndingBalance implements EndingBalance {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(DefaultEndingBalance.class.getName());
	
	/**
	 * Name of this ending balance
	 */
	private final String name;
	
	/**
	 * Currency calculator instance
	 */
	private final CurrencyCalculator calculator;
	
	/**
	 * Initial balance
	 */
	private final CashCommodity initialValue;
	
	/**
	 * Ending balance value
	 */
	private CashCommodity endingValue;
	
	/**
	 * Sum of all decreases
	 */
	private CashCommodity decreases;
	
	/**
	 * Sum of all increases
	 */
	private CashCommodity increases;
	
	/**
	 * Constructs a new ending balance instance.
	 * 
	 * @param name    name of this ending balance
	 * @param initial initial balance
	 * @param calc    currency calculator
	 */
	DefaultEndingBalance(String name, CashCommodity initial,
		CurrencyCalculator calc)
	{
		this.name = name;
		calculator = calc;
		
		initialValue = initial;
		endingValue = initial;
		
		// Start with 0
		decreases = Commodity.zero(initial.getCurrency());
		increases = Commodity.zero(initial.getCurrency());
	}
	
	void apply(CashCommodity change)
	{
		logger.log(Level.INFO, "Applying to " + name + " balance: " + change);
		
		Number value = change.getValue();
		
		if (value.isNegative())
		{
			decreases = calculator.add(decreases, change.negate());
		}
		else
		{
			increases = calculator.add(increases, change);
		}
		
		endingValue = calculator.add(endingValue, change);
	}

	@Override
	public CashCommodity getInitialValue()
	{
		return initialValue;
	}

	@Override
	public CashCommodity getValue()
	{
		return endingValue;
	}

	@Override
	public CashCommodity getSumIncreases()
	{
		return increases;
	}

	@Override
	public CashCommodity getSumDecreases()
	{
		return decreases;
	}

	@Override
	public CashCommodity getNetChange()
	{
		return calculator.subtract(increases, decreases);
	}

}
