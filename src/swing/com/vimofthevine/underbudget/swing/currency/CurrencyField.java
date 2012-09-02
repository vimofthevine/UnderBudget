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

import com.vimofthevine.underbudget.core.currency.CashCommodity;

/**
 * Custom formatted text field for defining
 * currency values.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CurrencyField extends JFormattedTextField
implements CurrencyChangeListener {
	
	/**
	 * Currency input model
	 */
	private final CurrencyInputModel inputModel;

	/**
	 * Constructs a new currency input field.
	 * 
	 * @param model currency input model
	 */
	public CurrencyField(CurrencyInputModel model)
	{
		super(model.getFormatterFactory());
		setInputVerifier(model.getInputVerifier());
		inputModel = model;
		inputModel.addChangeListener(this);
	}
	
	/**
	 * Returns the currency input model.
	 * 
	 * @return currency input model
	 */
	CurrencyInputModel getModel()
	{
		return inputModel;
	}

	@Override
    public void currencyChanged(CashCommodity currency)
    {
		setValue(currency);
    }
	
}
