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
import java.math.BigDecimal;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

import org.junit.Before;
import org.junit.Test;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import com.vimofthevine.underbudget.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.budget.period.MonthlyBudgetingPeriod;
import com.vimofthevine.underbudget.stubs.StubTaskProgressListener;
import com.vimofthevine.underbudget.transactions.Transaction;

/**
 * Unit test case for the CsvFileParser class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CsvFileParserTest {
	
	/**
	 * The class under test
	 */
	ImportFileParser parser;
	
	/**
	 * The import file stream
	 */
	InputStream stream;
	
	/**
	 * Stub task progress listener
	 */
	StubTaskProgressListener listener;
	
	/**
	 * Sets up the test objects
	 */
	@Before
	public void setUp()
	{
		try
		{
			listener = new StubTaskProgressListener();
			
			parser = new CsvFileParser();
			parser.getProgress().addTaskProgressListener(listener);
		}
		catch (Exception e)
		{
			fail("Error occurred executing test");
		}
	}

	/**
	 * Verifies that transactions are parsed correctly
	 * from a Mint CSV file
	 */
	@Test
	public void testParseMintCsvTransactions()
	{		
		try
        {
			String[] expectedPayees = new String[] {
				"Direct deposit",
				"Mr. Mechanic",
				"Vendor",
				"Vendor",
			};
				
			String[] expectedMemos = new String[] {
				"",
				"Tires",
				"Entertainment",
				"Food",
			};
				
			SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
			Date[] expectedDates = new Date[] {
				format.parse("2010-11-05"),
				format.parse("2011-12-07"),
				format.parse("2010-07-28"),
				format.parse("2010-07-28"),
			};
				
			BigDecimal[] expectedValues = new BigDecimal[] {
				new BigDecimal("650.03"),
				new BigDecimal("402.44"),
				new BigDecimal("150.00"),
				new BigDecimal("14.68"),
			};
				
			String[] expectedDepositAccts = new String[] {
				"My Bank",
				"Auto Repair",
				"Recreation",
				"Dining",
			};
				
			String[] expectedWithdrawalAccts = new String[] {
				"Paycheck",
				"My Bank",
				"My Bank",
				"My Bank",
			};
			
			stream = getClass().getResourceAsStream("mint.txt");
	        parser.parse(stream, new StubBudgetingPeriod());
			
			List<Transaction> transactions = parser.getTransactions();
			
			for (int i=0; i<transactions.size(); i++)
			{
				Transaction transaction = transactions.get(i);
				
				assertEquals(expectedPayees[i], transaction.payee);
				assertEquals(expectedMemos[i], transaction.memo);
				assertEquals(expectedDates[i], transaction.date);
				assertEquals(expectedValues[i], transaction.value);
				assertEquals(expectedDepositAccts[i], transaction.deposit.getName());
				assertEquals(expectedWithdrawalAccts[i], transaction.withdrawal.getName());
			}
			
        }
		catch (Exception e)
        {
			fail("Error occurred executing test");
        }
	}
	
	/**
	 * Verifies that transactions are parsed correctly
	 * from a generic CSV file
	 */
	@Test
	public void testParseGenericCsvTransactions()
	{		
		try
        {
			String[] expectedPayees = new String[] {
				"Direct deposit",
				"Mr. Mechanic",
				"Vendor",
				"Vendor",
			};
				
			SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
			Date[] expectedDates = new Date[] {
				format.parse("2010-11-05"),
				format.parse("2011-12-07"),
				format.parse("2010-07-28"),
				format.parse("2010-07-28"),
			};
				
			BigDecimal[] expectedValues = new BigDecimal[] {
				new BigDecimal("650.03"),
				new BigDecimal("402.44"),
				new BigDecimal("150.00"),
				new BigDecimal("14.68"),
			};
			
			stream = getClass().getResourceAsStream("bank.txt");
	        parser.parse(stream, new StubBudgetingPeriod());
			
			List<Transaction> transactions = parser.getTransactions();
			
			for (int i=0; i<transactions.size(); i++)
			{
				Transaction transaction = transactions.get(i);
				
				assertEquals(expectedPayees[i], transaction.payee);
				assertEquals("", transaction.memo);
				assertEquals(expectedDates[i], transaction.date);
				assertEquals(expectedValues[i], transaction.value);
				assertEquals("", transaction.deposit.getName());
				assertEquals("", transaction.withdrawal.getName());
			}
			
        }
		catch (Exception e)
        {
			e.printStackTrace();
			fail("Error occurred executing test");
        }
	}
	
	/**
	 * Verifies that task progress is updated while parsing
	 */
	@Test
	public void testParseProgress()
	{
		try
		{
			assertEquals(0, listener.lastValue);
			
			stream = getClass().getResourceAsStream("mint.txt");
			parser.parse(stream, new StubBudgetingPeriod());
			
			assertEquals(100, listener.lastValue);
			// Can't be certain how often the stream will be read
			assertTrue(listener.numberOfUpdates > 1);
		}
		catch (Exception e)
        {
			fail("Error occurred executing test");
        }
	}
	
	/**
	 * Verifies that transactions are filtered by the
	 * budgeting period
	 */
	@Test
	public void testParseDateFiltered()
	{
		try
		{
			stream = getClass().getResourceAsStream("mint.txt");
			parser.parse(stream, new MonthlyBudgetingPeriod(11, 2011));
			
			assertEquals(1, parser.getTransactions().size());
		}
		catch (Exception e)
		{
			fail("Error occurred executing test");
		}
	}
	
	/**
	 * Stub budgeting period for unit testing
	 */
	class StubBudgetingPeriod implements BudgetingPeriod {
		
		@Override
	    public Date getStartDate()
	    {
			return new Date(0);
	    }

		@Override
	    public Date getEndDate()
	    {
			return new Date();
	    }

		@Override
	    public void store(Document doc, Element element)
	    {
			// Do nothing
	    }

		@Override
	    public BudgetingPeriod parse(Element element)
	    {
		    return null;
	    }
	}

}
