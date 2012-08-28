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

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;

import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;

/**
 * Model for <code>CurrencyField</code> instances.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public abstract class CurrencyInputModel {
	
	/**
	 * Currency formatter factory
	 */
	private final CurrencyFormatterFactory formatter;
	
	/**
	 * Currency input verifier
	 */
	private final CurrencyInputVerifier verifier;
	
	/**
	 * Registered change listeners
	 */
	private final Collection<CurrencyChangeListener> listeners;
	
	/**
	 * Currency currency value
	 */
	private Currency value;
	
	/**
	 * Constructs a new currency input model.
	 * 
	 * @param factory currency factory
	 */
	public CurrencyInputModel(CurrencyFactory factory)
	{
		formatter = new CurrencyFormatterFactory(factory);
		verifier = new CurrencyInputVerifier(factory);
		listeners = Collections.synchronizedList(
			new ArrayList<CurrencyChangeListener>());
	}
	
	/**
	 * Updates with the new currency value.
	 * 
	 * @param newValue new currency value
	 */
	abstract public void setNewValue(Currency newValue);
	
	/**
	 * Retrieves the current currency value of this model.
	 * 
	 * @return current currency value
	 */
	public Currency getValue()
	{
		return value;
	}
	
	/**
	 * Sets the currency value of this input model.
	 * 
	 * @param newValue new currency value
	 */
	public void setValue(Currency newValue)
	{
		value = newValue;
		fireCurrencyChange(value);
	}
	
	/**
	 * Returns a formatter factory for currency values.
	 * 
	 * @return currency formatter factory
	 */
	CurrencyFormatterFactory getFormatterFactory()
	{
		return formatter;
	}
	
	/**
	 * Returns an input verifier for the currency value.
	 * 
	 * @return currency input verifier
	 */
	CurrencyInputVerifier getInputVerifier()
	{
		return verifier;
	}
	
	/**
	 * Notify all registered listeners of the new currency value.
	 * 
	 * @param newValue new currency value
	 */
	protected synchronized void fireCurrencyChange(Currency newValue)
	{
		for (CurrencyChangeListener listener : listeners)
		{
			listener.currencyChanged(newValue);
		}
	}

	/**
	 * Registers a listener to receive notification
	 * when the currency has changed.
	 * 
	 * @param listener listener to be registered
	 */
	public synchronized void addChangeListener(CurrencyChangeListener listener)
	{
		listeners.add(listener);
	}
	
	/**
	 * Unregisters a listener from receiving notification
	 * when the currency has changed.
	 * 
	 * @param listener listener to be unregistered
	 */
	public synchronized void removeChangeListener(CurrencyChangeListener listener)
	{
		listeners.remove(listener);
	}
	
}
