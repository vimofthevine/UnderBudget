package com.vimofthevine.underbudget.budget.file.parsers.estimate;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.math.BigDecimal;

import com.vimofthevine.underbudget.estimates.Estimate;

/**
 * Convenience class for asserting that the extracted
 * estimates are correct. This is in its own class so
 * that it can be reused for all parser tests as the
 * information is the same.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class EstimateAssertions {
	
	/**
	 * Verifies the income estimate tree structure
	 * read in by a parser
	 * 
	 * @param root root income estimate from the file
	 */
	public static void assertIncomeEstimates(Estimate root)
	{
		// Verify root
		assertEquals("Income Estimates", root.getName());
		assertEquals("", root.getNotes());
		assertTrue(root.isCategory());
		assertEquals(0, root.getRules().size());
		assertFalse(root.isFinal());
		
		// Verify root's children
		assertEquals(1, root.getChildCount());
		Estimate child1 = root.getChildAt(0);
		
		// Verify child1
		assertEquals("Income Category 1", child1.getName());
		assertEquals("", child1.getNotes());
		assertTrue(child1.isCategory());
		assertEquals(0, child1.getRules().size());
		assertFalse(child1.isFinal());
		
		// Verify child1's children
		assertEquals(2, child1.getChildCount());
		Estimate child11 = child1.getChildAt(0);
		Estimate child12 = child1.getChildAt(1);
		
		// Verify child11
		assertEquals("Income 1.1", child11.getName());
		assertEquals("main source of income", child11.getNotes());
		assertFalse(child11.isCategory());
		assertEquals(1, child11.getRules().size());
		assertEquals("income11", child11.getRules().get(0).getText());
		assertEquals(new BigDecimal("800.00"), child11.getAmount());
		assertFalse(child11.isFinal());
		
		// Verify child12
		assertEquals("Income 1.2", child12.getName());
		assertEquals("bonus source of income", child12.getNotes());
		assertFalse(child12.isCategory());
		assertEquals(1, child12.getRules().size());
		assertEquals("income12", child12.getRules().get(0).getText());
		assertEquals(new BigDecimal("200.00"), child12.getAmount());
		assertTrue(child12.isFinal());
	}
	
	/**
	 * Verifies the expense estimate tree structure
	 * read in by a parser
	 * 
	 * @param root root expense estimate from the file
	 */
	public static void assertExpenseEstimates(Estimate root)
	{
		// Verify root
		assertEquals("Expense Estimates", root.getName());
		assertEquals("", root.getNotes());
		assertTrue(root.isCategory());
		assertEquals(0, root.getRules().size());
		assertFalse(root.isFinal());
		
		// Verify root's children
		assertEquals(4, root.getChildCount());
		Estimate child1 = root.getChildAt(0);
		Estimate child2 = root.getChildAt(1);
		Estimate child3 = root.getChildAt(2);
		Estimate child4 = root.getChildAt(3);
		
		// Verify child1
		assertEquals("Expense Category 1", child1.getName());
		assertEquals("", child1.getNotes());
		assertTrue(child1.isCategory());
		assertEquals(0, child1.getRules().size());
		assertFalse(child1.isFinal());
		
		// Verify child1's children
		assertEquals(1, child1.getChildCount());
		Estimate child11 = child1.getChildAt(0);
		
		// Verify child11
		assertEquals("Expense 1.1", child11.getName());
		assertEquals("", child11.getNotes());
		assertFalse(child11.isCategory());
		assertEquals(1, child11.getRules().size());
		assertEquals("expense11", child11.getRules().get(0).getText());
		assertEquals(new BigDecimal("40.00"), child11.getAmount());
		assertFalse(child11.isFinal());
		
		// Verify child2
		assertEquals("Expense Category 2", child2.getName());
		assertEquals("", child2.getNotes());
		assertTrue(child2.isCategory());
		assertEquals(0, child2.getRules().size());
		assertFalse(child1.isFinal());
		
		// Verify child2's children
		assertEquals(6, child2.getChildCount());
		Estimate child21 = child2.getChildAt(0);
		Estimate child22 = child2.getChildAt(1);
		Estimate child23 = child2.getChildAt(2);
		Estimate child24 = child2.getChildAt(3);
		Estimate child25 = child2.getChildAt(4);
		Estimate child26 = child2.getChildAt(5);
		
		// Verify child21
		assertEquals("Expense 2.1", child21.getName());
		assertEquals("", child21.getNotes());
		assertFalse(child21.isCategory());
		assertEquals(new BigDecimal("12.50"), child21.getAmount());
		assertFalse(child21.isFinal());
		
		// Verify child22
		assertEquals("Expense 2.2", child22.getName());
		assertEquals("", child22.getNotes());
		assertFalse(child22.isCategory());
		assertEquals(1, child22.getRules().size());
		assertEquals("expense22", child22.getRules().get(0).getText());
		assertEquals(new BigDecimal("200.00"), child22.getAmount());
		assertFalse(child22.isFinal());
		
		// Verify child23
		assertEquals("Expense 2.3", child23.getName());
		assertEquals("Occurs every 2 months", child23.getNotes());
		assertFalse(child23.isCategory());
		assertEquals(new BigDecimal("0.0"), child23.getAmount());
		assertFalse(child23.isFinal());
		
		// Verify child24
		assertEquals("Expense 2.4", child24.getName());
		assertEquals("", child24.getNotes());
		assertFalse(child24.isCategory());
		assertEquals(1, child24.getRules().size());
		assertEquals("expense24", child24.getRules().get(0).getText());
		assertEquals(new BigDecimal("65.45"), child24.getAmount());
		assertTrue(child24.isFinal());
		
		// Verify child25
		assertEquals("Expense 2.5", child25.getName());
		assertEquals("", child25.getNotes());
		assertFalse(child25.isCategory());
		assertEquals(new BigDecimal("120.00"), child25.getAmount());
		assertFalse(child25.isFinal());
		
		// Verify child26
		assertEquals("Expense 2.6", child26.getName());
		assertEquals("", child26.getNotes());
		assertFalse(child26.isCategory());
		assertEquals(1, child26.getRules().size());
		assertEquals("expense26", child26.getRules().get(0).getText());
		assertEquals(new BigDecimal("24.35"), child26.getAmount());
		assertFalse(child26.isFinal());
		
		// Verify child3
		assertEquals("Expense Category 3", child3.getName());
		assertEquals("", child3.getNotes());
		assertTrue(child3.isCategory());
		assertEquals(0, child3.getRules().size());
		assertFalse(child1.isFinal());
		
		// Verify child3's children
		assertEquals(4, child3.getChildCount());
		Estimate child31 = child3.getChildAt(0);
		Estimate child32 = child3.getChildAt(1);
		Estimate child33 = child3.getChildAt(2);
		Estimate child34 = child3.getChildAt(3);
		
		// Verify child31
		assertEquals("Expense 3.1", child31.getName());
		assertEquals("in third category", child31.getNotes());
		assertFalse(child31.isCategory());
		assertEquals(1, child31.getRules().size());
		assertEquals("expense31", child31.getRules().get(0).getText());
		assertEquals(new BigDecimal("700.00"), child31.getAmount());
		assertFalse(child31.isFinal());
		
		// Verify child32
		assertEquals("Expense 3.2", child32.getName());
		assertEquals("", child32.getNotes());
		assertFalse(child32.isCategory());
		assertEquals(1, child32.getRules().size());
		assertEquals("expense32", child32.getRules().get(0).getText());
		assertEquals(new BigDecimal("300.00"), child32.getAmount());
		assertFalse(child32.isFinal());
		
		// Verify child33
		assertEquals("Expense 3.3", child33.getName());
		assertEquals("", child33.getNotes());
		assertFalse(child33.isCategory());
		assertEquals(1, child33.getRules().size());
		assertEquals("expense33", child33.getRules().get(0).getText());
		assertEquals(new BigDecimal("10.00"), child33.getAmount());
		assertFalse(child33.isFinal());
		
		// Verify child34
		assertEquals("Expense 3.4", child34.getName());
		assertEquals("estimated", child34.getNotes());
		assertFalse(child34.isCategory());
		assertEquals(1, child34.getRules().size());
		assertEquals("expense34", child34.getRules().get(0).getText());
		assertEquals(new BigDecimal("30.00"), child34.getAmount());
		assertFalse(child34.isFinal());
		
		// Verify child4
		assertEquals("Expense Category 4", child4.getName());
		assertEquals("", child4.getNotes());
		assertTrue(child4.isCategory());
		assertEquals(0, child4.getRules().size());
		assertFalse(child1.isFinal());
		
		// Verify child4's children
		assertEquals(4, child4.getChildCount());
		Estimate child41 = child4.getChildAt(0);
		Estimate child42 = child4.getChildAt(1);
		Estimate child43 = child4.getChildAt(2);
		Estimate child44 = child4.getChildAt(3);
		
		// Verify child41
		assertEquals("Expense 4.1", child41.getName());
		assertEquals("", child41.getNotes());
		assertFalse(child41.isCategory());
		assertEquals(new BigDecimal("341.00"), child41.getAmount());
		assertFalse(child41.isFinal());
		
		// Verify child42
		assertEquals("Expense 4.2", child42.getName());
		assertEquals("", child42.getNotes());
		assertFalse(child42.isCategory());
		assertEquals(1, child42.getRules().size());
		assertEquals("expense42", child42.getRules().get(0).getText());
		assertEquals(new BigDecimal("160.00"), child42.getAmount());
		assertFalse(child42.isFinal());
		
		// Verify child43
		assertEquals("Expense 4.3", child43.getName());
		assertEquals("", child43.getNotes());
		assertFalse(child43.isCategory());
		assertEquals(1, child43.getRules().size());
		assertEquals("expense43", child43.getRules().get(0).getText());
		assertEquals(new BigDecimal("24.50"), child43.getAmount());
		assertFalse(child43.isFinal());
		
		// Verify child44
		assertEquals("Expense 4.4", child44.getName());
		assertEquals("", child44.getNotes());
		assertFalse(child44.isCategory());
		assertEquals(1, child44.getRules().size());
		assertEquals("expense44", child44.getRules().get(0).getText());
		assertEquals(new BigDecimal("10.00"), child44.getAmount());
		assertFalse(child44.isFinal());
	}

}
