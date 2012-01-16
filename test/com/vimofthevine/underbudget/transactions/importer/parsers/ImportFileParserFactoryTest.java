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

package com.vimofthevine.underbudget.transactions.importer.parsers;

import static org.junit.Assert.*;

import java.io.InputStream;

import org.junit.Test;

import com.vimofthevine.underbudget.transactions.file.parsers.TransactionFileDomParserTest;
import com.vimofthevine.underbudget.transactions.file.parsers.TransactionFileParser;

/**
 * Unit test case for the ImportFileParserFactory class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ImportFileParserFactoryTest {
	
	/**
	 * Verifies that the transaction file parser is
	 * created for transaction XML files
	 */
	@Test
	public final void testCreateParserTransactionXml()
	{
		try
		{
			InputStream stream = TransactionFileDomParserTest.class.getResourceAsStream("transactions.xml");
			ImportFileParser parser = ImportFileParserFactory.createParser(stream);
			assertTrue(parser instanceof TransactionFileParser);
		}
		catch (Exception e)
		{
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
			ImportFileParser parser = ImportFileParserFactory.createParser(stream);
			assertTrue(parser instanceof GnuCashFileParser);
		}
		catch (Exception e)
		{
			fail("Error occurred executing test");
		}
	}
	
	/**
	 * Verifies that the Mint CSV file parser is
	 * created for Mint CSV files
	 */
	@Test
	public final void testCreateParserMintCsv()
	{
		try
		{
			InputStream stream = getClass().getResourceAsStream("mint.txt");
			ImportFileParser parser = ImportFileParserFactory.createParser(stream);
			assertTrue(parser instanceof MintCsvFileParser);
		}
		catch (Exception e)
		{
			e.printStackTrace();
			fail("Error occurred executing test");
		}
	}

}
