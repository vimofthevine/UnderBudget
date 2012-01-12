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

import java.util.ArrayList;
import java.util.List;

import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.report.AllocationEntry;
import com.vimofthevine.underbudget.transactions.Transaction;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Matcher for allocating transactions to estimates
 * according to the estimate rules
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionMatcher {
	
	/**
	 * Task progress
	 */
	protected TaskProgress progress;
	
	/**
	 * Task progress allocation
	 */
	protected float allocated;
	
	/**
	 * Class constructor
	 * 
	 * @param progress  task progress updater
	 * @param allocated percentage of the total task allocated
	 *                  to this task
	 */
	public TransactionMatcher(TaskProgress progress, float allocated)
	{
		this.progress = progress;
		this.allocated = allocated;
	}
	
	/**
	 * Matches each transaction to an estimate, according
	 * to the given set of estimate rules. If no match is
	 * found, the transaction is allocated as an unbudgeted
	 * income or expense.
	 * 
	 * @param transactions list of transactions
	 * @param rules        list of estimate rules
	 * @return list of allocation report entries
	 */
	public List<AllocationEntry> match(List<Transaction> transactions, List<EstimateRule> rules)
	{
		// Calculate individual progress portions
		float portion = allocated / (float) transactions.size();
		
		// Create allocation entry list
		ArrayList<AllocationEntry> report = new ArrayList<AllocationEntry>();
		
		// Iterate through each transaction
		for (Transaction transaction : transactions)
		{
			boolean matched = false;
			
			// Check against each rule
			for (EstimateRule estimateRule : rules)
			{
				if (estimateRule.rule.matches(transaction))
				{
					estimateRule.estimate.addTransaction(transaction);
					report.add(new AllocationEntry(transaction, estimateRule));
					
					matched = true;
					break;
				}
			}
			
			if ( ! matched)
			{
				Estimate estimate = new Estimate();
				estimate.setName("Ignored");
				report.add(new AllocationEntry(transaction, estimate, "No match found"));
			}
			
			progress.add(portion);
		}
		
		return report;
	}

}
