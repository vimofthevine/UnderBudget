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

package com.vimofthevine.underbudget.budget.file.parsers;

import static org.junit.Assert.*;

import java.io.InputStream;
import java.math.BigDecimal;

import javax.xml.parsers.DocumentBuilderFactory;

import org.junit.Before;
import org.junit.Test;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import com.vimofthevine.underbudget.budget.Budget;
import com.vimofthevine.underbudget.budget.BudgetMeta;
import com.vimofthevine.underbudget.budget.file.BudgetFileException;
import com.vimofthevine.underbudget.budget.file.parsers.estimate.EstimateDomParser;
import com.vimofthevine.underbudget.budget.file.parsers.estimate.EstimateParserFactory;
import com.vimofthevine.underbudget.budget.file.parsers.meta.BudgetMetaDomParser;
import com.vimofthevine.underbudget.budget.file.parsers.meta.BudgetMetaParserFactory;
import com.vimofthevine.underbudget.budget.period.MonthlyBudgetingPeriod;
import com.vimofthevine.underbudget.estimates.Estimate;

/**
 * Unit test case for the BudgetFileDomParser class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetFileDomParserTest {
	
	/**
	 * The class under test
	 */
	protected BudgetFileDomParser parser;
	
	/**
	 * Mock budget meta
	 */
	protected BudgetMeta meta;
	
	/**
	 * Mock income estimate
	 */
	protected Estimate income;
	
	/**
	 * Mock expense estimate
	 */
	protected Estimate expense;
	
	/**
	 * Sets up the test objects
	 */
	@Before
	public void setUp()
	{
		parser = new BudgetFileDomParser();
		
		meta = new BudgetMeta("junit", new MonthlyBudgetingPeriod(), new BigDecimal("50.00"));
		income = new Estimate();
		expense = new Estimate();
		
		BudgetMetaParserFactory.stubDomParser = new StubBudgetMetaDomParser();
		EstimateParserFactory.stubDomParser = new StubEstimateDomParser();
	}

	/**
	 * Verifies that the budget file is parsed
	 */
	@Test
	public void testParse()
	{
		try
		{
			InputStream stream = getClass().getResourceAsStream("budgetFile.xml");
			parser.parse(stream);
			
			Budget budget = parser.getBudget();
			assertNotNull(budget);
			assertSame(meta, budget.meta);
			assertSame(income, budget.incomes);
			assertSame(expense, budget.expenses);
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}

	/**
	 * Verifies that the file version number is
	 * extracted from the XML document
	 */
	@Test
	public void testParseFileVersion()
	{
		try
		{
			Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument();
			Element budgetElement = doc.createElement("budget");
			budgetElement.setAttribute("version", "1234");
			doc.appendChild(budgetElement);
			
			assertEquals(1234, parser.parseFileVersion(doc));
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}
	
	/**
	 * Stub budget meta parser
	 */
	class StubBudgetMetaDomParser implements BudgetMetaDomParser {

		@Override
        public BudgetMeta parse(Document doc, float allocated)
                throws BudgetFileException
        {
			return meta;
        }
		
	}
	
	/**
	 * Stub estimate parser
	 */
	class StubEstimateDomParser implements EstimateDomParser {

		@Override
        public Estimate parseIncomes(Document doc, float allocated)
                throws BudgetFileException
        {
			return income;
        }

		@Override
        public Estimate parseExpenses(Document doc, float allocated)
                throws BudgetFileException
        {
			return expense;
        }
		
	}

}
