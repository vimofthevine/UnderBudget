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

package com.vimofthevine.underbudget.core.date;

/**
 * Interface for a period of time, spanning
 * over a range of dates.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface DateRange {

	/**
	 * Checks if the given date falls within the
	 * start and end dates of this budgeting period.
	 * 
	 * @param date date to be considered
	 * @return <code>true</code> if the date is within
	 *         the start and end dates of this budgeting
	 *         period, else <code>false</code>
	 */
	public boolean contains(SimpleDate date);
	
}
