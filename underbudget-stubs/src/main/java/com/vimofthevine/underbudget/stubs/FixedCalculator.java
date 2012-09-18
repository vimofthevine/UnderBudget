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

package com.vimofthevine.underbudget.stubs;

import java.util.Currency;

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.Commodity;
import com.vimofthevine.underbudget.core.currency.CurrencyCalculator;
import com.vimofthevine.underbudget.core.currency.Number;

/**
 * Implementation of a <code>CurrencyCalculator</code>
 * that uses a fixed currency instance, performing
 * dumb conversions (i.e., no numerical conversion)
 * between commodities.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class FixedCalculator implements CurrencyCalculator {
	
	/**
	 * Fixed currency instance
	 */
	private final Currency currency;
	
	/**
	 * Constructs a new currency calculator,
	 * fixed with the given currency.
	 * 
	 * @param currency base/target currency
	 */
	public FixedCalculator(Currency currency)
	{
		this.currency = currency;
	}

	@Override
	public CashCommodity zero()
	{
		return Commodity.zero(currency);
	}

	@Override
	public CashCommodity convert(CashCommodity value)
	{
		return Commodity.create(currency, value.getValue());
	}

	@Override
	public CashCommodity convert(Currency currency, Number amount)
	{
		return Commodity.create(this.currency, amount);
	}

	@Override
	public CashCommodity convert(String iso4217, float amount)
	{
		return Commodity.create(currency, String.valueOf(amount));
	}

	@Override
	public CashCommodity add(CashCommodity augend, CashCommodity addend)
	{
		Number sum = augend.getValue();
		sum = sum.increaseBy(addend.getValue());
		return Commodity.create(currency, sum);
	}

	@Override
	public CashCommodity subtract(CashCommodity minuend,
	        CashCommodity subtrahend)
	{
		Number difference = minuend.getValue();
		difference = difference.decreaseBy(subtrahend.getValue());
		return Commodity.create(currency, difference);
	}

}
