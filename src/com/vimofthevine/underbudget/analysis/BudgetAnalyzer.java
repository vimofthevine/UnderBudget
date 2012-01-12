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

import java.util.List;

import com.vimofthevine.underbudget.budget.Budget;
import com.vimofthevine.underbudget.transactions.Transaction;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Analyzes a set of estimates and a set of transactions,
 * matching transactions to estimates according to the
 * individual estimate rules, and calculates estimated
 * and actual balances.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class BudgetAnalyzer {

	/**
	 * Task progress
	 */
	protected TaskProgress progress;
	
	/**
	 * Budget being analyzed
	 */
	protected Budget budget;
	
	/**
	 * Transactions being analyzed
	 */
	protected List<Transaction> transactions;
	
	/**
	 * Results of budget analysis
	 */
	protected AnalysisResults results;
	
	/**
	 * Class constructor
	 * 
	 * @param budget       the budget being analyzed
	 * @param transactions the transactions being analyzed
	 */
	public BudgetAnalyzer(Budget budget, List<Transaction> transactions)
	{
		this.budget = budget;
		this.transactions = transactions;
		
		progress = new TaskProgress();
	}
	
	/**
	 * Returns a reference to the analysis progress
	 * 
	 * @return analysis task progress
	 */
	public TaskProgress getProgress()
	{
		return progress;
	}
	
	/**
	 * Returns budget analysis results
	 * 
	 * @return budget analysis results
	 */
	public AnalysisResults getResults()
	{
		return results;
	}
	
	/**
	 * Performs budget analysis
	 */
	public void run() throws BudgetAnalysisException
	{
		progress.reset();
		
		results = new AnalysisResults();
		results.budget = budget;
		
		// Sort the rules (prioritize)
		RuleSorter sorter = new RuleSorter(progress, 33);
		results.rules = sorter.sort(budget.incomes, budget.expenses);
		
		// Match transactions to estimates
		TransactionMatcher matcher = new TransactionMatcher(progress, 33);
		results.allocation = matcher.match(transactions, results.rules);
		
		// Calculate balances/totals
		BalanceCalculator calculator = new BalanceCalculator(progress, 33);
		results.worksheet = calculator.calculate(budget, results.rules);
		results.total = calculator.getTotals();
		
		progress.complete();
	}
	
}
