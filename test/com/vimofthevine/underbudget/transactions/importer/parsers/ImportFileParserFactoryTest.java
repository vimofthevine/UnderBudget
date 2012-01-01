package com.vimofthevine.underbudget.transactions.importer.parsers;

import static org.junit.Assert.*;

import java.io.InputStream;

import org.junit.Test;

/**
 * Unit test case for the ImportFileParserFactory class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ImportFileParserFactoryTest {

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
