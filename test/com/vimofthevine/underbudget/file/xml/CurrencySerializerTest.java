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

package com.vimofthevine.underbudget.file.xml;

import static org.junit.Assert.*;

import java.io.StringWriter;

import org.junit.Test;
import org.simpleframework.xml.Serializer;

import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.core.currency.USDCurrency;

/**
 * Unit test case for the Simple xml framework
 * serializer used for currency values
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CurrencySerializerTest {
	
	/**
	 * Verifies that USD currency is serialized correctly
	 */
	@Test
	public final void testUsdCurrencySerialization()
	{
		final String expected = BudgetSerializerFactory.PROLOG + "\n" +
			"<amount currency=\"USD\">234.56</amount>";
		final CurrencyFactory factory = new CurrencyFactory(USDCurrency.class);
		final Currency input = factory.newCurrencyInstance("234.56");
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		StringWriter writer = new StringWriter();
		
		try
		{
			serializer.write(input, writer);
			assertEquals(expected, writer.toString());
		}
		catch (Exception e)
		{
			fail("Exception thrown during serialization, " + e.getMessage());
		}
	}
	
	/**
	 * Verifies that USD currency is deserialized correctly
	 */
	@Test
	public final void testUsdCurrencyDeserialization()
	{
		final String input = BudgetSerializerFactory.PROLOG + "\n" +
			"<amount currency=\"USD\">2400.25</amount>";
		final CurrencyFactory factory = new CurrencyFactory(USDCurrency.class);
		final Currency expected = factory.newCurrencyInstance("2400.25");
		
		Serializer serializer = BudgetSerializerFactory.createSerializer();
		
		try
		{
			USDCurrency actual = serializer.read(USDCurrency.class, input);
			assertEquals(expected, actual);
		}
		catch (Exception e)
		{
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}
	
}
