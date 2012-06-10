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

package com.vimofthevine.underbudget.gui.currency;

import java.text.ParseException;

import javax.swing.JFormattedTextField;
import javax.swing.JFormattedTextField.AbstractFormatter;

import com.vimofthevine.underbudget.currency.Currency;
import com.vimofthevine.underbudget.currency.CurrencyFactory;

/**
 * Currency formatter used when editing a currency amount.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class EditingCurrencyFormatter extends AbstractFormatter {
	
	/**
	 * Currency factory
	 */
	private final CurrencyFactory factory;
	
	/**
	 * Document model
	 */
	private CommittableDocument document;
	
	/**
	 * Constructs a new editable currency formatter.
	 * 
	 * @param factory currency factory
	 */
	EditingCurrencyFormatter(CurrencyFactory factory)
	{
		this.factory = factory;
	}
	
	@Override
	public void install(JFormattedTextField field)
	{
		super.install(field);
		
		if (field.getDocument() instanceof CommittableDocument)
		{
			document = (CommittableDocument) field.getDocument();
		}
	}
	
	@Override
	public void uninstall()
	{
		super.uninstall();
		
		if (document != null)
		{
			document.commit();
		}
	}

	@Override
    public Object stringToValue(String text) throws ParseException
    {
		try
		{
			return factory.newCurrencyInstance(text);
		}
		catch (NumberFormatException nfe)
		{
			throw new ParseException("Invalid currency input", 0);
		}
    }

	@Override
    public String valueToString(Object value) throws ParseException
    {
		if (value instanceof Currency)
		{
			Currency currency = (Currency) value;
			return currency.toStr();
		}
		else
			throw new ParseException("Invalid currency object type: " +
				value.getClass().getName(), 0);
    }

}
