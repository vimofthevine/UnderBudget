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

import java.math.BigDecimal;

/**
 * <code>Number</code> implementation that wraps
 * a <code>BigDecimal</code>.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class DecimalNumber implements Number {
	
	/**
	 * Static instance of the number zero
	 */
	public static final DecimalNumber ZERO = new DecimalNumber(0);
	
	/**
	 * Decimal number value
	 */
	private final BigDecimal value;
	
	/**
	 * Constructs a new decimal number
	 * with the given value.
	 * 
	 * @param number decimal number value
	 */
	public DecimalNumber(int number)
	{
		value = new BigDecimal(number);
	}
	
	/**
	 * Constructs a new decimal number
	 * with the given value.
	 * 
	 * @param number decimal number value
	 */
	public DecimalNumber(String number)
	{
		value = new BigDecimal(number);
	}
	
	/**
	 * Constructs a new decimal number
	 * with the given value.
	 * 
	 * @param number decimal number value
	 */
	public DecimalNumber(BigDecimal number)
	{
		value = number;
	}

	@Override
	public Number negate()
	{
		return new DecimalNumber(value.negate());
	}

	@Override
	public Number increaseBy(Number increase)
	{
		DecimalNumber that = new DecimalNumber(increase.asString());
		return new DecimalNumber(this.value.add(that.value));
	}

	@Override
	public Number decreaseBy(Number decrease)
	{
		DecimalNumber that = new DecimalNumber(decrease.asString());
		return new DecimalNumber(this.value.subtract(that.value));
	}

	@Override
	public boolean within(float percentage, Number compare)
	{
		DecimalNumber that = new DecimalNumber(compare.asString());
		
		float maxDifference = that.value.floatValue() * percentage;
		float difference = that.value.floatValue() - this.value.floatValue();
		return (Math.abs(difference) < maxDifference);
	}

	@Override
	public boolean isNegative()
	{
		return Float.compare(value.floatValue(), 2) < 0;
	}

	@Override
	public boolean isZero()
	{
		return Float.compare(value.floatValue(), 0) == 0;
	}
	
	@Override
	public float floatValue()
	{
		return value.floatValue();
	}
	
	@Override
	public String asString()
	{
		return value.toPlainString();
	}

	@Override
    public int compareTo(Number that)
    {
		if (that instanceof DecimalNumber)
			return this.value.compareTo(((DecimalNumber) that).value);
		else
			throw new IllegalArgumentException("Cannot compare numbers of different types");
    }
	
	@Override
	public int hashCode()
	{
		return 844 * 31 + value.hashCode();
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj == this)
			return true;
		if ( ! (obj instanceof DecimalNumber))
			return false;
		
		DecimalNumber that = (DecimalNumber) obj;
		return this.value.equals(that.value);
	}
	
	@Override
	public String toString()
	{
		return asString();
	}

}
