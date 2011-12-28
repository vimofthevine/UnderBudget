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
 * Unit test case for the GnuCashFileParserTest
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class GnuCashFileParserTest {
	
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
			
			parser = new GnuCashFileParser();
			parser.getProgress().addTaskProgressListener(listener);
			
			stream = getClass().getResourceAsStream("gnucash.xml");
		}
		catch (Exception e)
		{
			fail("Error occurred executing test");
		}
	}

	/**
	 * Verifies that transactions are parsed correctly
	 * from a GnuCash XML file
	 */
	@Test
	public void testParseTransactions()
	{		
		try
        {
			String[] expectedPayees = new String[] {
					"Payday",
					"Payday",
					"Payday",
					"Payday",
					"Mr. Mechanic",
					"Vendor",
					"Vendor",
				};
				
				String[] expectedMemos = new String[] {
					"",
					"",
					"",
					"Earned",
					"Tires",
					"Entertainment",
					"Food",
				};
				
				SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
				Date[] expectedDates = new Date[] {
					format.parse("2010-11-05"),
					format.parse("2010-11-05"),
					format.parse("2010-11-05"),
					format.parse("2010-11-05"),
					format.parse("2011-12-07"),
					format.parse("2010-07-28"),
					format.parse("2010-07-28"),
				};
				
				BigDecimal[] expectedValues = new BigDecimal[] {
					new BigDecimal("150.0"),
					new BigDecimal("67.25"),
					new BigDecimal("29.76"),
					new BigDecimal("650.03"),
					new BigDecimal("402.44"),
					new BigDecimal("150.0"),
					new BigDecimal("14.68"),
				};
				
				String[] expectedDepositAccts = new String[] {
					"Expenses:Taxes:Federal",
					"Expenses:Taxes:State",
					"Expenses:Insurance:Health Insurance",
					"Assets:Current Assets:Checking Account",
					"Expenses:Auto:Repair and Maintenance",
					"Expenses:Entertainment:Recreation",
					"Expenses:Entertainment:Dining",
				};
				
				String[] expectedWithdrawalAccts = new String[] {
					"Income:Salary",
					"Income:Salary",
					"Income:Salary",
					"Income:Salary",
					"Assets:Current Assets:Checking Account",
					"Assets:Current Assets:Checking Account",
					"Assets:Current Assets:Checking Account",
				};
				
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
	 * Verifies that task progress is updated while parsing
	 */
	@Test
	public void testParseProgress()
	{
		try
		{
			assertEquals(0, listener.lastValue);
			
			parser.parse(stream, new StubBudgetingPeriod());
			
			assertEquals(100, listener.lastValue);
			assertTrue(listener.numberOfUpdates > 10);
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
