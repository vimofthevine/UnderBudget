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

import javax.swing.JFormattedTextField;
import javax.swing.JFormattedTextField.AbstractFormatter;

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.Commodity;

/**
 * Currency formatter used when editing a currency amount.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class EditingCurrencyFormatter extends AbstractFormatter {
	
	/**
	 * Currency code
	 */
	private final Currency currency;
	
	/**
	 * Mathematical calculator
	 */
	private final Calculator calculator;
	
	/**
	 * Currency field instance
	 */
	private CurrencyField currencyField;
	
	/**
	 * Constructs a new editable currency formatter.
	 * 
	 * @param factory currency factory
	 * @param currency currency being used
	 */
	EditingCurrencyFormatter(Currency currency)
	{
		this.currency = currency;
		calculator = new Calculator();
	}
	
	@Override
	public void install(JFormattedTextField field)
	{
		super.install(field);
		
		if (field instanceof CurrencyField)
		{
			currencyField = (CurrencyField) field;
		}
	}
	
	@Override
	public void uninstall()
	{
		super.uninstall();
		
		if (currencyField != null)
		{
			CashCommodity value = (CashCommodity) currencyField.getValue();
			currencyField.getModel().setNewValue(value);
		}
	}

	@Override
    public Object stringToValue(String text) throws ParseException
    {
		try
		{
			Float amount = calculator.calculate(text);
			if (amount == null)
				throw new ParseException("Invalid currency input", 0);
			
			return Commodity.create(currency, String.valueOf(amount));
		}
		catch (NumberFormatException nfe)
		{
			throw new ParseException("Invalid currency input", 0);
		}
    }

	@Override
    public String valueToString(Object value) throws ParseException
    {
		if (value instanceof CashCommodity)
		{
			CashCommodity currency = (CashCommodity) value;
			return currency.getValue().asString();
		}
		else
			throw new ParseException("Invalid currency object type: " +
				value.getClass().getName(), 0);
    }

}
