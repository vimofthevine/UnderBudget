package com.vimofthevine.underbudget.analysis;

import static org.junit.Assert.*;

import java.util.ArrayList;

import org.junit.Before;
import org.junit.Test;

import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.estimates.rules.ComparisonOperator;
import com.vimofthevine.underbudget.estimates.rules.Rule;
import com.vimofthevine.underbudget.stubs.StubTaskProgressListener;
import com.vimofthevine.underbudget.transactions.Account;
import com.vimofthevine.underbudget.transactions.Transaction;
import com.vimofthevine.underbudget.transactions.TransactionField;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Unit test case for the TransactionMatcher class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionMatcherTest {
	
	/**
	 * The class under test
	 */
	protected TransactionMatcher matcher;
	
	/**
	 * Stub task progress listener
	 */
	protected StubTaskProgressListener listener;
	
	/**
	 * List of transactions
	 */
	protected ArrayList<Transaction> transactions;
	
	/**
	 * List of estimate rules
	 */
	protected ArrayList<EstimateRule> rules;
	
	/**
	 * Array of expected transactions as matched
	 */
	Transaction[] expectedTransactions;
	
	/**
	 * Setup the test objects
	 */
	@Before
	public void setUp()
	{
		TaskProgress progress = new TaskProgress();
		matcher = new TransactionMatcher(progress, 100);
		
		listener = new StubTaskProgressListener();
		progress.addTaskProgressListener(listener);
		
		createTransactions();
		createRules();
	}

	/**
	 * Verifies that the transactions are correctly matched
	 */
	@Test
	public void testMatch()
	{	
		matcher.match(transactions, rules);
		
		for (int i=0; i<rules.size(); i++)
		{
			Estimate estimate = rules.get(i).estimate;
			assertEquals(estimate.getName() + " estimate expected to have 1 transaction",
				1, estimate.getTransactions().size());
			assertSame(expectedTransactions[i], estimate.getTransactions().get(0));
		}
	}
	
	/**
	 * Verifies that the task progress is updated
	 */
	@Test
	public void testMatchProgress()
	{
		assertEquals(0, listener.lastValue);
		
		matcher.match(transactions, rules);
		
		assertEquals(100, listener.lastValue);
		assertEquals(4, listener.numberOfUpdates);
	}
	
	/**
	 * Sets up test transactions
	 */
	protected void createTransactions()
	{
		transactions = new ArrayList<Transaction>();
		expectedTransactions = new Transaction[4];
		
		// Set up some accounts
		Account root = new Account("", "Root", "", null);
		Account income = new Account("", "Income", "", root);
		Account salary = new Account("", "Salary", "", income);
		Account expense = new Account("", "Expense", "", root);
		Account shelter = new Account("", "Shelter", "", expense);
		Account luxury = new Account("", "Luxury", "", expense);
		Account asset = new Account("", "Asset", "", root);
		Account bank = new Account("", "Bank", "", asset);
		
		Transaction trans1 = new Transaction();
		trans1.payee = "vendor1";
		trans1.memo  = "product1";
		trans1.withdrawal = bank;
		trans1.deposit = luxury;
		transactions.add(trans1);
		expectedTransactions[3] = trans1;
		
		Transaction trans2 = new Transaction();
		trans2.payee = "vendor1";
		trans2.memo  = "product2";
		trans2.withdrawal = bank;
		trans2.deposit = shelter;
		transactions.add(trans2);
		expectedTransactions[0] = trans2;
		
		Transaction trans3 = new Transaction();
		trans3.payee = "payday";
		trans3.withdrawal = salary;
		trans3.deposit = bank;
		transactions.add(trans3);
		expectedTransactions[1] = trans3;
		
		Transaction trans4 = new Transaction();
		trans4.payee = "vendor2";
		trans4.memo = "refund";
		trans4.withdrawal = shelter;
		trans4.deposit = bank;
		transactions.add(trans4);
		expectedTransactions[2] = trans4;
	}
	
	/**
	 * Sets up test rules
	 */
	protected void createRules()
	{
		rules = new ArrayList<EstimateRule>();
		
		Estimate estimate1 = new Estimate();
		estimate1.setName("product2");
		Estimate estimate2 = new Estimate();
		estimate2.setName("salary");
		Estimate estimate3 = new Estimate();
		estimate3.setName("asset");
		Estimate estimate4 = new Estimate();
		estimate4.setName("vendor");
		
		rules.add(new EstimateRule(estimate1,
			new Rule(TransactionField.MEMO, ComparisonOperator.EQUALS, "product2")));
		rules.add(new EstimateRule(estimate2,
			new Rule(TransactionField.WITHDRAWAL, ComparisonOperator.ENDS_WITH, "Salary")));
		rules.add(new EstimateRule(estimate3,
			new Rule(TransactionField.DEPOSIT, ComparisonOperator.BEGINS_WITH, "Asset")));
		rules.add(new EstimateRule(estimate4,
			new Rule(TransactionField.ANY, ComparisonOperator.CONTAINS, "vendor")));
	}

}
