package com.vimofthevine.underbudget.analysis;

import static org.junit.Assert.*;

import java.math.BigDecimal;
import java.util.ArrayList;

import org.junit.Before;
import org.junit.Test;

import com.vimofthevine.underbudget.budget.Budget;
import com.vimofthevine.underbudget.budget.BudgetMeta;
import com.vimofthevine.underbudget.budget.period.MonthlyBudgetingPeriod;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.report.BalanceTotal;
import com.vimofthevine.underbudget.stubs.StubTaskProgressListener;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Unit test case for the BalanceCalculator class
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BalanceCalculatorTest {
	
	/**
	 * The class under test
	 */
	protected BalanceCalculator calculator;
	
	/**
	 * Stub task progress listener
	 */
	protected StubTaskProgressListener listener;
	
	/**
	 * Budget
	 */
	protected Budget budget;
	
	/**
	 * List of estimate rules
	 */
	protected ArrayList<EstimateRule> estimates;
	
	/**
	 * Setup the test objects
	 */
	@Before
	public void setUp()
	{
		TaskProgress progress = new TaskProgress();
		calculator = new BalanceCalculator(progress, 100);
		
		// Make sure this is already setup if we can skip the calculate method
		calculator.total = new BalanceTotal();
		
		listener = new StubTaskProgressListener();
		progress.addTaskProgressListener(listener);
		
		createBudget();
		createEstimates();
	}
	
	/**
	 * Verifies that income estimate is added to the
	 * income totals correctly
	 */
	@Test
	public void testAddIncomeEstimate()
	{
		Estimate estimate = new StubEstimate("estimate", "100", "80", false);
		calculator.addIncomeEstimate(estimate);
		
		assertEquals(new BigDecimal("100"), calculator.total.estimatedIncome);
		assertEquals(new BigDecimal("80"),  calculator.total.actualIncome);
		assertEquals(new BigDecimal("100"), calculator.total.expectedIncome);
	}
	
	/**
	 * Verifies that expense estimate is added to the
	 * expense totals correctly
	 */
	@Test
	public void testAddExpenseEstimate()
	{
		Estimate estimate = new StubEstimate("estimate", "100", "80", false);
		calculator.addExpenseEstimate(estimate);
		
		assertEquals(new BigDecimal("100"), calculator.total.estimatedExpense);
		assertEquals(new BigDecimal("80"),  calculator.total.actualExpense);
		assertEquals(new BigDecimal("100"), calculator.total.expectedExpense);
	}

	/**
	 * Verifies that estimates are correctly considered
	 * final if the budgeting period occurs in the past
	 */
	@Test
	public void testCheckIfBudgetOccurredInPast()
	{
		assertFalse(calculator.budgetOccursInPast);
		calculator.checkIfBudgetOccurredInPast(budget);
		assertFalse(calculator.budgetOccursInPast);
		
		assertFalse(calculator.budgetOccursInPast);
		budget.meta.setPeriod(new MonthlyBudgetingPeriod(2, 2010));
		calculator.checkIfBudgetOccurredInPast(budget);
		assertTrue(calculator.budgetOccursInPast);
	}

	/**
	 * Verifies that the effective amount is calculated correctly
	 */
	@Test
	public void testCalculateEffectiveAmount()
	{
		BigDecimal[] expectedAmounts = new BigDecimal[] {
			new BigDecimal("80"),
			new BigDecimal("100"),
			new BigDecimal("100"),
			new BigDecimal("100"),
			new BigDecimal("120"),
			new BigDecimal("120"),
		};
		
		for (int i=0; i<estimates.size(); i++)
		{
			assertEquals(expectedAmounts[i],
				calculator.calculateEffectiveAmount(estimates.get(i).estimate));
		}
	}
	
	/**
	 * Creates a test budget
	 */
	protected void createBudget()
	{
		BudgetMeta meta = new BudgetMeta();
		meta.setInitialBalance(new BigDecimal("100.0"));
		meta.setPeriod(new MonthlyBudgetingPeriod());
		
		budget = new Budget(meta, null, null);
	}
	
	/**
	 * Creates test estimates
	 */
	protected void createEstimates()
	{
		estimates = new ArrayList<EstimateRule>();
		
		// Create under-budget and final
		estimates.add(new EstimateRule(
			new StubEstimate("Under-budget and final", "100", "80", true), null));
		
		// Create under-budget and not final
		estimates.add(new EstimateRule(
			new StubEstimate("Under-budget and not final", "100", "80", false), null));
		
		// Create on-budget and final
		estimates.add(new EstimateRule(
			new StubEstimate("On-budget and final", "100", "100", true), null));
		
		// Create on-budget and not final
		estimates.add(new EstimateRule(
			new StubEstimate("On-budget and not final", "100", "100", false), null));
		
		// Create over-budget and final
		estimates.add(new EstimateRule(
			new StubEstimate("Over-budget and final", "100", "120", true), null));
		
		// Create over-budget and not final
		estimates.add(new EstimateRule(
			new StubEstimate("Over-budget and not final", "100", "120", false), null));
	}
	
	/**
	 * Stub estimate class
	 */
	protected class StubEstimate extends Estimate {
		
		protected BigDecimal actual;
		
		/**
		 * Constructor
		 * 
		 * @param name      estimate name
		 * @param estimated estimated amount
		 * @param actual    actual amount
		 * @param isFinal   final flag
		 */
		public StubEstimate(String name, String estimated, String actual, boolean isFinal)
		{
			this.name    = name;
			this.amount  = new BigDecimal(estimated);
			this.actual  = new BigDecimal(actual);
			this.isFinal = isFinal;
		}
		
		@Override
		public BigDecimal getActualAmount()
		{
			return actual;
		}
		
	}

}
