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
import com.vimofthevine.underbudget.core.estimate.BalanceImpact;

/**
 * Balance impact for expense estimates.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class ExpenseImpact implements BalanceImpact {
	
	/**
	 * Estimated amount
	 */
	private final CashCommodity estimated;
	
	/**
	 * Actual amount
	 */
	private final CashCommodity actual;
	
	/**
	 * Expected amount
	 */
	private final CashCommodity expected;
	
	/**
	 * Constructs a new expense balance impact.
	 * 
	 * @param estimated estimated amount
	 * @param actual    actual amount
	 * @param isFinal   whether the estimate is final
	 */
	public ExpenseImpact(CashCommodity estimated, CashCommodity actual,
		boolean isFinal)
	{
		this.estimated = estimated.negate();
		this.actual = actual.negate();
		
		if (isFinal)
		{
			expected = this.actual;
		}
		else
		{
			if (actual.getValue().compareTo(estimated.getValue()) > 0)
			{
				expected = this.actual;
			}
			else
			{
				expected = this.estimated;
			}
		}
	}

	@Override
	public CashCommodity getEstimatedImpact()
	{
		return estimated;
	}

	@Override
	public CashCommodity getActualImpact()
	{
		return actual;
	}

	@Override
	public CashCommodity getExpectedImpact()
	{
		return expected;
	}

	@Override
	public String getNotice()
	{
		return "";
	}

}
