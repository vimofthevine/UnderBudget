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

package com.vimofthevine.underbudget.swing.currency;

import java.text.ParseException;
import java.util.Currency;

import javax.swing.JFormattedTextField.AbstractFormatter;

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.Commodity;

/**
 * Currency formatter used when displaying a non-editable
 * currency amount.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class DisplayCurrencyFormatter extends AbstractFormatter {

	/**
	 * Currency
	 */
	private final Currency currency;
	
	/**
	 * Constructs a new non-editable currency formatter.
	 * 
	 * @param currency being used
	 */
	DisplayCurrencyFormatter(Currency currency)
	{
		this.currency = currency;
	}
	
	@Override
    public Object stringToValue(String text) throws ParseException
    {
		return Commodity.create(currency, text);
    }

	@Override
    public String valueToString(Object value) throws ParseException
    {
		if (value == null)
		{
			return "";
		}
		else if (value instanceof CashCommodity)
		{
			CashCommodity currency = (CashCommodity) value;
			return currency.formatAsString();
		}
		else
			throw new ParseException("Invalid currency object type: " +
				value.getClass().getName(), 0);
    }

}
