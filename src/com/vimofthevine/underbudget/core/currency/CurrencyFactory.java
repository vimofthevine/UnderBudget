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

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Factory class for creating new currency objects.
 * Should multiple currency types be needed, this
 * class can be modified to allow the dynamic
 * specification of the currency class to be used.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CurrencyFactory {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(CurrencyFactory.class.getName());
	
	/**
	 * Currency class to be created
	 */
	private Class<?> currencyClass;
	
	/**
	 * Constructs a currency factory to create currency
	 * objects for the given ISO 4217 currency code.
	 * 
	 * @param code ISO 4217 currency code
	 * @see http://en.wikipedia.org/wiki/ISO_4217 
	 */
	public CurrencyFactory(String code)
	{
		try
		{
			currencyClass = Class.forName(getClass().getPackage().getName() +
				"." + code + "Currency");
		}
		catch (ClassNotFoundException cnfe)
		{
			cnfe.printStackTrace();
			throw new IllegalArgumentException("Invalid currency type, " + code);
		}
	}
	
	/**
	 * Constructs a currency factory to create currency
	 * objects of the given type
	 * 
	 * @param clazz currency class to use when creating
	 *               new currency objects
	 */
    public CurrencyFactory(Class<?> clazz)
	{
		if (Currency.class.isAssignableFrom(clazz))
		{
			currencyClass = clazz;
		}
		else
			throw new IllegalArgumentException("Invalid currency class");
	}
    
    /**
     * Creates a new currency instance
     * 
     * @return new currency instance
     */
    public Currency newCurrencyInstance()
    {
    	try
    	{
    		return (Currency) currencyClass.newInstance();
    	}
    	catch (Exception e)
    	{
    		logger.log(Level.WARNING, "Error creating currency instance", e);
    		throw new ClassCastException("Unable to create currency instance");
    	}
    }
    
	/**
	 * Creates a new currency instance with the given initial amount
	 * 
	 * @param amount string representation of the currency amount
	 * @return new currency instance
	 */
	public Currency newCurrencyInstance(String amount)
	{
		Currency currency = newCurrencyInstance();
		currency.parse(amount);
		
		return currency;
	}

}
