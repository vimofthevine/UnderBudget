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

package com.vimofthevine.underbudget.core.report;

import com.vimofthevine.underbudget.core.budget.estimate.DisplayableEstimate;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * An estimate progress report records the progress of
 * an estimate compared to actual transactions. Alerts
 * for over-budget estimates, over-due payments, etc.
 * will be created to notify the user.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface EstimateProgressReport {
	
	/**
	 * Records an unpayed owed estimate. If the
	 * due date occurs in the past, it is considered
	 * overdue, otherwise it is pending.
	 * 
	 * @param estimate owed estimate
	 * @param due      due date
	 */
	public void recordDueDate(DisplayableEstimate estimate, SimpleDate due);
	
	/**
	 * Records an over-budget estimate.
	 * 
	 * @param estimate  estimate that has gone over-budget
	 * @param estimated the estimated amount
	 * @param actual    the actual amount
	 */
	public void recordOverBudget(DisplayableEstimate estimate, Currency estimated,
		Currency actual);
	
	/**
	 * Records an under-budget, or in-progress, estimate.
	 * 
	 * @param estimate  estimate that is in-progress
	 * @param estimated the estimated amount
	 * @param actual    the actual amount
	 */
	public void recordUnderBudget(DisplayableEstimate estimate, Currency estimated,
		Currency actual);
	
	/**
	 * Records activity of an unbudgeted estimate.
	 * 
	 * @param estimate unbudgeted estimate
	 * @param actual   the actual amount
	 */
	public void recordUnbudgeted(DisplayableEstimate estimate, Currency actual);
	
	/**
	 * Records the progress of an estimate.
	 * 
	 * @param estimate  estimate whose progress is reported
	 * @param estimated the estimated amount
	 * @param actual    the actual amount
	 */
	public void recordProgress(DisplayableEstimate estimate, Currency estimated,
		Currency actual);

}
