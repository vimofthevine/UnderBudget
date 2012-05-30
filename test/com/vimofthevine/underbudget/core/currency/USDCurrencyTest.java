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
 * Unit test case for the USDCurrency class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class USDCurrencyTest {

	/**
	 * Verifies that the increaseBy method
	 * adds the value of the given currency
	 */
	@Test
	public final void testIncreaseBy()
	{
		USDCurrency dollar = new USDCurrency("10.0");
		dollar.increaseBy(new USDCurrency("5.35"));
		assertEquals("$15.35", dollar.formatAsString());
	}
	
	/**
	 * Verifies that the increaseBy method
	 * throws an exception if an incompatible
	 * currency type is given
	 */
	@Test(expected=IllegalArgumentException.class)
	public final void testIncreaseByThrowsException()
	{
		USDCurrency dollar = new USDCurrency("10.0");
		dollar.increaseBy(new StubCurrency());
	}

	/**
	 * Verifies that the decreaseBy method
	 * subtracts the value of the given currency
	 */
	@Test
	public final void testDecreaseBy()
	{
		USDCurrency dollar = new USDCurrency("10.0");
		dollar.decreaseBy(new USDCurrency("5.35"));
		assertEquals("$4.65", dollar.formatAsString());
	}
	
	/**
	 * Verifies that the decreaseBy method
	 * throws an exception if an incompatible
	 * currency type is given
	 */
	@Test(expected=IllegalArgumentException.class)
	public final void testDecreaseByThrowsException()
	{
		USDCurrency dollar = new USDCurrency("10.0");
		dollar.decreaseBy(new StubCurrency());
	}

	/**
	 * Verifies that the equals method correctly
	 * determines when a given currency is of equal value
	 */
	@Test
	public final void testEquals()
	{
		USDCurrency base = new USDCurrency("12.34");
		USDCurrency equal = new USDCurrency("12.34");
		USDCurrency diff = new USDCurrency("23.45");
		
		assertEquals(base, equal);
		assertTrue(base.equals(equal));
		assertFalse(base.equals(diff));
		assertFalse(base.equals(new StubCurrency()));
	}
	
	/**
	 * Verifies that the compareTo method correctly
	 * compares currency values
	 */
	@Test
	public final void testCompareTo()
	{
		USDCurrency base = new USDCurrency("10.00");
		USDCurrency same = new USDCurrency("10.0");
		USDCurrency lower = new USDCurrency("5.00");
		USDCurrency higher = new USDCurrency("15.00");
		
		assertEquals(0, base.compareTo(same));
		assertEquals(1, base.compareTo(lower));
		assertEquals(-1, base.compareTo(higher));
	}
	
	/**
	 * Verifies that the within method correctly
	 * compares currency values
	 */
	@Test
	public final void testWithin()
	{
		USDCurrency base = new USDCurrency("100.00");
		USDCurrency small = new USDCurrency("20.00");
		USDCurrency close = new USDCurrency("98.00");
		USDCurrency over = new USDCurrency("102.00");
		USDCurrency large = new USDCurrency("106.00");
		
		assertFalse(small.within(0.05f, base));
		assertTrue(close.within(0.05f, base));
		assertTrue(over.within(0.05f, base));
		assertFalse(large.within(0.05f, base));
	}
	
	/**
	 * Verifies that the compareTo method
	 * throws an exception if an incompatible
	 * currency type is given
	 */
	@Test(expected=IllegalArgumentException.class)
	public final void testCompareToThrowsException()
	{
		USDCurrency dollar = new USDCurrency("10.0");
		dollar.compareTo(new StubCurrency());
	}
	
	/**
	 * Verifies that the isNegative method correctly
	 * determines whether the currency value is below 0
	 */
	@Test
	public final void testIsNegative()
	{
		USDCurrency negative = new USDCurrency("-10.0");
		USDCurrency zero = new USDCurrency("0.0");
		USDCurrency positive = new USDCurrency("10.0");
		
		assertTrue(negative.isNegative());
		assertFalse(zero.isNegative());
		assertFalse(positive.isNegative());
	}
	
	/**
	 * Stub currency class
	 */
	class StubCurrency implements Currency {

		@Override
		public Currency clone()
		{
			return null;
		}
		
		@Override
		public void parse(String string)
		{
			// Do nothing
		}
		
		@Override
        public Currency increaseBy(Currency increase)
        {
			return null;
        }

		@Override
        public Currency decreaseBy(Currency decrease)
        {
			return null;
        }

		@Override
        public String formatAsString()
        {
	        return null;
        }

		@Override
        public int compareTo(Currency arg0)
        {
	        return 0;
        }
		
		@Override
		public boolean within(float percent, Currency o) { return false; }

		@Override
        public boolean isNegative()
        {
	        return false;
        }

		@Override
        public boolean isZero()
        {
	        return false;
        }
		
	}

}
