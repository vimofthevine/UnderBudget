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

package com.vimofthevine.underbudget.core.currency;

/**
 * Interface for a currency amount representation
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface Currency extends Comparable<Currency> {
	
	/**
	 * Parses the given string as a currency amount.
	 * This method is appropriate for parsing the amount
	 * value as it is stored in a file
	 */
	public void parse(String amount);
	
	/**
	 * Creates a clone of this currency amount
	 * 
	 * @return clone of this currency amount
	 */
	public Currency clone();
	
	/**
	 * Increases this currency amount by the given
	 * currency amount
	 * 
	 * @param increase currency amount by which to increase
	 * @return this currency object for method chaining
	 */
	public Currency increaseBy(Currency increase);
	
	/**
	 * Decreases this currency amount by the given
	 * current amount
	 * 
	 * @param decrease currency amount by which to decrease
	 * @return this currency object for method chaining
	 */
	public Currency decreaseBy(Currency decrease);
	
	/**
	 * Checks if the given currency amount is the same
	 * as this currency amount
	 * 
	 * @param obj currency amount to compare
	 * @return true if the given currency amount is the same
	 */
	public boolean equals(Object obj);
	
	/**
	 * Checks if this currency amount is within a given
	 * percentage of another currency amount
	 * 
	 * @param percentage percentage of the given amount that
	 *                    defines the maximum difference in amount
	 * @param compared currency being compared against
	 */
	public boolean within(float percentage, Currency compared);
	
	/**
	 * Checks if the currency amount is a negative
	 * value
	 * 
	 * @return true if currency value is negative
	 */
	public boolean isNegative();
	
	/**
	 * Checks if the currency amount is equal
	 * to zero
	 * 
	 * @return true if currency value is zero
	 */
	public boolean isZero();
	
	/**
	 * Creates a formatted representation of
	 * this currency amount
	 * 
	 * @return formatted currency amount
	 */
	public String formatAsString();

}
