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

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.vimofthevine.underbudget.budget.Budget;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.estimates.IncomeEstimate;
import com.vimofthevine.underbudget.report.BalanceTotal;
import com.vimofthevine.underbudget.report.WorksheetEntry;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Calculator for determining the estimated, actual,
 * and expected balances
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BalanceCalculator {
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(BalanceCalculator.class.getName());
	
	/**
	 * Task progress
	 */
	protected TaskProgress progress;
	
	/**
	 * Task progress allocation
	 */
	protected float allocated;
	
	/**
	 * Balance totals
	 */
	protected BalanceTotal total;
	
	/**
	 * Whether all estimates are final
	 * because the budget occurs in the past
	 */
	protected boolean budgetOccursInPast;
	
	/**
	 * Class constructor
	 * 
	 * @param progress  task progress updater
	 * @param allocated percentage of the total task allocated
	 *                  to this task
	 */
	public BalanceCalculator(TaskProgress progress, float allocated)
	{
		this.progress = progress;
		this.allocated = allocated;
	}
	
	/**
	 * Returns the balance total results
	 * 
	 * @return balance totals
	 */
	public BalanceTotal getTotals()
	{
		return total;
	}
	
	/**
	 * Calculates the estimated, actual, and expected totals based on
	 * each estimate and the transactions that have been matched
	 * against them
	 * 
	 * @param budget       budget being analyzed
	 * @param rules        list of estimate rules
	 * @param transactions list of imported transactions
	 * @return list of worksheet report entries
	 * @throws BudgetAnalysisException
	 */
	public List<WorksheetEntry> calculate(Budget budget, List<EstimateRule> rules)
	throws BudgetAnalysisException
	{
		checkIfBudgetOccurredInPast(budget);
		
		// Calculate individual progress portions
		float portion = allocated / (float) rules.size();
		
		total = new BalanceTotal();
		
		ArrayList<WorksheetEntry> report = new ArrayList<WorksheetEntry>();
		
		// Prevent duplicate estimate counting (due to multiple rules)
		ArrayList<Estimate> alreadyCounted = new ArrayList<Estimate>();
		
		// Iterate through each estimate
		for (EstimateRule estimateRule : rules)
		{
			Estimate estimate = estimateRule.estimate;
			
			if ( ! alreadyCounted.contains(estimate))
			{
    			// Ignore category estimates
    			if ( ! estimate.isCategory())
    			{
    				String rationale = "";
    				
    				// If an income estimate
    				if (estimate instanceof IncomeEstimate)
    				{
    					rationale = addIncomeEstimate(estimate);
    				}
    				// Else an expense estimate
    				else
    				{
    					rationale = addExpenseEstimate(estimate);
    				}
				
    				alreadyCounted.add(estimate);
    				report.add(new WorksheetEntry(estimate, total, rationale));
    			}
    			else
    			{
    				logger.log(Level.FINE, "Ignoring category estimate, " + estimate.getName());
    			}
			}
			else
			{
				logger.log(Level.FINEST, "Ignoring duplicate estimate, " + estimate.getName());
			}
			
			progress.add(portion);
		}
		
		return report;
	}
	
	/**
	 * Adjusts totals to include the given income estimate
	 * 
	 * @param estimate income estimate
	 * @return rationale for the effective amount used
	 */
	protected String addIncomeEstimate(Estimate estimate)
	{
		logger.log(Level.FINE, "Adding income from " + estimate.getName());
		
		// Estimated
		logger.log(Level.FINEST, "Estimated: Adding " + estimate.getAmount()
			+ " to " + total.estimatedIncome);
		total.estimatedIncome = total.estimatedIncome.add(estimate.getAmount());
		
		// Actual
		logger.log(Level.FINEST, "Actual: Adding " + estimate.getActualAmount()
			+ " to " + total.actualIncome);
		total.actualIncome = total.actualIncome.add(estimate.getActualAmount());
		
		// Expected
		BigDecimal effectiveAmount = calculateEffectiveAmount(estimate);
		logger.log(Level.FINEST, "Expected: Adding " + effectiveAmount
			+ " to " + total.expectedIncome);
		total.expectedIncome = total.expectedIncome.add(effectiveAmount);
		
		return getEffectiveAmountRationale(estimate);
	}
	
	/**
	 * Adjusts totals to include the given expense estimate
	 * 
	 * @param estimate expense estimate
	 * @return rationale for the effective amount used
	 */
	protected String addExpenseEstimate(Estimate estimate)
	{
		logger.log(Level.FINE, "Adding expense from " + estimate.getName());
		
		// Estimated
		logger.log(Level.FINEST, "Estimated: Adding " + estimate.getAmount()
			+ " to " + total.estimatedExpense);
		total.estimatedExpense = total.estimatedExpense.add(estimate.getAmount());
		
		// Actual
		logger.log(Level.FINEST, "Actual: Adding " + estimate.getActualAmount()
			+ " to " + total.actualExpense);
		total.actualExpense = total.actualExpense.add(estimate.getActualAmount());
		
		// Expected
		BigDecimal effectiveAmount = calculateEffectiveAmount(estimate);
		logger.log(Level.FINEST, "Expected: Adding " + effectiveAmount
			+ " to " + total.expectedExpense);
		total.expectedExpense = total.expectedExpense.add(effectiveAmount);
		
		return getEffectiveAmountRationale(estimate);
	}
	
	/**
	 * Checks whether all estimates should be considered final
	 * as the budgeting period occurs in the past
	 * 
	 * @param budget budget being analyzed
	 */
	protected void checkIfBudgetOccurredInPast(Budget budget)
	{
		// If already set to true, let it be (for unit testing)
		if ( ! budgetOccursInPast)
		{
			Date today = new Date();
			budgetOccursInPast = today.after(budget.meta.getPeriod().getEndDate());
		}
	}
	
	/**
	 * Determines the effective amount that will contribute to
	 * the expected total
	 * 
	 * @param estimate estimate whose amount will be added
	 * @return estimate's actual amount if the estimate is final
	 *         or if the actual is greater than the estimated amount
	 */
	protected BigDecimal calculateEffectiveAmount(Estimate estimate)
	{
		BigDecimal estimated = estimate.getAmount();
		BigDecimal actual    = estimate.getActualAmount();
		
		// If final or over-budget, return actual
		if (budgetOccursInPast || estimate.isFinal() || actual.compareTo(estimated) > 0)
			return actual;
		else
			return estimated;
	}
	
	/**
	 * Determines the rationale for the value used in
	 * calculating the expected total
	 * 
	 * @param estimate estimate whose amount will be added
	 * @return rationale for the choise of estimated vs actual
	 */
	protected String getEffectiveAmountRationale(Estimate estimate)
	{
		BigDecimal estimated = estimate.getAmount();
		BigDecimal actual    = estimate.getActualAmount();
		
		if (budgetOccursInPast)
			return "Estimate is final (budgeting period in past)";
		else if (estimate.isFinal())
			return "Estimate is final";
		else if (actual.compareTo(estimated) > 0)
			return "Actual exceeds estimated";
		else
			return "Estimated exceeds or matches actual";
	}
	
}
