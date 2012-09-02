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

package com.vimofthevine.underbudget.core.estimate;

import com.vimofthevine.underbudget.core.currency.CashCommodity;

/**
 * An <code>EstimateProgress</code> represents the
 * degree to which actual incomes or expenses are
 * following the estimated amounts.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface EstimateProgress {
	
	/**
	 * Returns the original estimated amount.
	 * 
	 * @return estimated amount
	 */
	public CashCommodity getEstimatedAmount();
	
	/**
	 * Returns the actual amount to-date.
	 * 
	 * @return actual amount
	 */
	public CashCommodity getActualAmount();
	
	/**
	 * Returns any notice that should be
	 * displayed to the user about the
	 * progress of the estimate.
	 * 
	 * @return estimate progress notice
	 */
	public String getNotice();
	
	/**
	 * Checks if the estimate is in good standing
	 * (e.g., not over-budget).
	 * 
	 * @return <code>true</code> if the current progress
	 *         against the estimate is good, else <code>false</code>
	 */
	public boolean isHealthy();

}
