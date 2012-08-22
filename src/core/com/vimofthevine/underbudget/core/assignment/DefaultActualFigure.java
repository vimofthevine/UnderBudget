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

import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
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
	private final Currency actual;

	/**
	 * Constructs a default actual figure, which is
	 * the sum of the value of the given list of
	 * transactions.
	 * 
	 * @param factory      currency factory
	 * @param transactions transaction list
	 */
	DefaultActualFigure(CurrencyFactory factory,
		List<Transaction> transactions)
	{
		actual = factory.newCurrencyInstance();
		
		for (Transaction transaction : transactions)
		{
			actual.increaseBy(transaction.getDefinition().getTransferAmount());
		}
	}

	@Override
    public Currency getAmount()
    {
		return actual;
    }
	
}
