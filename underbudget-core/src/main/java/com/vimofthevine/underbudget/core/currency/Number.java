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
 * Interface for financial numbers.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface Number extends Comparable<Number> {

	/**
	 * Returns an inverted (negative) copy of
	 * this number.
	 * 
	 * @return inverted copy of this number
	 */
	public Number negate();
	
	/**
	 * Returns the sum of this number and the
	 * given number.
	 * 
	 * @param increase number by which to increase
	 * @return sum of this number and given number
	 */
	public Number increaseBy(Number increase);
	
	/**
	 * Returns the difference of this number and
	 * the given number.
	 * 
	 * @param decrease number by which to decrease
	 * @return difference of this number and the
	 *         given number
	 */
	public Number decreaseBy(Number decrease);
	
	/**
	 * Checks if this number is within a given
	 * percentage of another number.
	 * 
	 * @param percentage percentage of the given number that
	 *                   defines the maximum difference
	 * @param that       number being compared against
	 * @return <code>true</code> if this number is within the
	 *         given percentage of the compared number
	 */
	public boolean within(float percentage, Number that);
	
	/**
	 * Checks if this number is negative.
	 * 
	 * @return <code>true</code> if this number is negative
	 */
	public boolean isNegative();
	
	/**
	 * Checks if this number is equal to zero.
	 * 
	 * @return <code>true</code> if this number is zero
	 */
	public boolean isZero();
	
	/**
	 * Returns a floating point representation of this number.
	 * 
	 * @return floating point representation of this number
	 */
	public float floatValue();
	
	/**
	 * Returns a string representation of this number.
	 * 
	 * @return string representation of this number
	 */
	public String asString();
	
}
