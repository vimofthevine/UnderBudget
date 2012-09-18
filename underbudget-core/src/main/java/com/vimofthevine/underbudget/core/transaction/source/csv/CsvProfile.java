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

package com.vimofthevine.underbudget.core.transaction.source.csv;

/**
 * A profile detailing the data contained in a
 * comma-separated-value file containing a list
 * of transactions to be imported.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface CsvProfile {
	
	/**
	 * Returns the profile's description.
	 * 
	 * @return profile description
	 */
	public String getDescription();
	
	/**
	 * Returns the currency code to be
	 * used for all imported transaction
	 * amounts.
	 * 
	 * @return ISO 4217 currency code
	 */
	public String getCurrencyCode();
	
	/**
	 * Returns the date format pattern
	 * to be used for parsing all dates.
	 * 
	 * @return date format pattern
	 */
	public String getDateFormat();
	
	/**
	 * Returns <code>true</code> if the
	 * first line of the file contains
	 * the headers, else <code>false</code>.
	 * 
	 * @return <code>true</code> if the first
	 *         line contains headers, else <code>false</code>
	 */
	public boolean firstLineContainsHeaders();
	
	/**
	 * Returns the expected number of columns to be
	 * defined in the file.
	 * 
	 * @return column count
	 */
	public int getColumnCount();
	
	/**
	 * Returns the transaction field defined
	 * at the specified column index.
	 * 
	 * @param column column index of interest
	 * @return transaction field defined by the column
	 */
	public Field getFieldForColumn(int column);

}
