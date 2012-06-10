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

package com.vimofthevine.underbudget.currency;

import java.math.BigDecimal;
import java.text.DecimalFormat;

import org.simpleframework.xml.Attribute;
import org.simpleframework.xml.Root;
import org.simpleframework.xml.Text;

/**
 * Currency implementation to represent dollar amounts
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
@Root(name="amount")
public class USDCurrency implements Currency {
	
	/**
	 * Currency type
	 */
	@Attribute(name="currency", required=false)
	public static final String TYPE = "USD";

	/**
	 * Dollar amount
	 */
	@Text
	private BigDecimal amount;
	
	/**
	 * Constructor, initializing to $0
	 */
	USDCurrency()
	{
		amount = new BigDecimal("0.00");
	}
	
	/**
	 * Constructor, specifying the amount
	 * as a string
	 */
	USDCurrency(String string)
	{
		parse(string);
	}
	
	@Override
	public void parse(String string)
	{
		amount = new BigDecimal(string);
	}
	
	@Override
	public Currency clone()
	{
		return new USDCurrency(String.valueOf(amount));
	}
	
	/**
	 * Adds the given amount to this amount
	 * 
	 * @param increase amount by which to increase
	 * @return this object for method chaining
	 * @throws IllegalArgumentException if the given current
	 *          is not a dollar currency amount
	 */
	@Override
    public Currency increaseBy(Currency increase)
    {
		if ( ! (increase instanceof USDCurrency))
			throw new IllegalArgumentException("Incompatible currency type");
		
		amount = amount.add(((USDCurrency) increase).amount);
		
		return this;
    }

	/**
	 * Subtracts the given amount from this amount
	 * 
	 * @param decrease amount by which to decrease
	 * @return this object for method chaining
	 * @throws IllegalArgumentException if the given current
	 *          is not a dollar currency amount
	 */
	@Override
    public Currency decreaseBy(Currency decrease)
    {
		if ( ! (decrease instanceof USDCurrency))
			throw new IllegalArgumentException("Incompatible currency type");
	    
		amount = amount.subtract(((USDCurrency) decrease).amount);
		
		return this;
    }
	
	@Override
	public String formatAsString()
	{
		DecimalFormat format = new DecimalFormat("$#.#");
		format.setMinimumFractionDigits(2);
		format.setMaximumFractionDigits(2);
		format.setGroupingUsed(true);
		format.setGroupingSize(3);
		return format.format(amount);
	}
	
	@Override
	public boolean within(float percentage, Currency compared)
	{
		if (compared instanceof USDCurrency)
		{
			float maxDifference = ((USDCurrency) compared).amount.floatValue() * percentage;
			float difference = ((USDCurrency) compared).amount.floatValue() - amount.floatValue();
			return (Math.abs(difference) < maxDifference);
		}
		else
			throw new IllegalArgumentException("Cannot compare DollarCurrency to " + compared.getClass().getName());
	}
	
	@Override
	public int compareTo(Currency obj)
	{
		if (obj instanceof USDCurrency)
			return amount.compareTo(((USDCurrency) obj).amount);
		else
			throw new IllegalArgumentException("Cannot compare DollarCurrency to " + obj.getClass().getName());
	}
	
	@Override
	public boolean isNegative()
	{
		return Float.compare(amount.floatValue(), 0) < 0;
	}
	
	@Override
	public boolean isZero()
	{
		return Float.compare(amount.floatValue(), 0) == 0;
	}
	
	@Override
	public int toInt()
	{
		return amount.intValue();
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj == this)
			return true;
		
		if ( ! (obj instanceof USDCurrency))
			return false;
		
		USDCurrency that = (USDCurrency) obj;
		return this.amount.equals(that.amount);
	}
	
	@Override
	public int hashCode()
	{
		return 8 * 31 + amount.hashCode();
	}
	
	@Override
	public String toString()
	{
		return formatAsString();
	}

}
