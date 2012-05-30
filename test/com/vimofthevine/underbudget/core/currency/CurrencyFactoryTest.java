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

import static org.junit.Assert.*;

import org.junit.Test;

/**
 * Unit test case for the CurrencyFactory class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CurrencyFactoryTest {

	/**
	 * Verifies that the currency factory cannot
	 * be initialized with a non-currency class
	 */
	@Test(expected=IllegalArgumentException.class)
	public final void testInvalidCurrencyClass()
	{
		new CurrencyFactory(CurrencyFactoryTest.class);
	}
	
	/**
	 * Verifies that the currency factory cannot
	 * be initiated with an invalid currency code
	 */
	@Test(expected=IllegalArgumentException.class)
	public final void testInvalidCurrencyCode()
	{
		new CurrencyFactory("BLAH");
	}
	
	/**
	 * Verifies that the newCurrencyInstance methods
	 * returns new currency objects of the correct
	 * type
	 */
	@Test
	public final void testNewCurrencyInstanceByClass()
	{
		CurrencyFactory abcCurrencyFactory = new CurrencyFactory(ABCCurrency.class);
		CurrencyFactory xyzCurrencyFactory = new CurrencyFactory(XYZCurrency.class);
		
		assertTrue(abcCurrencyFactory.newCurrencyInstance() instanceof ABCCurrency);
		assertTrue(abcCurrencyFactory.newCurrencyInstance("0") instanceof ABCCurrency);
		assertTrue(xyzCurrencyFactory.newCurrencyInstance() instanceof XYZCurrency);
		assertTrue(xyzCurrencyFactory.newCurrencyInstance("0") instanceof XYZCurrency);
	}
	
	/**
	 * Verifies that the newCurrencyInstance methods
	 * returns new currency objects of the correct
	 * type
	 */
	@Test
	public final void testNewCurrencyInstanceByCode()
	{
		CurrencyFactory abcCurrencyFactory = new CurrencyFactory("ABC");
		CurrencyFactory xyzCurrencyFactory = new CurrencyFactory("XYZ");
		
		assertTrue(abcCurrencyFactory.newCurrencyInstance() instanceof ABCCurrency);
		assertTrue(abcCurrencyFactory.newCurrencyInstance("0") instanceof ABCCurrency);
		assertTrue(xyzCurrencyFactory.newCurrencyInstance() instanceof XYZCurrency);
		assertTrue(xyzCurrencyFactory.newCurrencyInstance("0") instanceof XYZCurrency);
	}
	
	/**
	 * Verifies that the newCurrencyInstance methods
	 * throw an exception if the currency object could
	 * not be instantiated
	 */
	@Test(expected=RuntimeException.class)
	public final void testNewCurrencyInstanceThrowsException()
	{
		CurrencyFactory factory = new CurrencyFactory(JFKCurrency.class);
		factory.newCurrencyInstance("0");
	}
	
	/**
	 * Stub currency type (purposely non-static)
	 */
	private class JFKCurrency implements Currency
	{
		@Override
		public Currency clone()
		{
			return new JFKCurrency();
		}
		
		@Override
        public int compareTo(Currency o)
        {
	        return 0;
        }

		@Override
		public boolean within(float percent, Currency o) { return false; }

		@Override
        public void parse(String amount) { }

		@Override
        public Currency increaseBy(Currency increase) { return null; }

		@Override
        public Currency decreaseBy(Currency decrease) { return null; }

		@Override
        public boolean isNegative()
        {
	        return false;
        }

		@Override
        public String formatAsString()
        {
	        return null;
        }

		@Override
        public boolean isZero()
        {
	        return false;
        }
		
	}

}
