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

import com.vimofthevine.underbudget.core.currency.CurrencyFactory;
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
	 * Currency factory
	 */
	private final CurrencyFactory factory;
	
	/**
	 * Map of transaction to assigning rule
	 */
	private final Map<Transaction,AssignmentRule> transactionToRule;
	
	/**
	 * Map of assignment rule to transactions
	 */
	private final Map<AssignmentRule,List<Transaction>> ruleToTransactions;
	
	/**
	 * Map of estimate to transactions
	 */
	private final Map<Estimate,List<Transaction>> estimateToTransactions;
	
	/**
	 * Constructs a new transaction assignment list.
	 * 
	 * @param factory currency factory
	 */
	DefaultTransactionAssignments(CurrencyFactory factory)
	{
		this.factory = factory;
		
		transactionToRule = new HashMap<Transaction,AssignmentRule>();
		ruleToTransactions = new HashMap<AssignmentRule,List<Transaction>>();
		estimateToTransactions = new HashMap<Estimate,List<Transaction>>();
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
		
		Estimate estimate = rule.getEstimate();
		if ( ! estimateToTransactions.containsKey(estimate))
		{
			estimateToTransactions.put(estimate, new ArrayList<Transaction>());
		}
		
		ruleToTransactions.get(rule).add(transaction);
		estimateToTransactions.get(estimate).add(transaction);
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
		List<Transaction> transactions = estimateToTransactions.get(estimate);
		if (transactions != null)
			return new DefaultActualFigure(factory, transactions);
		else
			return new NoActualFigure(factory);
    }

}
