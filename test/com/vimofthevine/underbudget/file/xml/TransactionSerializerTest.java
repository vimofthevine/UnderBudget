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

import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.core.ledger.Account;
import com.vimofthevine.underbudget.core.ledger.Ledger;
import com.vimofthevine.underbudget.core.ledger.transaction.ImportedTransaction;
import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * Unit test case for the Simple xml framework
 * serializer used for transaction files
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionSerializerTest {
	
	/**
	 * Currency factory
	 */
	private final static CurrencyFactory currency = new CurrencyFactory("USD");

	/**
	 * Verifies that transactions are serialized correctly
	 */
	@Test
	public final void testTransactionSerialization()
	{
		final String expected = TransactionSerializerFactory.PROLOG + "\n" +
			"<transaction>\n" +
			"   <date-posted>\n" +
			"      <day>5</day>\n" +
			"      <month>10</month>\n" +
			"      <year>2010</year>\n" +
			"   </date-posted>\n" +
			"   <payee>Direct deposit</payee>\n" +
			"   <memo></memo>\n" +
			"   <amount currency=\"USD\">650.03</amount>\n" +
			"   <withdrawal-acct>Paycheck</withdrawal-acct>\n" +
			"   <deposit-acct>My Bank</deposit-acct>\n" +
			"</transaction>";
		final ImportedTransaction input = new ImportedTransaction(new SimpleDate(5, 10, 2010),
			"Direct deposit", "", currency.newCurrencyInstance("650.03"),
			new Account("Paycheck"), new Account("My Bank"));
		
		Serializer serializer = TransactionSerializerFactory.createSerializer();
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
	 * Verifies that transactions are deserialized correctly
	 */
	@Test
	public final void testTransactionDeserialization()
	{
		final String input = TransactionSerializerFactory.PROLOG + "\n" +
			"<transaction>\n" +
			"   <date-posted>\n" +
			"      <day>7</day>\n" +
			"      <month>11</month>\n" +
			"      <year>2011</year>\n" +
			"   </date-posted>\n" +
			"   <payee>Mr. Mechanic</payee>\n" +
			"   <memo>Tires</memo>\n" +
			"   <amount currency=\"USD\">402.44</amount>\n" +
			"   <withdrawal-acct>My Bank</withdrawal-acct>\n" +
			"   <deposit-acct>Auto Repair</deposit-acct>\n" +
			"</transaction>";
		final ImportedTransaction expected = new ImportedTransaction(new SimpleDate(7, 11, 2011),
			"Mr. Mechanic", "Tires", currency.newCurrencyInstance("402.44"),
			new Account("My Bank"), new Account("Auto Repair"));
		
		Serializer serializer = TransactionSerializerFactory.createSerializer();
		
		try
		{
			ImportedTransaction actual = serializer.read(ImportedTransaction.class, input);
			assertEquals(expected, actual);
		}
		catch (Exception e)
		{
			e.printStackTrace();
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}

	/**
	 * Verifies that ledgers are serialized correctly
	 */
	@Test
	public final void testLedgerSerialization()
	{
		final String expected = TransactionSerializerFactory.PROLOG + "\n" +
			"<ledger>\n" +
			"   <transactions>\n" +
			"      <transaction>\n" +
			"         <date-posted>\n" +
			"            <day>5</day>\n" +
			"            <month>10</month>\n" +
			"            <year>2010</year>\n" +
			"         </date-posted>\n" +
			"         <payee>Direct deposit</payee>\n" +
			"         <memo></memo>\n" +
			"         <amount currency=\"USD\">650.03</amount>\n" +
			"         <withdrawal-acct>Paycheck</withdrawal-acct>\n" +
			"         <deposit-acct>My Bank</deposit-acct>\n" +
			"      </transaction>\n" +
			"      <transaction>\n" +
			"         <date-posted>\n" +
			"            <day>7</day>\n" +
			"            <month>11</month>\n" +
			"            <year>2011</year>\n" +
			"         </date-posted>\n" +
			"         <payee>Mr. Mechanic</payee>\n" +
			"         <memo>Tires</memo>\n" +
			"         <amount currency=\"USD\">402.44</amount>\n" +
			"         <withdrawal-acct>My Bank</withdrawal-acct>\n" +
			"         <deposit-acct>Auto Repair</deposit-acct>\n" +
			"      </transaction>\n" + 
			"   </transactions>\n" +
			"</ledger>";
		ImportedTransaction firstTransaction = new ImportedTransaction(new SimpleDate(5, 10, 2010),
			"Direct deposit", "", currency.newCurrencyInstance("650.03"),
			new Account("Paycheck"), new Account("My Bank"));
		ImportedTransaction secondTransaction = new ImportedTransaction(new SimpleDate(7, 11, 2011),
			"Mr. Mechanic", "Tires", currency.newCurrencyInstance("402.44"),
			new Account("My Bank"), new Account("Auto Repair"));
		final Ledger input = new Ledger();
		input.getTransactionList().add(firstTransaction);
		input.getTransactionList().add(secondTransaction);
		
		Serializer serializer = TransactionSerializerFactory.createSerializer();
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
	 * Verifies that ledgers are deserialized correctly
	 */
	@Test
	public final void testLedgerDeserialization()
	{
		final String input = TransactionSerializerFactory.PROLOG + "\n" +
			"<ledger>\n" +
			"   <transactions>\n" +
			"      <transaction>\n" +
			"         <date-posted>\n" +
			"            <day>5</day>\n" +
			"            <month>10</month>\n" +
			"            <year>2010</year>\n" +
			"         </date-posted>\n" +
			"         <payee>Direct deposit</payee>\n" +
			"         <memo></memo>\n" +
			"         <amount currency=\"USD\">650.03</amount>\n" +
			"         <withdrawal-acct>Paycheck</withdrawal-acct>\n" +
			"         <deposit-acct>My Bank</deposit-acct>\n" +
			"      </transaction>\n" +
			"      <transaction>\n" +
			"         <date-posted>\n" +
			"            <day>7</day>\n" +
			"            <month>11</month>\n" +
			"            <year>2011</year>\n" +
			"         </date-posted>\n" +
			"         <payee>Mr. Mechanic</payee>\n" +
			"         <memo>Tires</memo>\n" +
			"         <amount currency=\"USD\">402.44</amount>\n" +
			"         <withdrawal-acct>My Bank</withdrawal-acct>\n" +
			"         <deposit-acct>Auto Repair</deposit-acct>\n" +
			"      </transaction>\n" + 
			"   </transactions>\n" +
			"</ledger>";
		ImportedTransaction firstTransaction = new ImportedTransaction(new SimpleDate(5, 10, 2010),
			"Direct deposit", "", currency.newCurrencyInstance("650.03"),
			new Account("Paycheck"), new Account("My Bank"));
		ImportedTransaction secondTransaction = new ImportedTransaction(new SimpleDate(7, 11, 2011),
			"Mr. Mechanic", "Tires", currency.newCurrencyInstance("402.44"),
			new Account("My Bank"), new Account("Auto Repair"));
		final Ledger expected = new Ledger();
		expected.getTransactionList().add(firstTransaction);
		expected.getTransactionList().add(secondTransaction);
		
		Serializer serializer = TransactionSerializerFactory.createSerializer();
		
		try
		{
			Ledger actual = serializer.read(Ledger.class, input);
			assertEquals(expected, actual);
		}
		catch (Exception e)
		{
			fail("Exception thrown during deserialization, " + e.getMessage());
		}
	}
}
