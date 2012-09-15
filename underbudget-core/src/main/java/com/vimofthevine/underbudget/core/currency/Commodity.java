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

import java.util.Currency;

/**
 * Static utility functions for creating commodity values.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class Commodity {
	
	/**
	 * Returns a representation of zero
	 * for the given currency.
	 * 
	 * @param iso4217 ISO4217 currency code
	 * @return commodity value of zero
	 */
	public static final CashCommodity zero(String iso4217)
	{
		return new DefaultCashCommodity(
			Currency.getInstance(iso4217), DecimalNumber.ZERO);
	}
	
	/**
	 * Returns a representation of zero
	 * for the given currency.
	 * 
	 * @param currency commodity currency
	 * @return commodity value of zero
	 */
	public static final CashCommodity zero(Currency currency)
	{
		return new DefaultCashCommodity(currency, DecimalNumber.ZERO);
	}
	
	/**
	 * Returns the cash commodity value for the
	 * given currency amount.
	 * 
	 * @param iso4217 ISO4217 currency code
	 * @param amount  commodity amount
	 * @return commodity value
	 */
	public static final CashCommodity create(String iso4217, String amount)
	{
		return new DefaultCashCommodity(
			Currency.getInstance(iso4217),
			new DecimalNumber(amount));
	}
	
	/**
	 * Returns the cash commodity value for the
	 * given currency amount.
	 * 
	 * @param currency commodity currency
	 * @param amount  commodity amount
	 * @return commodity value
	 */
	public static final CashCommodity create(Currency currency, Number amount)
	{
		return new DefaultCashCommodity(currency, amount);
	}
	
	/**
	 * Returns the cash commodity value for the
	 * given currency amount.
	 * 
	 * @param currency commodity currency
	 * @param amount  commodity amount
	 * @return commodity value
	 */
	public static final CashCommodity create(Currency currency, String amount)
	{
		return new DefaultCashCommodity(currency,
			new DecimalNumber(amount));
	}

}
