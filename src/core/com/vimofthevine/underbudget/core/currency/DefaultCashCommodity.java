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

package com.vimofthevine.underbudget.core.currency;

import java.text.NumberFormat;
import java.util.Currency;

/**
 * Default <code>CashCommodity</code> implementation.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class DefaultCashCommodity implements CashCommodity {
	
	/**
	 * Commodity currency
	 */
	private final Currency currency;
	
	/**
	 * Commodity value
	 */
	private final Number value;
	
	/**
	 * Constructs a new cash commodity instance with
	 * the given currency and value.
	 * 
	 * @param currency cash commodity currency
	 * @param value    cash commodity value
	 */
	DefaultCashCommodity(Currency currency, Number value)
	{
		this.currency = currency;
		this.value = value;
	}

	@Override
	public Currency getCurrency()
	{
		return currency;
	}

	@Override
	public Number getValue()
	{
		return value;
	}
	
	@Override
	public CashCommodity negate()
	{
		return new DefaultCashCommodity(currency, value.negate());
	}

	@Override
	public String formatAsString()
	{
		NumberFormat format = NumberFormat.getCurrencyInstance();
		format.setCurrency(currency);
		return format.format(value.floatValue());
	}
	
	@Override
	public int hashCode()
	{
		int result = 783;
		result = result * 31 + currency.hashCode();
		result = result * 31 + value.hashCode();
		return result;
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj == this)
			return true;
		if ( ! (obj instanceof DefaultCashCommodity))
			return false;
		
		DefaultCashCommodity that =
			(DefaultCashCommodity) obj;
		
		return this.currency.equals(that.currency)
			&& this.value.equals(that.value);
	}
	
	@Override
	public String toString()
	{
		return formatAsString();
	}

}
