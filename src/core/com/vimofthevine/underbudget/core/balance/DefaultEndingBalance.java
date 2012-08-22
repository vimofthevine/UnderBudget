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
 * 
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class DefaultEndingBalance implements EndingBalance {
	
	/**
	 * Initial balance
	 */
	private final Currency initialValue;
	
	/**
	 * Ending balance value
	 */
	private final Currency endingValue;
	
	/**
	 * Sum of all decreases
	 */
	private final Currency decreases;
	
	/**
	 * Sum of all increases
	 */
	private final Currency increases;
	
	DefaultEndingBalance(Currency initial)
	{
		initialValue = initial.clone();
		endingValue = initial.clone();
		
		// Start with 0
		decreases = initial.clone().decreaseBy(initial);
		increases = decreases.clone();
	}
	
	void apply(Currency change)
	{
		if (change.isNegative())
		{
			decreases.increaseBy(change.clone().invert());
		}
		else
		{
			increases.increaseBy(change);
		}
		
		endingValue.increaseBy(change);
	}

	@Override
	public Currency getInitialValue()
	{
		return initialValue;
	}

	@Override
	public Currency getValue()
	{
		return endingValue;
	}

	@Override
	public Currency getSumIncreases()
	{
		return increases;
	}

	@Override
	public Currency getSumDecreases()
	{
		return decreases;
	}

	@Override
	public Currency getNetChange()
	{
		return increases.clone().decreaseBy(decreases);
	}

}
