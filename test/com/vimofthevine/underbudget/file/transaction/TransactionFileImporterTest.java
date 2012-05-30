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
import java.util.List;

import org.junit.Before;
import org.junit.Test;

import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.budget.period.MonthlyBudgetingPeriod;
import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
import com.vimofthevine.underbudget.core.ledger.Account;
import com.vimofthevine.underbudget.core.ledger.transaction.ImportedTransaction;
import com.vimofthevine.underbudget.core.ledger.transaction.Transaction;
import com.vimofthevine.underbudget.core.progress.TaskProgress;
import com.vimofthevine.underbudget.core.util.SimpleDate;
import com.vimofthevine.underbudget.stubs.StubTaskProgressListener;

/**
 * Unit test case for the TransactionFileImporter class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionFileImporterTest {

	/**
	 * The transaction file importer
	 */
	private TransactionFileImporter importer;
	
	/**
	 * Currency factory
	 */
	private CurrencyFactory currencyFactory;
	
	/**
	 * The import file stream
	 */
	private InputStream stream;
	
	/**
	 * Import task progress
	 */
	private TaskProgress progress;
	
	/**
	 * Stub task progress listener
	 */
	private StubTaskProgressListener listener;
	
	/**
	 * Sets up the test objects
	 */
	@Before
	public void setUp()
	{
		try
		{
			progress = new TaskProgress("Transaction file import");
			
			listener = new StubTaskProgressListener();
			progress.addTaskProgressListener(listener);
			
			currencyFactory = new CurrencyFactory("USD");
			
			importer = new TransactionFileImporter();
			
			stream = getClass().getResourceAsStream("transactions.xml");
		}
		catch (Exception e)
		{
			fail("Error occurred executing test");
		}
	}
	
	/**
	 * Verifies that transactions are parsed correctly
	 * from an XML transactions file
	 */
	@Test
	public final void testParseTransactionFile()
	{
		try
		{
			Transaction[] expected = new Transaction[] {
				new ImportedTransaction(new SimpleDate(5, 10, 2010), "Direct deposit",
					"", currencyFactory.newCurrencyInstance("650.03"),
					new Account("Paycheck"), new Account("My Bank")),
				new ImportedTransaction(new SimpleDate(7, 11, 2011), "Mr. Mechanic",
					"Tires", currencyFactory.newCurrencyInstance("402.44"),
					new Account("My Bank"), new Account("Auto Repair")),
				new ImportedTransaction(new SimpleDate(28, 6, 2010), "Vendor",
					"Entertainment", currencyFactory.newCurrencyInstance("150.00"),
					new Account("My Bank"), new Account("Recreation")),
				new ImportedTransaction(new SimpleDate(28, 6, 2010), "Vendor",
					"Food", currencyFactory.newCurrencyInstance("14.68"),
					new Account("My Bank"), new Account("Dining")),
			};
			
			List<Transaction> transactions = importer.read(stream, new StubBudgetingPeriod(), progress);
			
			assertEquals(expected.length, transactions.size());
			for (int i=0; i<transactions.size(); i++)
			{
				Transaction transaction = transactions.get(i);
				assertEquals(expected[i], transaction);
			}
		}
		catch (Exception e)
		{
			e.printStackTrace(System.err);
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
			
			importer.read(stream, new StubBudgetingPeriod(), progress);
			
			assertEquals(100, listener.lastValue);
			// At least a reset, some intermediate progress, and a complete
			assertTrue(listener.numberOfUpdates > 2);
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
			List<Transaction> transactions = importer.read(stream, new MonthlyBudgetingPeriod(11, 2011), progress);
			
			assertEquals(1, transactions.size());
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
		public boolean contains(SimpleDate date)
		{
			return true;
		}

		@Override
        public SimpleDate getStartDate()
        {
	        return null;
        }

		@Override
        public SimpleDate getEndDate()
        {
	        return null;
        }
		
	}
	
}
