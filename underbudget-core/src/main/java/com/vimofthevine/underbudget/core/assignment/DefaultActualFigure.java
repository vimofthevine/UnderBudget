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

import java.util.List;

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.CurrencyCalculator;
import com.vimofthevine.underbudget.core.transaction.Transaction;

/**
 * Default implementation of an actual figure
 * that is the sum value of a set of transactions.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class DefaultActualFigure implements ActualFigure {
	
	/**
	 * Actual figure
	 */
	private final CashCommodity actual;

	/**
	 * Constructs a default actual figure, which is
	 * the sum of the value of the given list of
	 * transactions.
	 * 
	 * @param calculator   currency calculator
	 * @param transactions transaction list
	 */
	DefaultActualFigure(CurrencyCalculator calculator,
		List<Transaction> transactions)
	{
		CashCommodity sum = calculator.zero();
		
		for (Transaction transaction : transactions)
		{
			sum = calculator.add(sum, transaction.getDefinition().getTransferAmount());
		}
		
		actual = sum;
	}

	@Override
    public CashCommodity getAmount()
    {
		return actual;
    }
	
}