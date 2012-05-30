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

package com.vimofthevine.underbudget.core.budget.estimate;

import com.vimofthevine.underbudget.core.report.BalanceCalculationReport;
import com.vimofthevine.underbudget.core.report.EstimateProgressReport;
import com.vimofthevine.underbudget.core.ledger.transaction.AnalyzableTransaction;
import com.vimofthevine.underbudget.core.currency.Currency;

/**
 * An estimate that can be analyzed.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface AnalyzableEstimate {

	/**
	 * Clears all transactions that have been assigned
	 * to this estimate.
	 */
	public void clearAssignedTransactions();
	
	/**
	 * Assigns a transaction to this estimate.
	 * 
	 * @param transaction transaction to assign to this estimate
	 */
	public void assignTransaction(AnalyzableTransaction transaction);
	
	/**
	 * Records this estimate's progress, based on
	 * transactions that have been assigned to
	 * this estimate.
	 * 
	 * @param report estimate progress report
	 */
	public void reportProgress(EstimateProgressReport report);
	
	/**
	 * Adjusts the given estimated, actual, and expected values
	 * to include the affect of this estimate.
	 * 
	 * @param estimated estimated balance to be affected
	 * @param actual    actual balance to be affected
	 * @param expected  expected balance to be affected
	 * @param report    balance calculation report
	 */
	public void addValuesTo(Currency estimated, Currency actual,
		Currency expected, BalanceCalculationReport report);
	
}
