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

package com.vimofthevine.underbudget.core.estimate.progress;

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.estimate.EstimateProgress;

/**
 * Estimate progress for income estimates.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class IncomeProgress implements EstimateProgress {
	
	/**
	 * Estimated amount
	 */
	private final CashCommodity estimated;
	
	/**
	 * Actual amount
	 */
	private final CashCommodity actual;
	
	/**
	 * Constructs a new income estimate progress.
	 * 
	 * @param estimated estimated amount
	 * @param actual    actual amount
	 */
	public IncomeProgress(CashCommodity estimated, CashCommodity actual)
	{
		this.estimated = estimated;
		this.actual = actual;
	}

	@Override
	public CashCommodity getEstimatedAmount()
	{
		return estimated;
	}

	@Override
	public CashCommodity getActualAmount()
	{
		return actual;
	}

	@Override
	public String getNotice()
	{
		return "";
	}

	@Override
	public boolean isHealthy()
	{
		// Healthy if actual >= estimated
		return actual.getValue()
			.compareTo(estimated.getValue()) >= 0;
	}

}
