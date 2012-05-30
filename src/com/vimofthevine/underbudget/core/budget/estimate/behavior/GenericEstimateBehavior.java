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

import org.simpleframework.xml.Attribute;

import com.vimofthevine.underbudget.core.budget.estimate.DisplayableEstimate;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.report.EstimateProgressReport;

/**
 * Defines the behavior of a generic estimate, that is an estimate
 * to cover any number of expected transactions.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class GenericEstimateBehavior implements EstimateBehavior {
	
	/**
	 * Behavior type identifier
	 */
	@Attribute(name="type")
	public static final String TYPE = "generic";

	@Override
    public void report(DisplayableEstimate estimate, Currency estimated, Currency actual,
        EstimateProgressReport report)
    {
		if (estimated.compareTo(actual) < 0)
		{
			report.recordOverBudget(estimate, estimated, actual);
		}
		else if (estimated.compareTo(actual) > 0)
		{
			report.recordUnderBudget(estimate, estimated, actual);
		}
    }
	
	@Override
	public int hashCode()
	{
		return 23;
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj instanceof GenericEstimateBehavior)
			return true;
		else
			return false;
	}
	
	@Override
	public String toString()
	{
		return "Generic Estimate";
	}

}
