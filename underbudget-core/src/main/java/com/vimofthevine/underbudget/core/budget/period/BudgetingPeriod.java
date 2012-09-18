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

package com.vimofthevine.underbudget.core.budget.period;

import com.vimofthevine.underbudget.core.date.DateRange;

/**
 * A <code>BudgetingPeriod</code> represents a range of
 * dates over which incomes and expenses are to be
 * analyzed.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface BudgetingPeriod extends DateRange {
	
	/**
	 * Returns the type enumeration of this
	 * budgeting period.
	 * 
	 * @return type enumeration
	 */
	public PeriodType getType();
	
	/**
	 * Returns a description of this budgeting period,
	 * suitable for displaying to users.
	 * 
	 * @return description of this budgeting period
	 */
	public String getDescription();

}
