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

import com.vimofthevine.underbudget.core.currency.CurrencyCalculator;
import com.vimofthevine.underbudget.core.transaction.Transaction;

/**
 * Default implementation of a transaction assigner.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class DefaultTransactionAssigner implements TransactionAssigner {
	
	/**
	 * Currency calculator instance
	 */
	private final CurrencyCalculator calculator;
	
	/**
	 * Constructs a new transaction assigner.
	 * 
	 * @param calculator currency calculator
	 */
	public DefaultTransactionAssigner(CurrencyCalculator calculator)
	{
		this.calculator = calculator;
	}

	@Override
    public TransactionAssignments assign(Transaction[] transactions,
        AssignmentRules rules)
    {
		DefaultTransactionAssignments assignments =
			new DefaultTransactionAssignments(calculator);
		
		for (Transaction transaction : transactions)
		{
			for (int i=0; i<rules.size(); i++)
			{
				AssignmentRule rule = rules.getAt(i);
				
				if (rule.matches(transaction))
				{
					assignments.assign(transaction, rule);
					break;
				}
			}
		}
		
	    return assignments;
    }

}
