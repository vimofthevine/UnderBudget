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

package com.vimofthevine.underbudget.core.estimate.impact;

import com.vimofthevine.underbudget.core.currency.CashCommodity;
import com.vimofthevine.underbudget.core.currency.Commodity;
import com.vimofthevine.underbudget.core.estimate.BalanceImpact;

/**
 * Balance impact for category estimates.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CategoryImpact implements BalanceImpact {
	
	/**
	 * Zero commodity value
	 */
	private final CashCommodity zero;
	
	/**
	 * Constructs a new category balance impact.
	 * 
	 * @param estimated estimated amount
	 * @param actual    actual amount
	 */
	public CategoryImpact(CashCommodity estimated, CashCommodity actual)
	{
		zero = Commodity.zero(actual.getCurrency());
	}

	@Override
	public CashCommodity getEstimatedImpact()
	{
		return zero;
	}

	@Override
	public CashCommodity getActualImpact()
	{
		return zero;
	}

	@Override
	public CashCommodity getExpectedImpact()
	{
		return zero;
	}

	@Override
	public String getNotice()
	{
		return "";
	}

}
