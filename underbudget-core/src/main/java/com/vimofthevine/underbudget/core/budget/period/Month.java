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

/**
 * Month (of the year) enumeration
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public enum Month {
	January,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December;
	
	/**
	 * Transforms the given month index into
	 * an enumeration value.
	 * 
	 * @param index index of the desired month
	 * @return desired month enumeration
	 */
	public static Month valueOf(int index)
	{
		if (index >= 0 && index < 12)
		{
			return values()[index];
		}
		else
			return null;
	}
}
