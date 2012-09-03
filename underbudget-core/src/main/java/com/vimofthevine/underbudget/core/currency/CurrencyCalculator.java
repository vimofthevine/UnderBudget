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
 * A calculator for arithmetic operations between
 * cash commodity values. The calculator will
 * convert commodities between currencies as
 * needed. The target currency refers to the
 * base currency on which all operations are
 * performed and used for returned values.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface CurrencyCalculator {
	
	/**
	 * Returns a representation of zero
	 * for the target currency.
	 * 
	 * @return commodity value of zero
	 */
	public CashCommodity zero();
	
	/**
	 * Converts the given commodity value to the
	 * target currency.
	 * 
	 * @param value commodity value to be converted
	 * @return commodity value in the target currency
	 */
	public CashCommodity convert(CashCommodity value);
	
	/**
	 * Converts the given currency amount to the
	 * target currency.
	 * 
	 * @param currency original currency
	 * @param amount   original amount
	 * @return commodity value in the target currency
	 */
	public CashCommodity convert(Currency currency, Number amount);
	
	/**
	 * Converts the given currency amount to the
	 * target currency.
	 * 
	 * @param iso4217 original ISO4217 currency code
	 * @param amount  original amount
	 * @return commodity value in the target currency
	 */
	public CashCommodity convert(String iso4217, float amount);
	
	/**
	 * Adds the augend to the addend and returns the
	 * sum, converted to the target currency, if it
	 * is not already.
	 * 
	 * @param augend first value to be added
	 * @param addend second value to be added
	 * @return sum of the augend and addend, in the
	 *         target currency
	 */
	public CashCommodity add(CashCommodity augend,
		CashCommodity addend);

	/**
	 * Subtracts the subtrahend from the minuend and
	 * returns the difference, converted to the target
	 * currency, if it is not already.
	 * 
	 * @param minuend    value to be subtracted from
	 * @param subtrahend value to subtract
	 * @return difference between the minuend and subtrahend,
	 *         in the target currency
	 */
	public CashCommodity subtract(CashCommodity minuend,
		CashCommodity subtrahend);
	
}
