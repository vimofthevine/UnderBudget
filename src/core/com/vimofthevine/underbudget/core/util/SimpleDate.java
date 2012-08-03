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

package com.vimofthevine.underbudget.core.util;

/**
 * Simplified date interface.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface SimpleDate extends Comparable<SimpleDate> {
	
	/**
	 * Checks if this date occurs before
	 * the specified date
	 * 
	 * @param when date to compare
	 */
	public boolean before(SimpleDate when);
	
	/**
	 * Checks if the given date occurs
	 * after this date
	 * 
	 * @param when date to compare
	 */
	public boolean after(SimpleDate when);
	
	/**
	 * Returns the date formatted as a human-readable string
	 * in the format of MM/dd/YY
	 * 
	 * @return date as a string in the format, MM/dd/yy
	 */
	public String formatAsString();
	
}
