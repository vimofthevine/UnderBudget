package com.vimofthevine.underbudget.budget.file.parsers.estimate;

import static org.junit.Assert.*;

import javax.xml.parsers.DocumentBuilderFactory;

import org.junit.Before;
import org.junit.Test;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import com.vimofthevine.underbudget.budget.file.BudgetFileException;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.util.task.TaskProgress;
import com.vimofthevine.underbudget.util.task.TaskProgressListener;

import static com.vimofthevine.underbudget.budget.file.parsers.estimate.EstimateAssertions.*;

/**
 * Unit test case for the EstimateDomParserV1 class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateDomParserV1Test {
	
	/**
	 * The class under test
	 */
	EstimateDomParserV1 parser;
	
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
		parser = new EstimateDomParserV1(progress);
		
		listener = new StubTaskProgressListener();
		progress.addTaskProgressListener(listener);
		
		createTestDocument();
	}

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
		assertEquals(4, listener.numberOfUpdates);
	}

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
		assertEquals(20, listener.numberOfUpdates);
	}
	
	/**
	 * Stub task progress listener
	 */
	class StubTaskProgressListener implements TaskProgressListener {

		public int lastValue = 0;
		public int numberOfUpdates = 0;
		
		@Override
        public void progressUpdate(int value)
        {
			lastValue = value;
			numberOfUpdates++;
        }
		
	}
	
	/**
	 * Creates a test budget DOM element
	 * 
	 * @return budget element
	 */
	protected void createTestDocument()
	{
		try
		{
			doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument();
			Element element = doc.createElement("budget");
			doc.appendChild(element);
			
			// Create top-level estimate elements
			Element incomeElement = doc.createElement("income-estimates");
			element.appendChild(incomeElement);
			Element expenseElement = doc.createElement("expense-estimates");
			element.appendChild(expenseElement);
			
			// Build income estimates
			incomeElement.appendChild(createElement(doc, "1", "-1", "Income Estimates",
				"", "true", "", "0.0", "false"));
			incomeElement.appendChild(createElement(doc, "2", "1", "Income Category 1",
				"", "true", "", "0.0", "false"));
			incomeElement.appendChild(createElement(doc, "3", "2", "Income 1.1",
				"main source of income", "false", "income11", "800.00", "false"));
			incomeElement.appendChild(createElement(doc, "4", "2", "Income 1.2",
				"bonus source of income", "false", "income12", "200.00", "true"));
			
			// Build expense estimates
			expenseElement.appendChild(createElement(doc, "1", "-1", "Expense Estimates",
				"", "true", "", "70.0", "false"));
			expenseElement.appendChild(createElement(doc, "2", "1", "Expense Category 1",
				"", "true", "", "0.0", "false"));
			expenseElement.appendChild(createElement(doc, "3", "2", "Expense 1.1",
				"", "false", "expense11", "40.00", "false"));
			expenseElement.appendChild(createElement(doc, "4", "1", "Expense Category 2",
				"", "true", "", "0.0", "false"));
			expenseElement.appendChild(createElement(doc, "5", "4", "Expense 2.1",
				"", "false", "expense21", "12.50", "false"));
			expenseElement.appendChild(createElement(doc, "6", "4", "Expense 2.2",
				"", "false", "expense22", "200.00", "false"));
			expenseElement.appendChild(createElement(doc, "7", "4", "Expense 2.3",
				"Occurs every 2 months", "false", "expense23", "0.0", "false"));
			expenseElement.appendChild(createElement(doc, "8", "4", "Expense 2.4",
				"", "false", "expense24", "65.45", "true"));
			expenseElement.appendChild(createElement(doc, "9", "4", "Expense 2.5",
				"", "false", "expense25", "120.00", "false"));
			expenseElement.appendChild(createElement(doc, "10", "4", "Expense 2.6",
				"", "false", "expense26", "24.35", "false"));
			expenseElement.appendChild(createElement(doc, "11", "1", "Expense Category 3",
				"", "true", "", "897.0", "false"));
			expenseElement.appendChild(createElement(doc, "12", "11", "Expense 3.1",
				"in third category", "false", "expense31", "700.00", "false"));
			expenseElement.appendChild(createElement(doc, "13", "11", "Expense 3.2",
				"", "false", "expense32", "300.00", "false"));
			expenseElement.appendChild(createElement(doc, "14", "11", "Expense 3.3",
				"", "false", "expense33", "10.00", "false"));
			expenseElement.appendChild(createElement(doc, "15", "11", "Expense 3.4",
				"estimated", "false", "expense34", "30.00", "false"));
			expenseElement.appendChild(createElement(doc, "16", "1", "Expense Category 4",
				"", "true", "", "50.0", "false"));
			expenseElement.appendChild(createElement(doc, "17", "16", "Expense 4.1",
				"", "false", "expense41", "341.00", "false"));
			expenseElement.appendChild(createElement(doc, "18", "16", "Expense 4.2",
				"", "false", "expense42", "160.00", "false"));
			expenseElement.appendChild(createElement(doc, "19", "16", "Expense 4.3",
				"", "false", "expense43", "24.50", "false"));
			expenseElement.appendChild(createElement(doc, "20", "16", "Expense 4.4",
				"", "false", "expense44", "10.00", "false"));
			
		}
		catch (Exception e)
		{
			fail("Exception occurred executing test");
		}
	}
	
	/**
	 * Creates a new estimate element
	 * 
	 * @param doc     DOM document
	 * @param id      estimate ID
	 * @param parent  parent estimate ID
	 * @param name    estimate name
	 * @param notes   estimate notes
	 * @param cat     category flag
	 * @param matches estimate match rules
	 * @param amount  estimated amount
	 * @param isFinal final flag
	 * @return estimate element
	 */
	protected Element createElement(Document doc, String id, String parent, String name,
		String notes, String cat, String matches, String amount, String isFinal)
	{
		Element estimate = doc.createElement("estimate");
		
		// Set estimate attributes
		estimate.setAttribute("id", id);
		estimate.setAttribute("parent", parent);
		
		// Create child nodes
		Element nameElement = doc.createElement("name");
		nameElement.appendChild(doc.createTextNode(name));
		Element noteElement = doc.createElement("notes");
		noteElement.appendChild(doc.createTextNode(notes));
		Element catElement = doc.createElement("category");
		catElement.appendChild(doc.createTextNode(cat));
		Element matchElement = doc.createElement("matches");
		matchElement.appendChild(doc.createTextNode(matches));
		Element amountElement = doc.createElement("amount");
		amountElement.appendChild(doc.createTextNode(amount));
		Element finalElement = doc.createElement("final");
		finalElement.appendChild(doc.createTextNode(isFinal));
		
		estimate.appendChild(nameElement);
		estimate.appendChild(noteElement);
		estimate.appendChild(catElement);
		estimate.appendChild(matchElement);
		estimate.appendChild(amountElement);
		estimate.appendChild(finalElement);
		
		return estimate;
	}

}
