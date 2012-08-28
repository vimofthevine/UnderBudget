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

import javax.swing.JFormattedTextField;
import javax.swing.JFormattedTextField.AbstractFormatter;
import javax.swing.JFormattedTextField.AbstractFormatterFactory;

import com.vimofthevine.underbudget.core.currency.CurrencyFactory;

/**
 * Formatter factory for creating currency
 * formatters to be used in formatted text
 * fields.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class CurrencyFormatterFactory extends AbstractFormatterFactory {
	
	/**
	 * Formatter used when the field is in focus
	 * (i.e., being edited)
	 */
	private AbstractFormatter editingFormatter;
	
	/**
	 * Formatter used when the field is not in
	 * focus (i.e., not being edited)
	 */
	private AbstractFormatter displayingFormatter;
	
	/**
	 * Constructs a new formatter factory for
	 * a given currency type.
	 * 
	 * @param factory currency factory
	 */
	CurrencyFormatterFactory(CurrencyFactory factory)
	{
		displayingFormatter = new DisplayCurrencyFormatter(factory);
		editingFormatter = new EditingCurrencyFormatter(factory);
	}

	@Override
    public AbstractFormatter getFormatter(JFormattedTextField field)
    {
		return field.isFocusOwner()
			? editingFormatter : displayingFormatter;
    }

}
