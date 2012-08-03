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

import javax.swing.JFormattedTextField;
import javax.swing.JFormattedTextField.AbstractFormatter;

import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;

/**
 * Currency formatter used when displaying a non-editable
 * currency amount.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class DisplayCurrencyFormatter extends AbstractFormatter {

	/**
	 * Currency factory
	 */
	private final CurrencyFactory factory;
	
	/**
	 * Constructs a new non-editable currency formatter.
	 * 
	 * @param factory currency factory
	 */
	DisplayCurrencyFormatter(CurrencyFactory factory)
	{
		this.factory = factory;
	}
	
	@Override
	public void install(JFormattedTextField field)
	{
		super.install(field);
		
		if (field.getDocument() instanceof CommittableDocument)
		{
			((CommittableDocument) field.getDocument()).commit();
		}
	}
	
	@Override
    public Object stringToValue(String text) throws ParseException
    {
		return factory.newCurrencyInstance(text);
    }

	@Override
    public String valueToString(Object value) throws ParseException
    {
		if (value == null)
		{
			return "";
		}
		else if (value instanceof Currency)
		{
			Currency currency = (Currency) value;
			return currency.formatAsString();
		}
		else
			throw new ParseException("Invalid currency object type: " +
				value.getClass().getName(), 0);
    }

}
