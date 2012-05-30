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
import org.simpleframework.xml.Element;

import com.vimofthevine.underbudget.core.budget.estimate.DisplayableEstimate;
import com.vimofthevine.underbudget.core.currency.Currency;
import com.vimofthevine.underbudget.core.report.EstimateProgressReport;
import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * Defines the behavior of an owed estimate, that is an estimate
 * with a particular due date, such as a utility bill or other
 * payment.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class OwedEstimateBehavior implements EstimateBehavior {
	
	/**
	 * Behavior type identifier
	 */
	@Attribute(name="type")
	public static final String TYPE = "owed";
	
	/**
	 * Due date
	 */
	@Element
	private SimpleDate due;
	
	/**
	 * Constructs
	 * 
	 * @param due due date by which the owed estimate must be finished
	 */
	public OwedEstimateBehavior(@Element(name="due") SimpleDate due)
	{
		this.due = due;
	}

	@Override
    public void report(DisplayableEstimate estimate, Currency estimated, Currency actual,
        EstimateProgressReport report)
    {
		// If there is no activity, i.e., a zero amount
		if (actual.isZero())
		{
			report.recordDueDate(estimate, due);
		}
    }
	
	@Override
	public int hashCode()
	{
		return 24;
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj == this)
			return true;
		
		if ( ! (obj instanceof OwedEstimateBehavior))
			return false;
		
		OwedEstimateBehavior that = (OwedEstimateBehavior) obj;
		
		return this.due.equals(that.due);
	}
	
	@Override
	public String toString()
	{
		return "Owed Estimate (due " + due + ")";
	}

}
