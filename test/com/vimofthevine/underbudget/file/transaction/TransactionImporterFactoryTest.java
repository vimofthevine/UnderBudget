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

package com.vimofthevine.underbudget.file.transaction;

import static org.junit.Assert.*;

import java.io.InputStream;

import org.junit.Before;
import org.junit.Test;

import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.file.transaction.CsvImporter;
import com.vimofthevine.underbudget.file.transaction.GnuCashImporter;

/**
 * Unit test case for the TransactionImporterFactory class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionImporterFactoryTest {
	
	/**
	 * Transaction importer factory
	 */
	private TransactionImporterFactory factory;
	
	/**
	 * Sets up the test case objects.
	 */
	@Before
	public final void setUp()
	{
		factory = new TransactionImporterFactory(new CurrencyFactory("USD"));
	}
	
	/**
	 * Verifies that the transaction file importer is
	 * created for transaction XML files
	 */
	@Test
	public final void testCreateParserTransactionXml()
	{
		try
		{
			InputStream stream = getClass().getResourceAsStream("transactions.xml");
			TransactionImporter importer = factory.createImporter(stream);
			assertNotNull(importer);
			assertTrue("Expected TransactionFileImporter but got " + importer.getClass().getName(),
				importer instanceof TransactionFileImporter);
		}
		catch (Exception e)
		{
			e.printStackTrace();
			fail("Error occurred executing test");
		}
	}

	/**
	 * Verifies that the GnuCash file parser is
	 * created for GnuCash XML files
	 */
	@Test
	public final void testCreateParserGnuCash()
	{
		try
		{
			InputStream stream = getClass().getResourceAsStream("gnucash.xml");
			TransactionImporter importer = factory.createImporter(stream);
			assertNotNull(importer);
			assertTrue("Expected GnuCashImporter but got " + importer.getClass().getName(),
				importer instanceof GnuCashImporter);
		}
		catch (Exception e)
		{
			fail("Error occurred executing test");
		}
	}
	
	/**
	 * Verifies that the CSV file parser is
	 * created for CSV files
	 */
	@Test
	public final void testCreateParserCsv()
	{
		try
		{
			InputStream stream = getClass().getResourceAsStream("mint.txt");
			TransactionImporter importer = factory.createImporter(stream);
			assertNotNull(importer);
			assertTrue("Expected CsvImporter but got " + importer.getClass().getName(),
				importer instanceof CsvImporter);
		}
		catch (Exception e)
		{
			fail("Error occurred executing test");
		}
	}

}
