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

package com.vimofthevine.underbudget.analysis;

import static org.junit.Assert.*;

import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.junit.Before;
import org.junit.Test;

import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.estimates.rules.ComparisonOperator;
import com.vimofthevine.underbudget.estimates.rules.Rule;
import com.vimofthevine.underbudget.estimates.rules.UnbudgetedExpenseRule;
import com.vimofthevine.underbudget.estimates.rules.UnbudgetedIncomeRule;
import com.vimofthevine.underbudget.stubs.StubTaskProgressListener;
import com.vimofthevine.underbudget.transactions.TransactionField;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Unit test case for the RuleSorter class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class RuleSorterTest {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(RuleSorterTest.class.getName());
	
	/**
	 * The class under test
	 */
	protected RuleSorter sorter;
	
	/**
	 * Stub task progress listener
	 */
	protected StubTaskProgressListener listener;
	
	/**
	 * Root income estimate
	 */
	protected Estimate income;
	
	/**
	 * Root expense estimate
	 */
	protected Estimate expense;
	
	/**
	 * Setup the test objects
	 */
	@Before
	public void setUp()
	{
		TaskProgress progress = new TaskProgress();
		sorter = new RuleSorter(progress, 100);
		
		listener = new StubTaskProgressListener();
		progress.addTaskProgressListener(listener);
		
		createEstimates();
	}

	/**
	 * Verifies that the estimate rules are sorted correctly
	 */
	@Test
	public void testSort()
	{
		try
		{
			Rule[] expectedRules = new Rule[] {
				new Rule(TransactionField.MEMO, ComparisonOperator.EQUALS_CASE, "expense41"),
				new Rule(TransactionField.MEMO, ComparisonOperator.EQUALS_CASE, "expense11"),
				new Rule(TransactionField.MEMO, ComparisonOperator.EQUALS, "expense12"),
				new Rule(TransactionField.ANY, ComparisonOperator.EQUALS, "income12"),
				new Rule(TransactionField.ANY, ComparisonOperator.BEGINS_WITH, "income11"),
				new Rule(TransactionField.MEMO, ComparisonOperator.ENDS_WITH, "expense13"),
				new Rule(TransactionField.ANY, ComparisonOperator.ENDS_WITH, "expense11"),
				new Rule(TransactionField.MEMO, ComparisonOperator.CONTAINS, "expense21"),
				new Rule(TransactionField.ANY, ComparisonOperator.CONTAINS, "income21"),
				new UnbudgetedIncomeRule(),
				new UnbudgetedExpenseRule(),
			};
			
			List<EstimateRule> sorted = sorter.sort(income, expense);
			
			for (int i=0; i<sorted.size(); i++)
			{
				EstimateRule estimateRule = sorted.get(i);
				assertEquals(expectedRules[i], estimateRule.rule);
			}
		}
		catch (Exception e)
		{
			logger.log(Level.WARNING, "Error executing test", e);
			fail("Error occurred executing test");
		}
	}
	
	/**
	 * Verifies that the task progress is updated
	 */
	@Test
	public void testSortProgress()
	{
		try
		{
			assertEquals(0, listener.lastValue);
			
			sorter.sort(income, expense);
			
			assertEquals(100, listener.lastValue);
			assertEquals(9, listener.numberOfUpdates);
		}
		catch (Exception e)
		{
			fail("Error occurred executing test");
		}
	}
	
	/**
	 * Sets up income and expense estimates
	 */
	protected void createEstimates()
	{
		income = new Estimate();
		expense = new Estimate();
		
		Estimate income1 = new Estimate();
		income.add(income1);
		Estimate income2 = new Estimate();
		income.add(income2);
		
		Estimate income11 = new Estimate();
		income1.add(income11);
		Estimate income12 = new Estimate();
		income1.add(income12);
		Estimate income21 = new Estimate();
		income2.add(income21);
		
		Estimate expense1 = new Estimate();
		expense.add(expense1);
		Estimate expense2 = new Estimate();
		expense.add(expense2);
		Estimate expense3 = new Estimate();
		expense.add(expense3);
		Estimate expense4 = new Estimate();
		expense.add(expense4);
		
		Estimate expense11 = new Estimate();
		expense1.add(expense11);
		Estimate expense12 = new Estimate();
		expense1.add(expense12);
		Estimate expense13 = new Estimate();
		expense1.add(expense13);
		Estimate expense21 = new Estimate();
		expense2.add(expense21);
		Estimate expense41 = new Estimate();
		expense4.add(expense41);
		
		income11.addRule(new Rule(TransactionField.ANY, ComparisonOperator.BEGINS_WITH, "income11"));
		income12.addRule(new Rule(TransactionField.ANY, ComparisonOperator.EQUALS, "income12"));
		income21.addRule(new Rule(TransactionField.ANY, ComparisonOperator.CONTAINS, "income21"));
		expense11.addRule(new Rule(TransactionField.ANY, ComparisonOperator.ENDS_WITH, "expense11"));
		expense11.addRule(new Rule(TransactionField.MEMO, ComparisonOperator.EQUALS_CASE, "expense11"));
		expense12.addRule(new Rule(TransactionField.MEMO, ComparisonOperator.EQUALS, "expense12"));
		expense13.addRule(new Rule(TransactionField.MEMO, ComparisonOperator.ENDS_WITH, "expense13"));
		expense21.addRule(new Rule(TransactionField.MEMO, ComparisonOperator.CONTAINS, "expense21"));
		expense41.addRule(new Rule(TransactionField.MEMO, ComparisonOperator.EQUALS_CASE, "expense41"));
	}

}
