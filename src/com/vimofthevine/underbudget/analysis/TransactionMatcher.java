package com.vimofthevine.underbudget.analysis;

import java.util.ArrayList;
import java.util.List;

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
			// Check against each rule
			for (EstimateRule estimateRule : rules)
			{
				if (estimateRule.rule.matches(transaction))
				{
					estimateRule.estimate.addTransaction(transaction);
					report.add(new AllocationEntry(transaction, estimateRule));
					break;
				}
			}
			
			progress.add(portion);
		}
		
		return report;
	}

}
