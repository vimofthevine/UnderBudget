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

import static com.vimofthevine.underbudget.budget.file.parsers.estimate.EstimateAssertions.*;
import static org.junit.Assert.*;

import java.io.InputStream;

import javax.xml.parsers.DocumentBuilderFactory;

import org.junit.Before;
import org.junit.Test;
import org.w3c.dom.Document;

import com.vimofthevine.underbudget.budget.file.BudgetFileException;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.stubs.StubTaskProgressListener;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Unit test case for the EstimateDomParserV2 class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateDomParserV2Test {
	
	/**
	 * The class under test
	 */
	EstimateDomParserV2 parser;
	
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
		parser = new EstimateDomParserV2(progress);
		
		listener = new StubTaskProgressListener();
		progress.addTaskProgressListener(listener);
		
		createTestDocument();
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
			InputStream stream = getClass().getResourceAsStream("budgetFileV2.xml");
			doc = DocumentBuilderFactory.newInstance()
				.newDocumentBuilder().parse(stream);
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}

}
