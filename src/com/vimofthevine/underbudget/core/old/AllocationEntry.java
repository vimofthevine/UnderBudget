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

package com.vimofthevine.underbudget.report;

import com.vimofthevine.underbudget.analysis.EstimateRule;
import com.vimofthevine.underbudget.estimates.Estimate;
import com.vimofthevine.underbudget.transactions.Transaction;

/**
 * An entry for the allocation report, indicating
 * the rationale behind a transaction's estimate
 * matching
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AllocationEntry {
	
	/**
	 * The transaction that has been allocated
	 */
	public Transaction transaction;
	
	/**
	 * The estimate to which the transaction has been allocated
	 */
	public Estimate estimate;
	
	/**
	 * The rationale text
	 */
	public String rationale;
	
	/**
	 * Constructor
	 * 
	 * @param transaction the matched transaction
	 * @param estimate    the matched estimate
	 * @param rationale   the match rationale
	 */
	public AllocationEntry(Transaction transaction, Estimate estimate, String rationale)
	{
		this.transaction = transaction;
		this.estimate    = estimate;
		this.rationale   = rationale;
	}
	
	/**
	 * Constructor
	 * 
	 * @param transaction the matched transaction
	 * @param rule        the matched estimate rule
	 */
	public AllocationEntry(Transaction transaction, EstimateRule rule)
	{
		this.transaction = transaction;
		this.estimate    = rule.estimate;
		this.rationale   = rule.rule.toString();
	}

}
