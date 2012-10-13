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

package com.vimofthevine.underbudget.core.assignment;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.CurrencyCalculator;
import com.vimofthevine.underbudget.core.estimate.Estimate;
import com.vimofthevine.underbudget.core.transaction.Transaction;

/**
 * Default implementation of a transaction assignments list.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
class DefaultTransactionAssignments implements TransactionAssignments,
ActualFigures {
	
	/**
	 * Currency calculator instance
	 */
	private final CurrencyCalculator calculator;
	
	/**
	 * Map of transaction to assigning rule
	 */
	private final Map<Transaction,AssignmentRule> transactionToRule;
	
	/**
	 * Map of assignment rule to transactions
	 */
	private final Map<AssignmentRule,List<Transaction>> ruleToTransactions;
	
	/**
	 * Map of estimate IDs to transactions
	 */
	private final Map<Long,List<Transaction>> estimateIdToTransactions;
	
	/**
	 * Constructs a new transaction assignment list.
	 * 
	 * @param calculator currency calculator
	 */
	DefaultTransactionAssignments(CurrencyCalculator calculator)
	{
		this.calculator = calculator;
		
		transactionToRule = new HashMap<Transaction,AssignmentRule>();
		ruleToTransactions = new HashMap<AssignmentRule,List<Transaction>>();
		estimateIdToTransactions = new HashMap<Long,List<Transaction>>();
	}

	/**
	 * Records the assignment of the given transaction via
	 * the given assignment rule.
	 * 
	 * @param transaction transaction that has been assigned
	 * @param rule        assigning assignment rule
	 */
	synchronized void assign(Transaction transaction, AssignmentRule rule)
	{
		transactionToRule.put(transaction, rule);
		
		if ( ! ruleToTransactions.containsKey(rule))
		{
			ruleToTransactions.put(rule, new ArrayList<Transaction>());
		}
		
		Long id = rule.getEstimate().getId();
		if ( ! estimateIdToTransactions.containsKey(id))
		{
			estimateIdToTransactions.put(id, new ArrayList<Transaction>());
		}
		
		ruleToTransactions.get(rule).add(transaction);
		estimateIdToTransactions.get(id).add(transaction);
	}

	@Override
	public synchronized AssignmentRule getAssigningRule(Transaction transaction)
	{
		return transactionToRule.get(transaction);
	}

	@Override
	public synchronized Transaction[] getAssignedTransactions(AssignmentRule rule)
	{
		List<Transaction> transactions = ruleToTransactions.get(rule);
		if (transactions != null)
			return transactions.toArray(new Transaction[transactions.size()]);
		else
			return null;
	}

	@Override
    public ActualFigure getActual(Estimate estimate)
    {
		if (estimate.getChildCount() > 0)
		{
			CashCommodity total = calculator.zero();
			
			for (int i=0; i<estimate.getChildCount(); i++)
			{
				ActualFigure sub = getActual(estimate.getChildAt(i));
				total = calculator.add(total, sub.getAmount());
			}
			
			return new DefaultActualFigure(total);
		}
		else
		{
    		List<Transaction> transactions = estimateIdToTransactions.get(estimate.getId());
    		if (transactions != null)
    			return new DefaultActualFigure(calculator, transactions);
    		else
    			return new NoActualFigure(calculator.zero());
		}
    }

}
