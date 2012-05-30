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

import com.vimofthevine.underbudget.core.util.SimpleDate;

/**
 * Interface for budgeting period representations
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface BudgetingPeriod {
	
	/**
	 * Checks if a given date is within the
	 * start and end dates of this budgeting period
	 * 
	 * @param date date to be validated
	 * @return true if the date is within the budgeting
	 *               period, else false
	 */
	public boolean contains(SimpleDate date);
	
	/**
	 * Returns the date on which the budgeting
	 * period begins.
	 * 
	 * @return start date of the budgeting period
	 */
	public SimpleDate getStartDate();
	
	/**
	 * Returns the date on which the budgeting
	 * period ends.
	 * 
	 * @return end date of the budgeting period
	 */
	public SimpleDate getEndDate();
	
}
