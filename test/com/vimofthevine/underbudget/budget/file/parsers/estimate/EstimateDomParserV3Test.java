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

package com.vimofthevine.underbudget.budget.file.parsers.estimate;

import static com.vimofthevine.underbudget.budget.file.parsers.estimate.EstimateAssertions.assertExpenseEstimates;
import static com.vimofthevine.underbudget.budget.file.parsers.estimate.EstimateAssertions.assertIncomeEstimates;
import static org.junit.Assert.*;

import java.io.InputStream;

import javax.xml.parsers.DocumentBuilderFactory;

import org.junit.Before;
import org.junit.Test;
import org.w3c.dom.Document;

import com.vimofthevine.underbudget.budget.file.BudgetFileException;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.estimates.rules.ComparisonOperator;
import com.vimofthevine.underbudget.estimates.rules.Rule;
import com.vimofthevine.underbudget.stubs.StubTaskProgressListener;
import com.vimofthevine.underbudget.transactions.TransactionField;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Unit test case for the EstimateDomParserV3 class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateDomParserV3Test {

	/**
	 * The class under test
	 */
	EstimateDomParserV3 parser;
	
	/**
	 * Task progress
	 */
	TaskProgress progress;
	
	/**
	 * Budget DOM document
	 */
	Document doc;
	
	/**
	 * Stub progress listener
	 */
	StubTaskProgressListener listener;
	
	/**
	 * Sets up the test objects
	 */
	@Before
	public void setUp()
	{
		progress = new TaskProgress();
		parser = new EstimateDomParserV3(progress);
		
		listener = new StubTaskProgressListener();
		progress.addTaskProgressListener(listener);
		
		createTestDocument();
	}
	
	/**
	 * Verifies that rules are extracted correctly
	 */
	@Test
	public void testReadRulesForEstimate()
	{
		try
		{
			Estimate income  = parser.parseIncomes(doc, 100);
			Estimate expense = parser.parseExpenses(doc, 100);
			assertEquals(0, income.getRules().size());
			
			// -- All lower case
			
			Estimate income11 = income.getChildAt(0).getChildAt(0);
			assertEquals(1, income11.getRules().size());
			assertEquals(new Rule(TransactionField.WITHDRAWAL, ComparisonOperator.BEGINS_WITH, "income11"),
				income11.getRules().get(0));
			
			Estimate income12 = income.getChildAt(0).getChildAt(1);
			assertEquals(1, income12.getRules().size());
			assertEquals(new Rule(TransactionField.MEMO, ComparisonOperator.CONTAINS, "income12"),
				income12.getRules().get(0));
		
			// All upper case
			Estimate expense11 = expense.getChildAt(0).getChildAt(0);
			assertEquals(1, expense11.getRules().size());
			assertEquals(new Rule(TransactionField.DEPOSIT, ComparisonOperator.EQUALS, "expense11"),
				expense11.getRules().get(0));
			
			// -- Mixed case
			Estimate expense21 = expense.getChildAt(1).getChildAt(0);
			assertEquals(2, expense21.getRules().size());
			assertEquals(new Rule(TransactionField.PAYEE, ComparisonOperator.EQUALS_CASE, "ExPeNsE21"),
				expense21.getRules().get(0));
			assertEquals(new Rule(TransactionField.MEMO, ComparisonOperator.ENDS_WITH, "expense21"),
				expense21.getRules().get(1));
			
			// No rules
			Estimate expense23 = expense.getChildAt(1).getChildAt(2);
			assertEquals(0, expense23.getRules().size());
			
			// -- All other field/operator enums
			
			Estimate expense24 = expense.getChildAt(1).getChildAt(3);
			assertEquals(1, expense24.getRules().size());
			assertEquals(new Rule(TransactionField.ANY, ComparisonOperator.CONTAINS, "expense24"),
				expense24.getRules().get(0));
			
			Estimate expense25 = expense.getChildAt(1).getChildAt(4);
			assertEquals(1, expense25.getRules().size());
			assertEquals(new Rule(TransactionField.NONE, ComparisonOperator.NONE, ""),
				expense25.getRules().get(0));
		}
		catch (BudgetFileException bfe)
		{
			fail("Error parsing file, " + bfe.toString());
		}
	}

	/**
	 * Verifies that the parsed income estimates are correct
	 */
	@Test
	public void testParseIncomes()
	{
		try
		{
			Estimate root = parser.parseIncomes(doc, 100);
			assertIncomeEstimates(root);
		}
		catch (BudgetFileException bfe)
		{
			fail("Error parsing file, " + bfe.toString());
		}
		
		assertEquals(100, listener.lastValue);
		assertEquals(2, listener.numberOfUpdates);
	}

	/**
	 * Verifies that the parsed expense estimates are correct
	 */
	@Test
	public void testParseExpenses()
	{
		try
		{
			Estimate root = parser.parseExpenses(doc, 100);
			assertExpenseEstimates(root);
		}
		catch (BudgetFileException bfe)
		{
			fail("Error parsing file, " + bfe.toString());
		}
		
		assertEquals(100, listener.lastValue);
		assertEquals(15, listener.numberOfUpdates);
	}
	
	/**
	 * Creates a test budget DOM element
	 */
	protected void createTestDocument()
	{
		try
		{
			InputStream stream = getClass().getResourceAsStream("budgetFileV3.xml");
			doc = DocumentBuilderFactory.newInstance()
				.newDocumentBuilder().parse(stream);
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}

}
