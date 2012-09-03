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

package com.vimofthevine.underbudget.swing.summary;

import java.util.Currency;

import com.vimofthevine.underbudget.core.balance.EndingBalance;
import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.Commodity;

/**
 * An ending balance implementation representing
 * an uninitialized ending balance value.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class UninitEndingBalance implements EndingBalance {
	
	/**
	 * Uninitialized balance value
	 */
	private final CashCommodity value;
	
	/**
	 * Constructs a new uninitialized ending balance.
	 * 
	 * @param currency currency in use
	 */
	UninitEndingBalance(Currency currency)
	{
		value = Commodity.zero(currency);
	}

	@Override
	public CashCommodity getInitialValue()
	{
		return value;
	}

	@Override
	public CashCommodity getValue()
	{
		return value;
	}

	@Override
	public CashCommodity getSumIncreases()
	{
		return value;
	}

	@Override
	public CashCommodity getSumDecreases()
	{
		return value;
	}

	@Override
	public CashCommodity getNetChange()
	{
		return value;
	}

}
