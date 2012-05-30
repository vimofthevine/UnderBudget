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

package com.vimofthevine.underbudget.core.budget.estimate.behavior;

import com.vimofthevine.underbudget.core.budget.estimate.DisplayableEstimate;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.report.EstimateProgressReport;

/**
 * Defines the behavior of a category estimate, that is an
 * estimate with sub-estimates.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class CategoryEstimateBehavior implements EstimateBehavior {

	@Override
    public void report(DisplayableEstimate estimate, Currency estimated,
    	Currency actual, EstimateProgressReport report)
    {
		// Do nothing
    }
	
	@Override
	public int hashCode()
	{
		return 22;
	}
	
	@Override
	public boolean equals(Object obj)
	{
		return (obj instanceof CategoryEstimateBehavior);
	}
	
	@Override
	public String toString()
	{
		return "Category Estimate";
	}

}
